#include "sys_common.h"

#define NOTE_ON_PERIOD          200
#define NOTE_OFF_PERIOD         50
#define TOTAL_ROUNDS            3
#define INTER_ROUND_SPACE       1500

typedef struct _buzzer_t {
    const uint32_t* freqs;
    uint32_t num_notes;
    uint32_t current_note;
    uint8_t buzzer_pin;
    uint8_t round;
    uint8_t running:1;
} buzzer_t;


static buzzer_t m_buzzer;

static void note_on(void* args);
static void note_off(void* args);

void play_sound( uint8_t pin )
{
    static const uint32_t freqs[] = {1047, 1175, 1319, 1397};

    m_buzzer.buzzer_pin = pin;
    m_buzzer.freqs = freqs;
    m_buzzer.num_notes = 4;
    m_buzzer.current_note = 0;
    m_buzzer.round = 0;
    if (m_buzzer.running == 0) {
        m_buzzer.running = 1;
        run_after_delay(note_on, (void*)&m_buzzer, NOTE_OFF_PERIOD);
    }
}

static void note_on(void* args)
{
    buzzer_t* buzzer = (buzzer_t*) args;
    uint32_t width;

    width = 32768/buzzer->freqs[buzzer->current_note];

    PWM_TIMER->TASKS_CLEAR = 1;

    // setup PWM
    PWM_TIMER->POWER = 1;

    PWM_TIMER->PRESCALER = 9; // 32k
    nrf_gpio_cfg_output(buzzer->buzzer_pin);
    PWM_TIMER->BITMODE = TIMER_BITMODE_BITMODE_16Bit;
    PWM_TIMER->CC[0] = width/4;
    PWM_TIMER->CC[1] = width/2;
    PWM_TIMER->MODE = TIMER_MODE_MODE_Timer;
    PWM_TIMER->SHORTS = TIMER_SHORTS_COMPARE1_CLEAR_Msk;
    PWM_TIMER->EVENTS_COMPARE[0] = PWM_TIMER->EVENTS_COMPARE[1] = 0;

    sd_ppi_channel_assign(0, &PWM_TIMER->EVENTS_COMPARE[0], &NRF_GPIOTE->TASKS_OUT[2]);
    sd_ppi_channel_enable_set(1);

    nrf_gpiote_task_configure(2, buzzer->buzzer_pin, NRF_GPIOTE_POLARITY_TOGGLE, NRF_GPIOTE_INITIAL_VALUE_HIGH);  
    nrf_gpiote_task_enable(2);
    
    PWM_TIMER->TASKS_START = 1;

    buzzer->current_note++;
    
    run_after_delay(note_off, args, NOTE_ON_PERIOD);
}

static void note_off(void* args)
{
    buzzer_t* buzzer = (buzzer_t*) args;

    nrf_gpio_cfg_output(buzzer->buzzer_pin);
    nrf_gpio_pin_write(buzzer->buzzer_pin, 1); //JG TEST
    nrf_gpiote_task_disable(2);
    PWM_TIMER->TASKS_CLEAR = 1;
    PWM_TIMER->POWER = 0;

    if (buzzer->current_note < buzzer->num_notes) {
        run_after_delay(note_on, args, NOTE_OFF_PERIOD);
    } else {
        buzzer->round++;

        if (buzzer->round < TOTAL_ROUNDS) {
            m_buzzer.current_note = 0;
            run_after_delay(note_on, (void*)&m_buzzer, INTER_ROUND_SPACE);
        } else {
            m_buzzer.running = 0;
        }
    }
}
