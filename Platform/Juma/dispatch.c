#include <stdint.h>
#include <string.h>
#include "nordic_common.h"
#include "nrf.h"
#include "app_error.h"
#include "nrf51_bitfields.h"
#include "nrf_soc.h"
#include "dispatch.h"
#include "app_util_platform.h"

// Timer frequency is 32768/32 = 1024

#define DISPATCH_OP_POOL_SIZE 32

#define RTC1_IRQ_PRI            8//APP_IRQ_PRIORITY_LOW
#define RTC_COUNTER_MASK        0x00FFFFFF
#define MIN_RTC_DELAY           3
#define set_timer(v)             NRF_RTC1->CC[0] = v

#define LATTER_THAN(t1, t2)                         \
    ((((t1)-(t2)) & RTC_COUNTER_MASK) < 0x007fffff)

static dispatch_queue_t freeq;
static dispatch_queue_t timerq;
static dispatch_queue_t mainq;

static operation_t op_pool[DISPATCH_OP_POOL_SIZE];

static void reset_rtc( void );
static void dispatch_head_op(dispatch_queue_t* q, uint32_t now);

void dispatch_init( void )
{
    int i;
    for (i=0; i<DISPATCH_OP_POOL_SIZE-1; i++) {
        op_pool[i].next = &op_pool[i+1];
    }
    op_pool[DISPATCH_OP_POOL_SIZE-1].next = NULL;
    freeq.head = &op_pool[0];
    freeq.tail = &op_pool[DISPATCH_OP_POOL_SIZE-1];

    NRF_RTC1->PRESCALER = 31;
    NVIC_SetPriority(RTC1_IRQn, RTC1_IRQ_PRI);

    NRF_RTC1->EVTENSET = RTC_EVTEN_COMPARE0_Msk;
    NRF_RTC1->INTENSET = RTC_INTENSET_COMPARE0_Msk;

    NVIC_ClearPendingIRQ(RTC1_IRQn);
    NVIC_EnableIRQ(RTC1_IRQn);

    NRF_RTC1->TASKS_START = 1;
}

static operation_t* allocate( void )
{
    operation_t* op;
    if (freeq.head) {
        op = freeq.head;
        freeq.head = op->next;

        if (freeq.head == NULL) {
            freeq.tail = NULL;
        }
        return op;
    } else {
        return NULL;
    }
}

void run_when_idle(function_t func,
                     void* args)
{
    dispatch_queue_t* q;
    operation_t* op;

    q = &mainq;

    op = allocate();

    if (op) {
        op->func = func;
        op->args = args;
        op->timestamp = 0;
        op->next = NULL;
        if (q->tail) {
            // queue is not empty, insert at tail
            q->tail->next = op;
            q->tail = op;
        } else {
            // queue is empty.
            q->head = q->tail = op;
        }
    }
}

uint32_t current_time( void )
{
    return NRF_RTC1->COUNTER;
}

void run_at_time(function_t func,
                    void* args, uint32_t timestamp)
{
    dispatch_queue_t* q;
    operation_t* op;
    operation_t* before;

    q = &timerq;
    op = allocate();

    if (op) {
        op->func = func;
        op->args = args;
        op->timestamp = timestamp;

        before = q->head;

        if ((before == NULL) ||
            (LATTER_THAN(before->timestamp, op->timestamp))) {
            op->next = q->head;
            q->head = op;
            if (q->tail == NULL) {
                q->tail = op;
            }
        } else {
            while (before) {
                if ((before->next == NULL) ||
                    LATTER_THAN(before->next->timestamp, op->timestamp)) {
                    op->next = before->next;
                    before->next = op;

                    if (q->tail == before) {
                        q->tail = op;
                    }
                    break;
                }
                before = before->next;
            }
        }

        reset_rtc();
    }
}

void dispatch( void )
{
    uint32_t more;
    dispatch_queue_t* q;
    uint32_t now;

    q = &timerq;

    do {
        more = 0;
        while (1) {
            if (q->head == NULL) {
                break;
            }

            now = current_time();
            if (LATTER_THAN(now, q->head->timestamp)) {
                dispatch_head_op(q, now);
                more = 1;
            } else {
                break;
            }
        }

        q = &mainq;

        if (q->head) {
            // not empty
            dispatch_head_op(q, 0);
            more = 1;
        }
    } while (more);

    reset_rtc();
}

static void dispatch_head_op(dispatch_queue_t* q, uint32_t now)
{
    operation_t* op;

    op = q->head;
    q->head = q->head->next;
    if (q->head == NULL) q->tail = NULL;
    op->func(op->args);
    if (freeq.tail) {
        // queue is not empty, insert at tail
        freeq.tail->next = op;
        freeq.tail = op;
    } else {
        // queue is empty.
        freeq.head = freeq.tail = op;
    }

}

void RTC1_IRQHandler(void)
{
    // Clear all events (also unexpected ones)
    NRF_RTC1->EVENTS_COMPARE[0] = 0;
    NRF_RTC1->EVENTS_COMPARE[1] = 0;
    NRF_RTC1->EVENTS_COMPARE[2] = 0;
    NRF_RTC1->EVENTS_COMPARE[3] = 0;
    NRF_RTC1->EVENTS_TICK       = 0;
    NRF_RTC1->EVENTS_OVRFLW     = 0;
}

static void reset_rtc( void )
{
    uint32_t now, head_time, next_time;

    if (timerq.head) {
        now = current_time();
        head_time = timerq.head->timestamp;

        if (LATTER_THAN(head_time, now + MIN_RTC_DELAY)) {
            next_time = head_time;
        } else {
            next_time = now + MIN_RTC_DELAY;
        }

        set_timer(next_time);
    } else {
	now = current_time();
        set_timer(now - 1);
    }
}
