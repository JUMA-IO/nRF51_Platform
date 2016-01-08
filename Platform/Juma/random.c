#include "sys_common.h"


uint8_t random_generate()
{
	uint8_t value = 0;
	
	NRF_RNG->CONFIG = RNG_CONFIG_DERCEN_Enabled << RNG_CONFIG_DERCEN_Pos;
	NRF_RNG->SHORTS = RNG_SHORTS_VALRDY_STOP_Enabled << RNG_SHORTS_VALRDY_STOP_Pos;
	NRF_RNG->EVENTS_VALRDY = 0;
	
	NRF_RNG->TASKS_START = 1;
	
	while(NRF_RNG->EVENTS_VALRDY == 0);
	value = NRF_RNG->VALUE;
	
	NRF_RNG->TASKS_STOP = 1;
	NRF_RNG->CONFIG = RNG_CONFIG_DERCEN_Disabled << RNG_CONFIG_DERCEN_Pos;
	
	return value;
}




