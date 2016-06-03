/* Copyright (c) 2014 junfenggeng@gmail.com. All Rights Reserved.
 */

#include <stdint.h>
#include <string.h>
#include "nordic_common.h"
#include "nrf.h"
#include "app_error.h"
#include "nrf51_bitfields.h"
#include "ble.h"
#include "ble_hci.h"
#include "ble_srv_common.h"
#include "ble_advdata.h"
#include "softdevice_handler.h"
#include "nrf_gpio.h"
#include "nrf_delay.h"
#include "device_manager.h"
#include "pstorage.h"
#include "app_util_platform.h"
#include "juma_common.h"
#include "juma_data_service.h"
#include "dispatch.h"
#include "sys_common.h"
#include "simple_uart.h"
#include "device_statue.h"
#include "boardconfig.h"



#define IS_SRVC_CHANGED_CHARACT_PRESENT      0
#define APP_ADV_INTERVAL                     200 //125ms

#define APP_BEACON_INFO_LENGTH          0x17                              /**< Total length of information advertised by the Beacon. */
#define APP_COMPANY_IDENTIFIER          0x004C                            /**< Company identifier for Apple */
#define DEFAULT_MEASURED_RSSI           0xC3

uint8_t default_beacon_uuid[16]={0x8C, 0xF9, 0x97, 0xA6,
                                 0xEE, 0x94, 0xE3, 0xBC,
                                 0xF8, 0x21, 0xB2, 0x60,
                                 0x4A, 0x55, 0x4D, 0x41};

#define MIN_CONN_INTERVAL                    24                                         /**< Minimum acceptable connection interval (30 ms). */
#define MAX_CONN_INTERVAL                    48                                        /**< Maximum acceptable connection interval (60ms). */
#define SLAVE_LATENCY                        0                                          /**< Slave latency. */
#define CONN_SUP_TIMEOUT                     MSEC_TO_UNITS(4000, UNIT_10_MS)            /**< Connection supervisory timeout (4 seconds). */

#define SEC_PARAM_TIMEOUT                    30                                         /**< Timeout for Pairing Request or Security Request (in seconds). */
#define SEC_PARAM_BOND                       1                                          /**< Perform bonding. */
#define SEC_PARAM_MITM                       0                                          /**< Man In The Middle protection not required. */
#define SEC_PARAM_IO_CAPABILITIES            BLE_GAP_IO_CAPS_NONE                       /**< No I/O capabilities. */
#define SEC_PARAM_OOB                        0                                          /**< Out Of Band data not available. */
#define SEC_PARAM_MIN_KEY_SIZE               7                                          /**< Minimum encryption key size. */
#define SEC_PARAM_MAX_KEY_SIZE               16                                         /**< Maximum encryption key size. */

#define DEAD_BEEF                            0xDEADBEEF                                 /**< Value used as error code on stack dump, can be used to identify stack location on stack unwind. */

juma_data_t m_juma_data_service;

static ble_gap_adv_params_t                  m_adv_params;                              /**< Parameters to be passed to the stack when starting advertising. */
static bool                                  m_memory_access_in_progress = false;       /**< Flag to keep track of ongoing operations on persistent memory. */
static dm_application_instance_t             m_app_handle;                              /**< Application identifier allocated by device manager */
static uint16_t                              m_conn_handle = BLE_CONN_HANDLE_INVALID;

#define BEACON_UUID_OFFSET              2
#define BEACON_MAJOR_OFFSET             18
#define BEACON_MINOR_OFFSET             20
#define MEASURED_RSSI_OFFSET            22


user_information_config_t * UICR_board_config_information = (user_information_config_t *)NRF_UICR_BASE;
uint8_t device_id[16];
static const char* device_name = "JUMA DEVICE";
static ble_gap_addr_t base_addr;

static const uint8_t default_key[16] = {0x1c, 0x0e, 0xd7, 0x73,
                                0xa2, 0xac, 0x44, 0x05,
                                0x95, 0x15, 0xad, 0xfa,
                                0x7f, 0xe3, 0xcf, 0x45};

static void ble_evt_dispatch(ble_evt_t * p_ble_evt);
static void sys_evt_dispatch(uint32_t sys_evt);

void ble_device_get_id(uint8_t* id, uint8_t len)
{
    if (len > 16) len = 16;

    memcpy(id, device_id, len);
}

void ble_device_set_name(const char* new_device_name)
{
    uint32_t err_code;
    ble_gap_conn_sec_mode_t sec_mode;
    ble_advdata_t advdata;
  
    ble_uuid_t juma_service_uuid;
   
    device_name = new_device_name;
    
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&sec_mode);
 
    err_code = sd_ble_gap_device_name_set(&sec_mode,
                                          (const uint8_t *)device_name,
                                          strlen(device_name));
    APP_ERROR_CHECK(err_code);

    // Build and set scan response data.
    memset(&advdata, 0, sizeof(advdata));

    advdata.flags              = BLE_GAP_ADV_FLAG_BR_EDR_NOT_SUPPORTED;
    advdata.name_type               = BLE_ADVDATA_FULL_NAME;

    advdata.uuids_complete.uuid_cnt = 1;
                                          
    juma_service_uuid.type = BLE_UUID_TYPE_BLE;
    juma_service_uuid.uuid = 0xFE90;
                                                                                  
    advdata.uuids_complete.p_uuids = &juma_service_uuid;
                                                                             
    err_code = ble_advdata_set(&advdata, NULL);
    APP_ERROR_CHECK(err_code);
}

void app_error_handler(uint32_t error_code, uint32_t line_num, const uint8_t * p_file_name)
{
    nrf_delay_ms(100);
    
    NVIC_SystemReset();
}


// Callback function for asserts in the SoftDevice.
void assert_nrf_callback(uint16_t line_num, const uint8_t * p_file_name)
{
    app_error_handler(DEAD_BEEF, line_num, p_file_name);
}


static void gap_params_init(void)
{
    uint32_t                err_code;
    ble_gap_conn_params_t   gap_conn_params;
    
    err_code = sd_ble_gap_appearance_set(BLE_APPEARANCE_UNKNOWN);
    APP_ERROR_CHECK(err_code);

    memset(&gap_conn_params, 0, sizeof(gap_conn_params));

    gap_conn_params.min_conn_interval = MIN_CONN_INTERVAL;
    gap_conn_params.max_conn_interval = MAX_CONN_INTERVAL;
    gap_conn_params.slave_latency     = SLAVE_LATENCY;
    gap_conn_params.conn_sup_timeout  = CONN_SUP_TIMEOUT;

    err_code = sd_ble_gap_ppcp_set(&gap_conn_params);
    APP_ERROR_CHECK(err_code);
}

#if 0
void beacon_setup(uint8_t* uuid, uint16_t major, uint16_t minor, uint8_t rssi)
{
	uint32_t      err_code;
	ble_advdata_t advdata, scandata;
	ble_advdata_manuf_data_t manuf_specific_data;
	uint8_t beacon_info[APP_BEACON_INFO_LENGTH];

	beacon_info[0] = 0x02; // device type: beacon
	beacon_info[1] = 0x15; // adv data length
	memcpy(&beacon_info[BEACON_UUID_OFFSET], uuid, 16);
	beacon_info[BEACON_MAJOR_OFFSET] = major >> 8;
	beacon_info[BEACON_MAJOR_OFFSET + 1] = major;
	beacon_info[BEACON_MINOR_OFFSET] = minor >> 8;
	beacon_info[BEACON_MINOR_OFFSET + 1] = minor;
	beacon_info[MEASURED_RSSI_OFFSET] = rssi;

	manuf_specific_data.company_identifier = APP_COMPANY_IDENTIFIER;
	manuf_specific_data.data.p_data = (uint8_t *) beacon_info;
	manuf_specific_data.data.size   = APP_BEACON_INFO_LENGTH;

	// Build and set advertising data.
	memset(&advdata, 0, sizeof(advdata));

	advdata.name_type             = BLE_ADVDATA_NO_NAME;
	advdata.flags              = BLE_GAP_ADV_FLAG_BR_EDR_NOT_SUPPORTED;
	advdata.p_manuf_specific_data = &manuf_specific_data;

	// Build and set scan response data.
	memset(&scandata, 0, sizeof(scandata));

	ble_uuid_t adv_uuids[] =
	{
			{0xFFE3,         BLE_UUID_TYPE_BLE},
	};

	scandata.uuids_complete.uuid_cnt = sizeof(adv_uuids) / sizeof(adv_uuids[0]);
	scandata.uuids_complete.p_uuids  = adv_uuids;
	scandata.name_type               = BLE_ADVDATA_FULL_NAME;

	err_code = ble_advdata_set(&advdata, &scandata);
	APP_ERROR_CHECK(err_code);
}
#endif

static void advertising_init(void)
{
#if 0
    beacon_setup(default_beacon_uuid,
                 (device_id[0] << 8) | device_id[1],
                 (device_id[2] << 8) | device_id[3],
                 DEFAULT_MEASURED_RSSI);
#endif

    // Initialize advertising parameters (used when starting advertising).
    memset(&m_adv_params, 0, sizeof(m_adv_params));

    m_adv_params.type        = BLE_GAP_ADV_TYPE_ADV_IND;
    m_adv_params.p_peer_addr = NULL;                           // Undirected advertisement.
    m_adv_params.fp          = BLE_GAP_ADV_FP_ANY;
    m_adv_params.interval    = APP_ADV_INTERVAL;
    m_adv_params.timeout     = 0;
}

void juma_data_evt_handler(juma_data_t * p_data, juma_data_evt_t * p_evt)
{
    if (p_evt->data[0] < 0x80) {
        ble_device_on_message(p_evt->data[0], p_evt->data[1], p_evt->data + 2);
    } 
    else 
    {
        system_on_message(p_evt->data[0], p_evt->data[1], p_evt->data + 2);
    }
}

static void services_init(void)
{
    juma_data_init_t data_init;
    uint32_t err_code;
    
    memset(&data_init, 0, sizeof(data_init));

    data_init.evt_handler = juma_data_evt_handler;

    err_code = juma_data_init(&m_juma_data_service, &data_init);
    APP_ERROR_CHECK(err_code);
}


static uint32_t device_manager_evt_handler(dm_handle_t const    * p_handle,
                                           dm_event_t const     * p_event,
                                           ret_code_t           event_result)
{
    APP_ERROR_CHECK(event_result);
    return NRF_SUCCESS;
}


static void device_manager_init(void)
{
  uint32_t                err_code;
  dm_init_param_t         init_data;
  dm_application_param_t  register_param;
  
  // Initialize persistent storage module.
  err_code = pstorage_init();
  APP_ERROR_CHECK(err_code);

  // Clear all bonded centrals if the Bonds Delete button is pushed.
  init_data.clear_persistent_data = 0;

  err_code = dm_init(&init_data);
  APP_ERROR_CHECK(err_code);

  memset(&register_param.sec_param, 0, sizeof(ble_gap_sec_params_t));

  register_param.sec_param.bond         = SEC_PARAM_BOND;
  register_param.sec_param.mitm         = SEC_PARAM_MITM;
  register_param.sec_param.io_caps      = SEC_PARAM_IO_CAPABILITIES;
  register_param.sec_param.oob          = SEC_PARAM_OOB;
  register_param.sec_param.min_key_size = SEC_PARAM_MIN_KEY_SIZE;
  register_param.sec_param.max_key_size = SEC_PARAM_MAX_KEY_SIZE;
  register_param.evt_handler            = device_manager_evt_handler;
  register_param.service_type           = DM_PROTOCOL_CNTXT_GATT_SRVR_ID;

  err_code = dm_register(&m_app_handle, &register_param);
  APP_ERROR_CHECK(err_code);
}

static void hash128(uint8_t* input, uint8_t len, uint8_t* output)
{
  nrf_ecb_hal_data_t ecb_data;

  memcpy(ecb_data.key, default_key, 16);
  if (len>16) len=16;
  memcpy(ecb_data.cleartext, input, len);

  sd_ecb_block_encrypt(&ecb_data);
  memcpy(output, ecb_data.ciphertext, 16);
}

static void ble_stack_init(void)
{
  uint32_t err_code;
  ble_gap_conn_sec_mode_t sec_mode;
  uint8_t* tstr;
  uint8_t cksum;
  int i;
  
  // Initialize the SoftDevice handler module.
	if(BOARD_CONFIG_VALUE_ENABLE == UICR_board_config_information->LFCLKSRC_EN)
	{//user config
		SOFTDEVICE_HANDLER_INIT(UICR_board_config_information->LFCLKSRC, false);
	}
	else
	{//default
		SOFTDEVICE_HANDLER_INIT(NRF_CLOCK_LFCLKSRC_RC_250_PPM_TEMP_16000MS_CALIBRATION, false);
	}
	
  // Enable BLE stack 
  ble_enable_params_t ble_enable_params;
  memset(&ble_enable_params, 0, sizeof(ble_enable_params));
  ble_enable_params.gatts_enable_params.service_changed = IS_SRVC_CHANGED_CHARACT_PRESENT;
  ble_enable_params.gatts_enable_params.attr_tab_size = 512;
  err_code = sd_ble_enable(&ble_enable_params);
  APP_ERROR_CHECK(err_code);

  // Register with the SoftDevice handler module for BLE events.
  err_code = softdevice_ble_evt_handler_set(ble_evt_dispatch);
  APP_ERROR_CHECK(err_code);
  
  // Register with the SoftDevice handler module for BLE events.
  err_code = softdevice_sys_evt_handler_set(sys_evt_dispatch);
  APP_ERROR_CHECK(err_code);

  sd_ble_gap_address_get(&base_addr);
  hash128(base_addr.addr, 6, device_id);

  base_addr.addr_type = BLE_GAP_ADDR_TYPE_RANDOM_STATIC;

  tstr = __TIME__;
  cksum = 0;
  for (i=0; i<strlen(__TIME__); i++) {
      cksum += tstr[i];
  }

  base_addr.addr[0] += cksum;
  sd_ble_gap_address_set(BLE_GAP_ADDR_CYCLE_MODE_NONE, &base_addr);

  BLE_GAP_CONN_SEC_MODE_SET_OPEN(&sec_mode);

  err_code = sd_ble_gap_device_name_set(&sec_mode,
                                        (const uint8_t *)device_name,
                                        strlen(device_name));
  APP_ERROR_CHECK(err_code);
}

void ble_device_select_address(uint8_t id)
{
  ble_gap_addr_t   addr;

  memcpy(&addr, &base_addr, sizeof(addr));

  addr.addr[0] += id;
  sd_ble_gap_address_set(BLE_GAP_ADDR_CYCLE_MODE_NONE, &addr);
}

/*****************************************************************************
 * Static Start Functions
 *****************************************************************************/

/**@brief Function for starting advertising.
 */
void ble_device_start_advertising(void)
{
  uint32_t err_code;

  err_code = sd_ble_gap_adv_start(&m_adv_params);
  APP_ERROR_CHECK(err_code);
}

void ble_device_stop_advertising(void)
{
  uint32_t err_code;

  err_code = sd_ble_gap_adv_stop();
  APP_ERROR_CHECK(err_code);
}

void ble_device_set_advertising_interval(uint16_t interval)
{
  m_adv_params.interval = interval;
}

/**@brief Function for putting the chip in System OFF Mode
 */
static void system_off_mode_enter(void)
{
  uint32_t err_code;
  uint32_t count;
  
  // Verify if there is any flash access pending, if yes delay starting advertising until 
  // it's complete.
  err_code = pstorage_access_status_get(&count);
  APP_ERROR_CHECK(err_code);
  
  if (count != 0)
  {
      m_memory_access_in_progress = true;
      return;
  }

  err_code = sd_power_system_off();
  APP_ERROR_CHECK(err_code);
}

void dfu_reset_prepare(void)
{
  uint32_t err_code;
  
  if (m_conn_handle != BLE_CONN_HANDLE_INVALID)
  {
      err_code = sd_ble_gap_disconnect(m_conn_handle, BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
      APP_ERROR_CHECK(err_code);
  }
  else
  {
      err_code = sd_ble_gap_adv_stop();
      APP_ERROR_CHECK(err_code);
  }
}

uint8_t ble_device_is_connected( void )
{
  return (m_conn_handle != BLE_CONN_HANDLE_INVALID);
}

void ble_device_disconnect( void )
{
  uint32_t err_code;

  if (m_conn_handle != BLE_CONN_HANDLE_INVALID)
  {
      err_code = sd_ble_gap_disconnect(m_conn_handle, BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
      APP_ERROR_CHECK(err_code);
  }
}

/**@brief Set the radio's transmit power.
 *
 * @param[in] tx_power Radio transmit power in dBm (accepted values are -40, -30, -20, -16, -12, -8, -4, 0, and 4 dBm).
 *
 * @note -40 dBm will not actually give -40 dBm, but will instead be remapped to -30 dBm.
*/
void ble_device_set_tx_power( int8_t tx_power)
{
	uint32_t err_code;
	
	err_code = sd_ble_gap_tx_power_set(tx_power);
	APP_ERROR_CHECK(err_code);
	
}

void _ble_device_on_connect(void * args)
{

}

void _ble_device_on_disconnect(void * args)
{
  
}

void _ble_device_on_message(void * args)
{

}

/*****************************************************************************
 * Static Event Handling Functions
 *****************************************************************************/

/**@brief Function for handling the Application's BLE Stack events.
 *
 * @param[in]   p_ble_evt   Bluetooth stack event.
 */
static void on_ble_evt(ble_evt_t * p_ble_evt)
{
  switch (p_ble_evt->header.evt_id)
  {
  case BLE_GAP_EVT_CONNECTED:
      m_conn_handle = p_ble_evt->evt.gap_evt.conn_handle;
      ble_device_on_connect();
      break;

  case BLE_GAP_EVT_DISCONNECTED:
  {
      uint8_t reason = p_ble_evt->evt.gap_evt.params.disconnected.reason;

      m_conn_handle = BLE_CONN_HANDLE_INVALID;
      
  if (reason == BLE_HCI_CONNECTION_TIMEOUT ) {
          ble_device_on_disconnect(BLE_DEVICE_LINK_LOST);
  } else {
          ble_device_on_disconnect(BLE_DEVICE_NORMAL);
      }

      if (app == NULL) {
          ble_device_start_advertising();
      }
      
      break;
  }
  case BLE_GAP_EVT_TIMEOUT:
      break;

  default:
      // No implementation needed.
      break;
  }
}


/**@brief Function for handling the Application's system events.
 *
 * @param[in]   sys_evt   system event.
 */
static void on_sys_evt(uint32_t sys_evt)
{
	switch(sys_evt)
	{
	case NRF_EVT_FLASH_OPERATION_SUCCESS:
	case NRF_EVT_FLASH_OPERATION_ERROR:
			if (m_memory_access_in_progress)
			{
					m_memory_access_in_progress = false;
					system_off_mode_enter();
			}
			break;
	default:
			// No implementation needed.
			break;
	}
}


/**@brief Function for dispatching a BLE stack event to all modules with a BLE stack event handler.
 *
 * @details This function is called from the BLE Stack event interrupt handler after a BLE stack
 *          event has been received.
 *
 * @param[in]   p_ble_evt   Bluetooth stack event.
 */
static void ble_evt_dispatch(ble_evt_t * p_ble_evt)
{
	dm_ble_evt_handler(p_ble_evt);
	juma_data_on_ble_evt(&m_juma_data_service, p_ble_evt);
	on_ble_evt(p_ble_evt);
}


/**@brief Function for dispatching a system event to interested modules.
 *
 * @details This function is called from the System event interrupt handler after a system
 *          event has been received.
 *
 * @param[in]   sys_evt   System stack event.
 */
static void sys_evt_dispatch(uint32_t sys_evt)
{
    pstorage_sys_event_handler(sys_evt);
    on_sys_evt(sys_evt);
}

void load_app(void);

void serial_get(void);

void watchDog_sys_SDK_dog1_RR(void);

//ota status
extern uint8_t enter_ota_mode_statue;
void enter_ota_process(void * args);
/*****************************************************************************
 * Main Function
 *****************************************************************************/

/**@brief Function for the application main entry.
 */
int main(void)
{
  uint32_t err_code;

  load_app();
  gpio_init();
  ble_stack_init();
  device_manager_init();

  juma_init();
  
  // initialize Bluetooth Stack parameters.
  gap_params_init();
  advertising_init();
  services_init();
  dispatch_init();

  flash_init();
  
  if (app) {
			device_current_statue_set(DEVICE_STATUE_APP);
      data_storage_init();
      on_ready();
  } else {
			device_current_statue_set(DEVICE_STATUE_OTA);
      ble_device_set_name("OTA Mode");
      ota_init();
      ble_device_set_advertising_interval(200);
      ble_device_start_advertising();
  }
  
  // Enter main loop.
  for (;;)
  {
      watchDog_sys_SDK_dog1_RR();
    
      if(0 != enter_ota_mode_statue)
      {
        enter_ota_process(NULL);
      }
      else
      {
        dispatch();
      }
      
      serial_get();
      // Switch to a low power state until an event is available for the application
      err_code = sd_app_evt_wait();
      APP_ERROR_CHECK(err_code);
  }
}


