
#GPIO配置
***
##函数声明
```
void gpio_setup (uint8_t pin, uint8_t mode);
```

***
##函数功能
用于对一个 GPIO 引脚的功能进行设定(输入 或 输入)。

***
##函数参数
参数    | 数据类型   | 说明
:----- | :-------- | :------
*pin*    | uint8_t  | 引脚号
*mode*   | uint8_t  | 引脚的工作模式
*返回值*  | void      | 无

***
###关于 *mode* 参数
*juma_sdk_types.h* 文件中定义了 *mode* 参数可以接受的数值范围：

*mode* 的数值|说明
:-------- | :------
GPIO_OUTPUT | 将GPIO配置成输出模式
GPIO_INPUT_NOPULL|将GPIO配置为输入悬空模式
GPIO_INPUT_PULLUP|将GPIO配置为输入上拉模式 
GPIO_INPUT_PULLDOWN|将GPIO配置为输入下拉模式

***
##函数举例
```
	void foo( void )
	{
		...
		
		/*将偏移量为18的引脚
		配置成GPIO_OUTPUT的工作模式*/
		gpio_setup(18, GPIO_OUTPUT);
		
		...
	}

```