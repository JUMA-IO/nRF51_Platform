
#调光灯设置
***
##函数声明
```
void light_setup(uint8_t* pins, uint8_t is_active_high);
```

***
##函数功能
对调光灯进行设置

***
##函数特点
JUMA SDK中自带对rgb3色的调光灯的支持，通过该函数可以设置对调光灯的操作。

***
##函数参数
参数    | 数据类型   | 说明
:----- | :-------- | :------
*pins*|uint8_t * |用于控制调光灯的引脚
*is_active_high*|uint8_t|驱动调光灯变亮的电平
*返回值*  | 无    | 无

***
###关于 *pins* 的描述
用于指定与调光灯相连的引脚。
在juma_sdk_types.h中有如下的定义。

```
	typedef struct _light_config_t {
	
    	uint8_t pins[4]; // pins for RGBW
    	
	} light_config_t;
```

###关于 *is_active_high*的描述
其用来指示驱动调光灯需要的电平。
如果调光灯一端和高电平连接，另一端和芯片的驱动电路相连，那么芯片需要提供低电平才能点亮调光灯，这时应该写0.否则写非零。

is_active_high的值| 对应的应用场合
:-----| :------
*0*|低电平驱动调光灯
*非零*|高电平驱动调光灯

***
##函数举例

```	
	//当前函数
	void foo()
	{
		...
	
		light_config_t light_init_config;
		
		light_init_config.pins[0] = 21;		//R
		light_init_config.pins[1] = 22;		//G
		light_init_config.pins[2] = 23;		//B
	
		//配置调光灯 其驱动引脚为21,22,23 其驱动电平为低
		light_setup((uint8_t *)&light_init_config, 0);
	
		...
	}
```
