#include "sys_common.h"
#include "pstorage.h"

#define DATA_CACHE_BASIC 0x28000
#define DATA_CACHE_SIZE 0x00400
#define DATA_AREA_BASIC (DATA_CACHE_BASIC + DATA_CACHE_SIZE)
#define DATA_AREA_SIZE  0x01C00

#define DATA_PAGE_SIZE  0x00400
#define DATA_BLOCK_SIZE 0x00020

#define STORAGE_ID_MAX 16

#define DATA_FLAG 0x5A

typedef struct
{
  uint8_t data_id;
  uint8_t data_len;
  uint8_t data_flag;
  uint8_t NC;
  uint8_t data[DATA_BLOCK_SIZE - 4];
}data_storage_struct;

static void data_storage_callback(pstorage_handle_t * p_handle,
                                      uint8_t             op_code,
                                      uint32_t            result,
                                      uint8_t           * p_data,
                                      uint32_t            data_len);

static uint8_t data_storage_statue;

void data_storage_init(void)
{
  flash_set_function_callback_point(data_storage_callback);
  data_storage_statue = 0;
}



uint32_t data_storage_data[DATA_BLOCK_SIZE/4];

#define FLASH_DATA_COUNT_SETTEL ((DATA_BLOCK_SIZE * STORAGE_ID_MAX)/4)


static void data_storage_on_finish_ok(void * args)
{
  //write STEP 4...
  //callback user
  uint8_t op_code = *(uint8_t *)args;
  data_storage_on_finish(op_code);
}

static void data_storage_write_to_flash(void * args)
{
  //write STEP 3....
  //write data to flash
  uint32_t data_storage_addr = (uint32_t)args;

  data_storage_statue = 0;
  flash_program(DATA_AREA_BASIC, (uint8_t *)data_storage_data, DATA_BLOCK_SIZE, data_storage_addr);
}

static void data_storage_write_find_sapce(void * args);
static void settle_data_flash_write_data_flash_ok(void *args)
{
  //write SETP 2.5...
  //write new data to data flash
  data_storage_statue = 25;
  data_storage_write_find_sapce(NULL);
}

static void settle_data_flash_clear_data_flash_ok(void * args)
{
  //write STEP 2.4...
  //write valid data to data flash
  data_storage_struct * data_storage_data_q = (void *)DATA_CACHE_BASIC;
  uint32_t count = 0;
  
  for(count=0; data_storage_data_q[count].data_flag == 0x5A; count ++);
  data_storage_statue = 24;
  flash_program(DATA_AREA_BASIC, (uint8_t *)DATA_CACHE_BASIC, count * DATA_BLOCK_SIZE, 0);
}

static void settle_data_flash_write_cache_ok(void * args)
{
  //write STEP 2.3....
  //clear data_flash
  
  data_storage_statue = 23;
  flash_clear(DATA_AREA_BASIC, DATA_AREA_SIZE);
}

static void settle_data_flash_clear_cache_ok(void * args)
{
  //write STEP 2.2...
  //write valid value to cache
  uint32_t flash_data[FLASH_DATA_COUNT_SETTEL];
  uint32_t i, j;
  data_storage_struct * data_storage_data_point = (void *)flash_data;
  data_storage_struct * data_storage_data_q = (void *)DATA_AREA_BASIC;
  
  for(i=0; i<FLASH_DATA_COUNT_SETTEL; i++)
  {
    flash_data[i] = 0xFFFFFFFFUL;
  }
  
  //get valid data
  for(i=DATA_AREA_SIZE/DATA_BLOCK_SIZE; i>0; i--)
  {
    j = i-1;
    if(data_storage_data_q[j].data_flag == 0x5A)
    {
      if(data_storage_data_point[data_storage_data_q[j].data_id].data_flag == 0xFF)
      {
        if(data_storage_data_point[data_storage_data_q[j].data_id].data_id == 0xFF)
        {
          data_storage_data_point[data_storage_data_q[j].data_id] = data_storage_data_q[j];
        }
      }
    }
  }
  
  j = 0;
  for(i=0; i<STORAGE_ID_MAX; i++)
  {
    if(data_storage_data_point[i].data_flag == 0x5A)
    {
      data_storage_data_point[j++] = data_storage_data_point[i];
    }
  }
  
  data_storage_statue = 22;
  flash_program(DATA_CACHE_BASIC, (uint8_t *)flash_data, j*DATA_BLOCK_SIZE, 0);
}

static void settle_data_flash_clear_cache(void * args)
{
  //write STEP 2.1....
  //clear 

  data_storage_statue = 21;
  flash_clear(DATA_CACHE_BASIC, DATA_CACHE_SIZE);
}

static void data_storage_write_find_sapce(void * args)
{
  //write STEP 2... find space
  uint32_t data_storage_addr = DATA_AREA_BASIC;

  for(data_storage_addr = DATA_AREA_BASIC; data_storage_addr < DATA_AREA_BASIC+DATA_AREA_SIZE; data_storage_addr += DATA_BLOCK_SIZE)
  {
    if( (((data_storage_struct *)data_storage_addr)->data_flag) != 0x5A )
    {
      break;
    }
  }
  
  if(data_storage_addr >= DATA_AREA_BASIC+DATA_AREA_SIZE)
  {
    //current area is over
    run_when_idle(settle_data_flash_clear_cache, NULL);
  }
  else
  {
    data_storage_statue = 0;
    data_storage_addr -= DATA_AREA_BASIC;
    run_when_idle(data_storage_write_to_flash, (void *)data_storage_addr);
  }
}

uint32_t data_storage_write(uint8_t data_id, uint8_t data_len, uint8_t * data)
{
  data_storage_struct * data_storage_data_p = (data_storage_struct *)data_storage_data;
  
  if(data_id >= STORAGE_ID_MAX)
  {
    return 1;
  }
  
  if(data_len >= (DATA_BLOCK_SIZE - 4))
  {
    return 2;
  }
  
  //write STEP 1... 
  //ready data
  data_storage_data_p->data_id = data_id;
  data_storage_data_p->data_len = data_len;
  data_storage_data_p->data_flag = 0x5A;
 
  for(uint8_t i=0; i<data_len; i++)
  {
    data_storage_data_p->data[i] = data[i];
  }
  
  data_storage_statue = 1;
  run_when_idle(data_storage_write_find_sapce, NULL);
  
  return 0;
}

uint32_t data_storage_read(uint8_t data_id, uint8_t * data_len, uint8_t * data)
{
  //read STEP 1... find object on data area
  uint32_t data_storage_addr = DATA_AREA_BASIC;

  for(data_storage_addr = DATA_AREA_BASIC+DATA_AREA_SIZE - DATA_BLOCK_SIZE; data_storage_addr >= DATA_CACHE_BASIC; data_storage_addr -= DATA_BLOCK_SIZE)
  {
    if( (((data_storage_struct *)data_storage_addr)->data_flag) == 0x5A )
    {
      if( (((data_storage_struct *)data_storage_addr)->data_id) == data_id )
      {
        *data_len = ((data_storage_struct *)data_storage_addr)->data_len;
        memcpy(data, ((data_storage_struct *)data_storage_addr)->data, *data_len);
        return NRF_SUCCESS;
      }
    }
  }

  return 1;
}

static void data_storage_callback(pstorage_handle_t * p_handle,
                                      uint8_t             op_code,
                                      uint32_t            result,
                                      uint8_t           * p_data,
                                      uint32_t            data_len)
{
  switch (op_code)
  {
    case PSTORAGE_STORE_OP_CODE:
    {
      if(data_storage_statue == 0)
      {
        run_when_idle(data_storage_on_finish_ok, &op_code);
      }
      else if(data_storage_statue == 22)
      {
        settle_data_flash_write_cache_ok(NULL);
      }
      else if(data_storage_statue == 24)
      {
        settle_data_flash_write_data_flash_ok(NULL);
      }
    }break;

    case PSTORAGE_CLEAR_OP_CODE:
    {
      if(data_storage_statue == 21)
      {
        settle_data_flash_clear_cache_ok(NULL);
      }
      else if(data_storage_statue == 23)
      {
        settle_data_flash_clear_data_flash_ok(NULL);
      }
    }break;
    
    default:
    {
      
    }break;
  }  
}

