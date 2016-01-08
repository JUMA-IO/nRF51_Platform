#ifndef _PWM_H_
#define _PWM_H_

#define PWM_TIMER               NRF_TIMER2
#define PWM_IRQHandler          TIMER2_IRQHandler
#define PWM_IRQn                TIMER2_IRQn
#define PWM_IRQ_PRIORITY        3

void play_notes( uint32_t* freqs, uint8_t num_notes, uint8_t buzzer_pin );

#endif // _PWM_H_
