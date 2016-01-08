#include "sys_common.h"


static pstorage_handle_t flash_storage;

flash_callback_function flash_callback_function_point = NULL;


uint32_t pstorage_raw_register(pstorage_module_param_t * p_module_param,
                               pstorage_handle_t       * p_block_id);
uint32_t pstorage_raw_store(pstorage_handle_t * p_dest,
                            uint8_t           * p_src,
                            pstorage_size_t     size,
                            pstorage_size_t     offset);
uint32_t pstorage_raw_clear(pstorage_handle_t * p_dest, pstorage_size_t size);

static void flash_storage_callback_handler(pstorage_handle_t * p_handle,
                                      uint8_t             op_code,
                                      uint32_t            result,
                                      uint8_t           * p_data,
                                      uint32_t            data_len);


void flash_init(void)
{
    uint32_t                err_code;
    pstorage_module_param_t storage_module_param = {.cb = flash_storage_callback_handler};
    
    flash_callback_function_point = NULL;
    err_code = pstorage_raw_register(&storage_module_param, &flash_storage);
    APP_ERROR_CHECK(err_code);
}

uint32_t flash_clear(uint32_t addr, uint32_t size)
{
  uint32_t                err_code;

  flash_storage.block_id  = addr;

  err_code = pstorage_raw_clear(&flash_storage, size);
  APP_ERROR_CHECK(err_code);
  
  return NRF_SUCCESS;
}


uint32_t flash_program(uint32_t basic_addr, uint8_t * data, uint32_t len, uint32_t offset)
{
  uint32_t                err_code;
  
  flash_storage.block_id  = basic_addr;
  err_code = pstorage_raw_store(&flash_storage, data, len, offset);
  APP_ERROR_CHECK(err_code);

  return NRF_SUCCESS;
}


uint32_t flash_read(uint32_t addr, uint8_t * data, uint32_t len)
{
  memcpy(data, (const void *)addr, len);
  
  return NRF_SUCCESS;
}

static void flash_storage_callback_handler(pstorage_handle_t *  p_handle,
                                      uint8_t             op_code,
                                      uint32_t            result,
                                      uint8_t           * p_data,
                                      uint32_t            data_len)
{
  if(flash_callback_function_point) flash_callback_function_point(p_handle, op_code, result, p_data, data_len);
}

uint32_t flash_set_function_callback_point(flash_callback_function flash_callback)
{
  flash_callback_function_point = flash_callback;
  
  return NRF_SUCCESS;
}
