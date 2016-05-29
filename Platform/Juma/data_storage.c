//flash存可以一块一块存，但是擦的时候是一页一页擦（1kb相当于0x400）
#include "sys_common.h"
#include "pstorage.h"

#define DATA_CACHE_BASIC 0x28000
#define DATA_CACHE_SIZE 0x00400
#define DATA_AREA_BASIC (DATA_CACHE_BASIC + DATA_CACHE_SIZE)
#define DATA_AREA_SIZE  0x01C00

//#define DATA_PAGE_SIZE  0x00400
#define DATA_BLOCK_SIZE 0x00020

#define STORAGE_ID_MAX 18
#define USER_ID_MAX 16
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
//将缓存里的数据放入flash
static void settle_data_flash_clear_data_flash_ok(void * args)
{
  //write STEP 2.4...
  //write valid data to data flash
  data_storage_struct * data_storage_data_q = (void *)DATA_CACHE_BASIC;
  uint32_t count = 0;
  
  for(count=0; data_storage_data_q[count].data_flag == DATA_FLAG; count ++);
  data_storage_statue = 24;
  flash_program(DATA_AREA_BASIC, (uint8_t *)DATA_CACHE_BASIC, count * DATA_BLOCK_SIZE, 0);
}
//清除存储区
static void settle_data_flash_write_cache_ok(void * args)
{
  //write STEP 2.3....
  //clear data_flash
  
  data_storage_statue = 23;
  flash_clear(DATA_AREA_BASIC, DATA_AREA_SIZE);
}
//将存储的数据放入缓存区
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
  
  //get valid data 把不同id的最新有效数据放入flash_data中
  for(i=DATA_AREA_SIZE/DATA_BLOCK_SIZE; i>0; i--)
  {
    j = i-1;
    if(data_storage_data_q[j].data_flag == DATA_FLAG)
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
  //将数组从1开始按照id从小到大依次排序
  j = 0;
  for(i=0; i<STORAGE_ID_MAX; i++)
  {
    if(data_storage_data_point[i].data_flag == DATA_FLAG)
    {
      data_storage_data_point[j++] = data_storage_data_point[i];
    }
  }
  //将数据放入缓存
  data_storage_statue = 22;
  flash_program(DATA_CACHE_BASIC, (uint8_t *)flash_data, j*DATA_BLOCK_SIZE, 0);
}
//清空缓存区域
static void settle_data_flash_clear_cache(void * args)
{
  //write STEP 2.1....
  //clear 

  data_storage_statue = 21;
  flash_clear(DATA_CACHE_BASIC, DATA_CACHE_SIZE);
}
//寻找写数据的空间地址
static void data_storage_write_find_sapce(void * args)
{
  //write STEP 2... find space
  uint32_t data_storage_addr = DATA_AREA_BASIC;//起始地址
  //寻找特定id的地址若没有则获取未被写入数据的flash地址
  for(data_storage_addr = DATA_AREA_BASIC; data_storage_addr < DATA_AREA_BASIC+DATA_AREA_SIZE; data_storage_addr += DATA_BLOCK_SIZE)
  {
    if( (((data_storage_struct *)data_storage_addr)->data_flag) != DATA_FLAG )
    {
      break;
    }
  }
  //如果当前存储区域满了则将数据放入缓存区域并清空当前区域
  if(data_storage_addr >= DATA_AREA_BASIC+DATA_AREA_SIZE)
  {
    //current area is over
    run_when_idle(settle_data_flash_clear_cache, NULL);
  }
  else
  {
    data_storage_statue = 0;
    data_storage_addr -= DATA_AREA_BASIC;
    run_when_idle(data_storage_write_to_flash, (void *)data_storage_addr);//将数据写入当前地址
  }
}
//写入数据
uint32_t data_storage_write(uint8_t data_id, uint8_t data_len, uint8_t * data)
{
  data_storage_struct * data_storage_data_p = (data_storage_struct *)data_storage_data;//使全局变量可以使用自定义结构
  
  if(data_id >= USER_ID_MAX)
  {
    return 1;//id 超过最大值返回1
  }
  
  if(data_len >= (DATA_BLOCK_SIZE - 4))
  {
    return 2;//长度超过最大值返回2
  }
  
  //write STEP 1... 
  //ready data 给变量赋值
  data_storage_data_p->data_id = data_id;
  data_storage_data_p->data_len = data_len;
  data_storage_data_p->data_flag = DATA_FLAG;
 
  for(uint8_t i=0; i<data_len; i++)
  {
    data_storage_data_p->data[i] = data[i];
  }
  
  data_storage_statue = 1;//?
  run_when_idle(data_storage_write_find_sapce, NULL);// 空闲时执行data_storage_write_find_sapce
  
  return 0;//执行完毕返回0
}
uint32_t manufacturer_storage_write(uint8_t data_id, uint8_t data_len, uint8_t * data)
{
  data_storage_struct * data_storage_data_p = (data_storage_struct *)data_storage_data;//使全局变量可以使用自定义结构 
  //write STEP 1... 
  //ready data 给变量赋值
  data_storage_data_p->data_id = data_id;
  data_storage_data_p->data_len = data_len;
  data_storage_data_p->data_flag = DATA_FLAG;
 
  for(uint8_t i=0; i<data_len; i++)
  {
    data_storage_data_p->data[i] = data[i];
  }
  
  run_when_idle(data_storage_write_find_sapce, NULL);// 空闲时执行data_storage_write_find_sapce
  
  return 0;//执行完毕返回0
}
//读取数据
uint32_t data_storage_read(uint8_t data_id, uint8_t * data_len, uint8_t * data)
{
  //read STEP 1... find object on data area
  uint32_t data_storage_addr = DATA_AREA_BASIC;
  //从存储区到缓存区反向搜索数据
  for(data_storage_addr = DATA_AREA_BASIC+DATA_AREA_SIZE - DATA_BLOCK_SIZE; data_storage_addr >= DATA_CACHE_BASIC; data_storage_addr -= DATA_BLOCK_SIZE)
  {
    if( (((data_storage_struct *)data_storage_addr)->data_flag) == DATA_FLAG )
    {
      if( (((data_storage_struct *)data_storage_addr)->data_id) == data_id )
      {
        *data_len = ((data_storage_struct *)data_storage_addr)->data_len;
        memcpy(data, ((data_storage_struct *)data_storage_addr)->data, *data_len);
        return NRF_SUCCESS;
      }
    }
  }

  return 1;//未读到数据返回1
}
//数据存储回调
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
      if(data_storage_statue == 0)//写完数据后触发data_storage_on_finish回调函数
      {
        run_when_idle(data_storage_on_finish_ok, &op_code);
      }
      else if(data_storage_statue == 22)
      {
        settle_data_flash_write_cache_ok(NULL);//清除存储区后执行23
      }
      else if(data_storage_statue == 24)
      {
        settle_data_flash_write_data_flash_ok(NULL);//重新寻找空间写入数据
      }
    }break;

    case PSTORAGE_CLEAR_OP_CODE:
    {
      if(data_storage_statue == 21)//清空缓存区后执行
      {
        settle_data_flash_clear_cache_ok(NULL);//将存储的数据放入缓存区后执行22
      }
      else if(data_storage_statue == 23)
      {
        settle_data_flash_clear_data_flash_ok(NULL);//将缓存里的数据放入flash后执行24
      }
    }break;
    
    default:
    {
      
    }break;
  }  
}

