#include "boardconfig.h"


const board_config_t board_config_inf __attribute__((section("board_config"))) = {
  .board_config_versions.value = {0x00, 0x01, 0x00, 0x00},
  .board_config_lfclksrcs.value = NRF_CLOCK_LFCLKSRC_RC_250_PPM_TEMP_16000MS_CALIBRATION,
};

