#include "boardconfig.h"

const user_information_config_t user_information_config __attribute__((section("user_information_config"))) = {
	.CLENR0							   	= 0xFFFFFFFFUL,
	.PBPCONF							 	= 0xFFFFFFFFUL,
	.XTALFREQ							 	= 0xFFFFFFFFFFFFFFFFUL,
	.FWID									 	= 0xFFFFFFFFUL,
	.BOOTLOADERADDR				 	= 0xFFFFFFFFUL,
	/******************/
	.NRFFW	  						 	= {0xFFFFFFFFUL, 0xFFFFFFFFUL},
	.NRFHW								 	= {0xFFFFFFFFUL},
	/******************/
	.LFCLKSRC_EN						= BOARD_CONFIG_VALUE_DISABLE,
	.LFCLKSRC							 	= 0xFFFFFFFFUL,
	.CUSTOMER	  					 	= 0xFFFFFFFFUL,
};

