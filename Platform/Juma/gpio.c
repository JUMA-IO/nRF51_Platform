#include "sys_common.h"

void gpio_init( void )
{
    NRF_GPIOTE->INTENCLR = 0xFFFFFFFF;

    NVIC_ClearPendingIRQ(GPIOTE_IRQn);
    NVIC_SetPriority(GPIOTE_IRQn, APP_IRQ_PRIORITY_LOW);
    NVIC_EnableIRQ(GPIOTE_IRQn);
}

void gpio_setup(uint8_t pin, uint8_t mode)
{
    switch (mode) {
    case GPIO_OUTPUT:
        nrf_gpio_cfg_output(pin);
        break;
    case GPIO_INPUT_NOPULL:
        nrf_gpio_cfg_input(pin, NRF_GPIO_PIN_NOPULL);
        break;
    case GPIO_INPUT_PULLUP:
        nrf_gpio_cfg_input(pin, NRF_GPIO_PIN_PULLUP);
        break;
    case GPIO_INPUT_PULLDOWN:
        nrf_gpio_cfg_input(pin, NRF_GPIO_PIN_PULLDOWN);
        break;
    }
}

void gpio_write(uint8_t pin, uint8_t state)
{
    nrf_gpio_pin_write(pin, state);
}

uint8_t gpio_read(uint8_t pin)
{
    return nrf_gpio_pin_read(pin);
}

void gpio_watch(uint8_t pin, uint8_t change_direction)
{
    uint32_t sense;
    
    NRF_GPIOTE->EVENTS_PORT = 0;

    NRF_GPIOTE->INTENSET = GPIOTE_INTENSET_PORT_Msk;

    if (change_direction) {
        sense = GPIO_PIN_CNF_SENSE_High << GPIO_PIN_CNF_SENSE_Pos;
    } else {
        sense = GPIO_PIN_CNF_SENSE_Low << GPIO_PIN_CNF_SENSE_Pos;
    }
    
    NRF_GPIO->PIN_CNF[pin] &= ~GPIO_PIN_CNF_SENSE_Msk;
    NRF_GPIO->PIN_CNF[pin] |= sense;
}

void gpio_unwatch(uint8_t pin)
{
    NRF_GPIO->PIN_CNF[pin] &= ~GPIO_PIN_CNF_SENSE_Msk;
    NRF_GPIO->PIN_CNF[pin] |= GPIO_PIN_CNF_SENSE_Disabled << GPIO_PIN_CNF_SENSE_Pos;
            
    NRF_GPIOTE->INTENCLR = GPIOTE_INTENSET_PORT_Msk;
}

void gpio_on_change_task(void* args)
{
    uint32_t pins_state = (uint32_t) args;

    gpio_on_change(pins_state);
}

void GPIOTE_IRQHandler(void)
{
    uint32_t pins_state = NRF_GPIO->IN;

    

    //run_when_idle(gpio_on_change_task, (void*)pins_state);
  gpio_on_change_task((void *)pins_state);
  
  // Clear event.
    NRF_GPIOTE->EVENTS_PORT = 0;
}
