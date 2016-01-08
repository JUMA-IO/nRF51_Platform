/* Copyright (c) 2014-2015 JUMA Technologies. All Rights Reserved.
 *
 */

#ifndef JUMA_DATA_H__
#define JUMA_DATA_H__

#include <stdint.h>
#include <stdbool.h>
#include "ble.h"
#include "ble_srv_common.h"

/**@brief Data Service event. */
typedef struct
{
    uint8_t* data;
} juma_data_evt_t;

// Forward declaration of the juma_data_t type. 
typedef struct juma_data_s juma_data_t;

/**@brief Data Service event handler type. */
typedef void (*juma_data_evt_handler_t) (juma_data_t * p_data, juma_data_evt_t * p_evt);

/**@brief Data Service init structure. This contains all options and data needed for
 *        initialization of the service. */
typedef struct
{
    juma_data_evt_handler_t        evt_handler;                                          /**< DATA Event handler */
} juma_data_init_t;

/**@brief Data Service structure. This contains various status information for the service. */
typedef struct juma_data_s
{	
    juma_data_evt_handler_t      evt_handler;
    uint16_t                     service_handle;
    ble_gatts_char_handles_t     command_handles;
    ble_gatts_char_handles_t     event_handles;
    ble_gatts_char_handles_t     bulkout_handles;
    ble_gatts_char_handles_t     bulkin_handles;
    uint16_t                     conn_handle;
    uint8_t                      flags;
} juma_data_t;

/**@brief Function for initializing the DATA Service.
 *
 * @param[out]  p_data       DATA Service structure. This structure will have to be supplied by
 *                          the application. It will be initialized by this function, and will later
 *                          be used to identify this particular service instance.
 * @param[in]   p_data_init  Information needed to initialize the service.
 *
 * @return      NRF_SUCCESS on successful initialization of service, otherwise an error code.
 */
uint32_t juma_data_init(juma_data_t * p_data, const juma_data_init_t * p_data_init);

/**@brief Function for handling the Application's BLE Stack events.
 *
 * @details Handles all events from the BLE stack of interest to the DATA Service.
 *
 * @param[in]   p_data      DATA Service structure.
 * @param[in]   p_ble_evt  Event received from the BLE stack.
 */
void juma_data_on_ble_evt(juma_data_t * p_data, ble_evt_t * p_ble_evt);

uint32_t juma_event_send(juma_data_t * p_data, uint8_t* packet, uint8_t length);
uint32_t juma_bulk_send(juma_data_t * p_data, uint8_t* packet, uint8_t length);

#endif // JUMA_DATA_H__

/** @} */
