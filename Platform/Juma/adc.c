#include "sys_common.h"

static function_t _on_complete = NULL;
static adc_result_t result;

void ADC_IRQHandler(void)
{
    if (NRF_ADC->EVENTS_END != 0)
    {
        uint32_t     raw_value;

        NRF_ADC->EVENTS_END     = 0;
        raw_value              = NRF_ADC->RESULT;
        NRF_ADC->TASKS_STOP     = 1;

        result.value = raw_value;

        run_when_idle(_on_complete, (void*)&result);
        _on_complete = NULL;
    }
}

static void start_adc(void)
{
    uint32_t err_code;

    err_code = sd_nvic_ClearPendingIRQ(ADC_IRQn);
    APP_ERROR_CHECK(err_code);

    err_code = sd_nvic_SetPriority(ADC_IRQn, NRF_APP_PRIORITY_LOW);
    APP_ERROR_CHECK(err_code);

    err_code = sd_nvic_EnableIRQ(ADC_IRQn);
    APP_ERROR_CHECK(err_code);

    NRF_ADC->EVENTS_END = 0;
    NRF_ADC->TASKS_START = 1;
}

void vcc_measure(function_t on_complete)
{
    if (_on_complete) return;
    
    _on_complete = on_complete;

    NRF_ADC->INTENSET   = ADC_INTENSET_END_Msk;
    NRF_ADC->CONFIG     = (ADC_CONFIG_RES_8bit                        << ADC_CONFIG_RES_Pos)     |
        (ADC_CONFIG_INPSEL_SupplyOneThirdPrescaling << ADC_CONFIG_INPSEL_Pos)  |
        (ADC_CONFIG_REFSEL_VBG                      << ADC_CONFIG_REFSEL_Pos)  |
        (ADC_CONFIG_PSEL_Disabled                   << ADC_CONFIG_PSEL_Pos)    |
        (ADC_CONFIG_EXTREFSEL_None                  << ADC_CONFIG_EXTREFSEL_Pos);
    NRF_ADC->EVENTS_END = 0;
    NRF_ADC->ENABLE     = ADC_ENABLE_ENABLE_Enabled;

    start_adc();
}

void adc_measure(uint8_t pin, uint8_t resolution, function_t on_complete)
{
    uint32_t psel;
    uint32_t res;

    if (_on_complete) return;
    
    _on_complete = on_complete;
    
    switch (pin) {
    case 26:
        psel = ADC_CONFIG_PSEL_AnalogInput0;
        break;
    case 27:
        psel = ADC_CONFIG_PSEL_AnalogInput1;
        break;
    case 1:
        psel = ADC_CONFIG_PSEL_AnalogInput2;
        break;
    case 2:
        psel = ADC_CONFIG_PSEL_AnalogInput3;
        break;
    case 3:
        psel = ADC_CONFIG_PSEL_AnalogInput4;
        break;
    case 4:
        psel = ADC_CONFIG_PSEL_AnalogInput5;
        break;
    case 5:
        psel = ADC_CONFIG_PSEL_AnalogInput6;
        break;
    case 6:
        psel = ADC_CONFIG_PSEL_AnalogInput7;
        break;
    }

    switch (resolution) {
    case 8:
        res = ADC_CONFIG_RES_8bit;
        break;
    case 9:
        res = ADC_CONFIG_RES_9bit;
        break;
    default:
        res = ADC_CONFIG_RES_10bit;
        break;
    }

    // Configure ADC
    NRF_ADC->INTENSET = ADC_INTENSET_END_Msk;
    NRF_ADC->CONFIG = (res << ADC_CONFIG_RES_Pos) |
        (ADC_CONFIG_INPSEL_AnalogInputOneThirdPrescaling << ADC_CONFIG_INPSEL_Pos) |
        (ADC_CONFIG_REFSEL_VBG << ADC_CONFIG_REFSEL_Pos) |
        (psel << ADC_CONFIG_PSEL_Pos) |
        (ADC_CONFIG_EXTREFSEL_None << ADC_CONFIG_EXTREFSEL_Pos);
    NRF_ADC->EVENTS_END = 0;
    NRF_ADC->ENABLE = ADC_ENABLE_ENABLE_Enabled;

    start_adc();
}
