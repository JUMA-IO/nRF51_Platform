#ifndef _JUMA_COMMON_H_
#define _JUMA_COMMON_H_

#include "juma_sdk_app_interface.h"
#include "boardconfig.h"

#define JUMA_DATA_SERVICE_UUID         0x8000
#define JUMA_DATA_COMMAND_CHAR_UUID    0x8001
#define JUMA_DATA_EVENT_CHAR_UUID      0x8002
#define JUMA_DATA_BULKOUT_CHAR_UUID    0x8003
#define JUMA_DATA_BULKIN_CHAR_UUID     0x8004

#define JUMA_LOCK_SERVICE_UUID         0x8200
#define JUMA_LOCK_KEY_CHAR_UUID        0x8201
#define JUMA_LOCK_RAND_CHAR_UUID       0x8202
#define JUMA_LOCK_STATUS_CHAR_UUID     0x8203

extern user_information_config_t * UICR_board_config_information;
extern uint8_t juma_uuid_type;
void juma_init( void );

#endif
