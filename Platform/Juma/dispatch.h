#ifndef _DISPATCH_H_
#define _DISPATCH_H_

#include "juma_common.h"

typedef struct _operation_t operation_t;
typedef struct _dispatch_queue_t dispatch_queue_t;

struct _operation_t {
    operation_t* next;
    function_t func;
    void* args;
    uint32_t timestamp;
};

struct _dispatch_queue_t {
    operation_t *head, *tail;
};

void dispatch_init( void );
void run_when_idle(function_t func,
                     void* args);
void run_at_time(function_t func,
                    void* args, uint32_t timestamp);
void dispatch( void );
uint32_t current_time( void );

#endif // _DISPATCH_H_
