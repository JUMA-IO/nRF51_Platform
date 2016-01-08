/* Copyright (c) 2014 junfenggeng@gmail.com. All Rights Reserved.
 *
 */

#include <stdio.h>
#include <string.h>
#include "nrf.h"
#include "juma_data_service.h"
#include "nordic_common.h"
#include "ble_l2cap.h"
#include "ble_srv_common.h"
#include "app_util.h"
#include "juma_common.h"

#define MAX_MESSAGE_LENGTH 200
#define HEADER_LENGTH 2

static uint8_t buffer[MAX_MESSAGE_LENGTH];
static uint16_t offset;

static void on_connect(juma_data_t * p_data, ble_evt_t * p_ble_evt)
{
    p_data->conn_handle = p_ble_evt->evt.gap_evt.conn_handle;
    offset = 0;
}

static void on_disconnect(juma_data_t * p_data, ble_evt_t * p_ble_evt)
{
    UNUSED_PARAMETER(p_ble_evt);
    p_data->conn_handle = BLE_CONN_HANDLE_INVALID;
}

static void on_command_value_write(juma_data_t * p_data, ble_gatts_evt_write_t * p_evt_write)
{
    uint8_t* d = p_evt_write->data;

    if ((p_evt_write->len > 0) && (p_data->evt_handler != NULL))
    {
        juma_data_evt_t evt;

        if (p_evt_write->len < 2) return;
        if (buffer[1] + HEADER_LENGTH > MAX_MESSAGE_LENGTH) return;
        
        offset = 0;
        memcpy(buffer, d, p_evt_write->len);
            offset += p_evt_write->len;

        if ((d[1] + HEADER_LENGTH) == p_evt_write->len) {
            evt.data = buffer;
            p_data->evt_handler(p_data, &evt);
        }
    }
}

static void on_bulk_value_write(juma_data_t * p_data, ble_gatts_evt_write_t * p_evt_write)
{
    uint8_t* d = p_evt_write->data;

    if ((p_evt_write->len > 0) && (p_data->evt_handler != NULL))
    {
        juma_data_evt_t evt;

        if (offset + p_evt_write->len <= HEADER_LENGTH + buffer[1]) {
            memcpy(buffer + offset, d, p_evt_write->len);
            offset += p_evt_write->len;

            if (offset == (HEADER_LENGTH + buffer[1])) {
                evt.data = buffer;
                p_data->evt_handler(p_data, &evt);
            }
        }
    }
}

/**@brief Function for handling the Write event.
 *
 * @param[in]   p_data      Data Service structure.
 * @param[in]   p_ble_evt   Event received from the BLE stack.
 */
static void on_write(juma_data_t * p_data, ble_evt_t * p_ble_evt)
{
    ble_gatts_evt_write_t * p_evt_write = &p_ble_evt->evt.gatts_evt.params.write;

    if (p_evt_write->handle == p_data->command_handles.value_handle) {
        on_command_value_write(p_data, p_evt_write);
    } else if (p_evt_write->handle == p_data->bulkout_handles.value_handle) {
        on_bulk_value_write(p_data, p_evt_write);
    }
}

static void on_tx_complete(juma_data_t * p_data, ble_evt_t * p_ble_evt)
{
}

void juma_data_on_ble_evt(juma_data_t * p_data, ble_evt_t * p_ble_evt)
{
    switch (p_ble_evt->header.evt_id)
    {
    case BLE_GAP_EVT_CONNECTED:
        on_connect(p_data, p_ble_evt);
        break;

    case BLE_GAP_EVT_DISCONNECTED:
        on_disconnect(p_data, p_ble_evt);
        break;

    case BLE_GATTS_EVT_WRITE:
        on_write(p_data, p_ble_evt);
        break;

    case BLE_EVT_TX_COMPLETE:
        on_tx_complete(p_data, p_ble_evt);
        break;

    default:
        // No implementation needed.
        break;
    }
}

static uint32_t command_char_add(juma_data_t            * p_data,
                          const juma_data_init_t * p_data_init)
{
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          char_uuid;
    ble_gatts_attr_md_t attr_md;

    memset(&char_md, 0, sizeof(char_md));

    char_md.char_props.write = 1;
    char_md.p_char_user_desc  = NULL;
    char_md.p_char_pf         = NULL;
    char_md.p_user_desc_md    = NULL;
    char_md.p_cccd_md         = NULL;
    char_md.p_sccd_md         = NULL;

    char_uuid.type = juma_uuid_type;
    char_uuid.uuid = JUMA_DATA_COMMAND_CHAR_UUID;

    memset(&attr_md, 0, sizeof(attr_md));

    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);

    attr_md.vloc       = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth    = 0;
    attr_md.wr_auth    = 0;
    attr_md.vlen       = 1;

    memset(&attr_char_value, 0, sizeof(attr_char_value));

    attr_char_value.p_uuid    = &char_uuid;
    attr_char_value.p_attr_md = &attr_md;
    attr_char_value.init_len  = 0;
    attr_char_value.init_offs = 0;
    attr_char_value.max_len   = BLE_L2CAP_MTU_DEF;
    attr_char_value.p_value   = NULL;

    return sd_ble_gatts_characteristic_add(p_data->service_handle,
                                           &char_md,
                                           &attr_char_value,
                                           &p_data->command_handles);
}

static uint32_t bulkout_char_add(juma_data_t            * p_data,
                          const juma_data_init_t * p_data_init)
{
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          char_uuid;
    ble_gatts_attr_md_t attr_md;

    memset(&char_md, 0, sizeof(char_md));

    char_md.char_props.write_wo_resp = 1;
    char_md.p_char_user_desc  = NULL;
    char_md.p_char_pf         = NULL;
    char_md.p_user_desc_md    = NULL;
    char_md.p_cccd_md         = NULL;
    char_md.p_sccd_md         = NULL;

    char_uuid.type = juma_uuid_type;
    char_uuid.uuid = JUMA_DATA_BULKOUT_CHAR_UUID;

    memset(&attr_md, 0, sizeof(attr_md));

    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);

    attr_md.vloc       = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth    = 0;
    attr_md.wr_auth    = 0;
    attr_md.vlen       = 1;

    memset(&attr_char_value, 0, sizeof(attr_char_value));

    attr_char_value.p_uuid    = &char_uuid;
    attr_char_value.p_attr_md = &attr_md;
    attr_char_value.init_len  = 0;
    attr_char_value.init_offs = 0;
    attr_char_value.max_len   = BLE_L2CAP_MTU_DEF;
    attr_char_value.p_value   = NULL;

    return sd_ble_gatts_characteristic_add(p_data->service_handle,
                                           &char_md,
                                           &attr_char_value,
                                           &p_data->bulkout_handles);
}

static uint32_t event_char_add(juma_data_t            * p_data,
                                const juma_data_init_t * p_data_init)
{
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          char_uuid;
    ble_gatts_attr_md_t attr_md;

    memset(&char_md, 0, sizeof(char_md));

    char_md.char_props.notify = 1;
    char_md.p_char_user_desc  = NULL;
    char_md.p_char_pf         = NULL;
    char_md.p_user_desc_md    = NULL;
    char_md.p_cccd_md         = NULL;
    char_md.p_sccd_md         = NULL;

    char_uuid.type = juma_uuid_type;
    char_uuid.uuid = JUMA_DATA_EVENT_CHAR_UUID;

    memset(&attr_md, 0, sizeof(attr_md));

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&attr_md.write_perm);
    attr_md.vloc       = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth    = 0;
    attr_md.wr_auth    = 0;
    attr_md.vlen       = 1;

    memset(&attr_char_value, 0, sizeof(attr_char_value));

    attr_char_value.p_uuid    = &char_uuid;
    attr_char_value.p_attr_md = &attr_md;
    attr_char_value.init_len  = 0;
    attr_char_value.init_offs = 0;
    attr_char_value.max_len   = BLE_L2CAP_MTU_DEF;
    attr_char_value.p_value   = NULL;

    return sd_ble_gatts_characteristic_add(p_data->service_handle,
                                           &char_md,
                                           &attr_char_value,
                                           &p_data->event_handles);
}

static uint32_t bulkin_char_add(juma_data_t            * p_data,
                                const juma_data_init_t * p_data_init)
{
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          char_uuid;
    ble_gatts_attr_md_t attr_md;

    memset(&char_md, 0, sizeof(char_md));

    char_md.char_props.notify = 1;
    char_md.p_char_user_desc  = NULL;
    char_md.p_char_pf         = NULL;
    char_md.p_user_desc_md    = NULL;
    char_md.p_cccd_md         = NULL;
    char_md.p_sccd_md         = NULL;

    char_uuid.type = juma_uuid_type;
    char_uuid.uuid = JUMA_DATA_BULKIN_CHAR_UUID;

    memset(&attr_md, 0, sizeof(attr_md));

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&attr_md.write_perm);
    attr_md.vloc       = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth    = 0;
    attr_md.wr_auth    = 0;
    attr_md.vlen       = 1;

    memset(&attr_char_value, 0, sizeof(attr_char_value));

    attr_char_value.p_uuid    = &char_uuid;
    attr_char_value.p_attr_md = &attr_md;
    attr_char_value.init_len  = 0;
    attr_char_value.init_offs = 0;
    attr_char_value.max_len   = BLE_L2CAP_MTU_DEF;
    attr_char_value.p_value   = NULL;

    return sd_ble_gatts_characteristic_add(p_data->service_handle,
                                           &char_md,
                                           &attr_char_value,
                                           &p_data->bulkin_handles);
}

uint32_t juma_data_init(juma_data_t * p_data, const juma_data_init_t * p_data_init)
{
    uint32_t   err_code;
    ble_uuid_t service_uuid;

    service_uuid.uuid = JUMA_DATA_SERVICE_UUID;
    service_uuid.type = juma_uuid_type;

    // Initialize service structure
    p_data->evt_handler                 = p_data_init->evt_handler;
    p_data->conn_handle                 = BLE_CONN_HANDLE_INVALID;

    // Add Service
    err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY,
                                        &service_uuid,
                                        &p_data->service_handle);

    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }

    // Add command characteristic
    err_code = command_char_add(p_data, p_data_init);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }

    // Add bulkout characteristic
    err_code = bulkout_char_add(p_data, p_data_init);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }

    // Add event characteristic
    err_code = event_char_add(p_data, p_data_init);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }

    // Add bulkin characteristic
    err_code = bulkin_char_add(p_data, p_data_init);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }

    return NRF_SUCCESS;
}

uint32_t juma_event_send(juma_data_t * p_data, uint8_t* packet, uint8_t length)
{
    uint32_t err_code;

    // Send value if connected and notifying
    if (p_data->conn_handle != BLE_CONN_HANDLE_INVALID)
    {
        uint16_t               hvx_len;
        ble_gatts_hvx_params_t hvx_params;

        hvx_len = length;

        memset(&hvx_params, 0, sizeof(hvx_params));

        hvx_params.handle = p_data->event_handles.value_handle;
        hvx_params.type   = BLE_GATT_HVX_NOTIFICATION;
        hvx_params.offset = 0;
        hvx_params.p_len  = &hvx_len;
        hvx_params.p_data = packet;

        err_code = sd_ble_gatts_hvx(p_data->conn_handle, &hvx_params);
        if ((err_code == NRF_SUCCESS) && (hvx_len != length))
        {
            err_code = NRF_ERROR_DATA_SIZE;
        }
    }
    else
    {
        err_code = NRF_ERROR_INVALID_STATE;
    }

    return err_code;
}

uint32_t juma_bulkin_send(juma_data_t * p_data, uint8_t* packet, uint8_t length)
{
    uint32_t err_code;

    // Send value if connected and notifying
    if (p_data->conn_handle != BLE_CONN_HANDLE_INVALID)
    {
        uint16_t               hvx_len;
        ble_gatts_hvx_params_t hvx_params;

        hvx_len = length;

        memset(&hvx_params, 0, sizeof(hvx_params));

        hvx_params.handle = p_data->bulkin_handles.value_handle;
        hvx_params.type   = BLE_GATT_HVX_NOTIFICATION;
        hvx_params.offset = 0;
        hvx_params.p_len  = &hvx_len;
        hvx_params.p_data = packet;

        err_code = sd_ble_gatts_hvx(p_data->conn_handle, &hvx_params);
        if ((err_code == NRF_SUCCESS) && (hvx_len != length))
        {
            err_code = NRF_ERROR_DATA_SIZE;
        }
    }
    else
    {
        err_code = NRF_ERROR_INVALID_STATE;
    }

    return err_code;
}

