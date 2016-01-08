#include <string.h>
#include "nordic_common.h"
#include "ble_l2cap.h"
#include "ble_srv_common.h"
#include "app_util.h"
#include "ble.h"

uint8_t juma_uuid_type;

void juma_init( void )
{
    const ble_uuid128_t base_uuid128 =
    {
        {
            0x8C, 0xF9, 0x97, 0xA6, 0xEE, 0x94, 0xE3, 0xBC,
            0xF8, 0x21, 0xB2, 0x60, 0x00, 0x00, 0x00, 0x00
        }
    };

    sd_ble_uuid_vs_add(&base_uuid128, &juma_uuid_type);
}
