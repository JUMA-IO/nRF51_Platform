#include "nrf51.h"
#include "nrf_sdm.h"

typedef struct _board_config_versions_
{
  uint8_t value[5];
}board_config_versions_t;

typedef struct _board_config_lfclksrcs_
{
  enum NRF_CLOCK_LFCLKSRCS value;
}board_config_lfclksrcs_t;

typedef struct _board_config_t_
{
  board_config_versions_t       board_config_versions;
  board_config_lfclksrcs_t      board_config_lfclksrcs;
}board_config_t;







