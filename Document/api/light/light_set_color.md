
#设置调光灯的颜色
***
##函数声明
```
void light_set_color(const uint8_t* rgb_values);
```

***
##函数功能
对调光灯的颜色进行设定

***
##函数参数
参数    | 数据类型   | 说明
:----- | :-------- | :------
*rgb_values*|const uint8_t * |要显示的颜色值
*返回值*  | 无    | 无

***
###关于 *rgb_values* 的描述
rgb的颜色设定，其通过一个数组来一次性指定全部的颜色值<br>

数组编号|对应的颜色值
:----|:----
rgb_values[0] | r_values
rgb_values[1] | g_values
rgb_values[2] | b_values

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
	
		uint8_t color[4] = {0, 100, 0, 0};

		light_set_color(color);
	
		/*之后就可以调用开启调光灯的函数来打开调光灯了*/
	
		...
	}
```
