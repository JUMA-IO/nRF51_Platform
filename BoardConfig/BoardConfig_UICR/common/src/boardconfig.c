#include "boardconfig.h"

const user_information_config_t user_information_config __attribute__((section("user_information_config"))) = {
	.FWID = 0xFFFF00,
	.BOOTLOADERADDR = 0x123456,
	.LFCLKSRC = 1,
};

