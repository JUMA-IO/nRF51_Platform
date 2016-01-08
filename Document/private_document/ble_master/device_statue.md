获取 设备当前的状态

通过发送 
命令格式：{0x83, any, any}

返回数据：

{0x83, 1, current_statue}

状态值：
typedef enum
{
  device_statue_app = 0,
  device_statue_ota = 1,
}device_statue_t;






