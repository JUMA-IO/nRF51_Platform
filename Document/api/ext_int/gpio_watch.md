#注册回调函数
***
##函数声明
```
void gpio_watch (uint8_t pin, uint8_t change_direction);
```

***
##函数功能
向系统注册一个 GPIO 事件，当满足指定条件的引脚状态变化满足时，  
`void gpio_on_change(uint32_t pins_state);`  
将会由系统调用。

可同时设置多个被检测的引脚，但是当一个引脚被触发没有释放的时候其他所有的引脚变化将无法被检测。

***
##函数参数
参数    | 数据类型   | 说明
:----- | :-------- | :------
*pin*    | uint8_t  | 引脚号
*change_direction*|uint8_t|引脚捕获的电平（即电平变为制定的值时，回调函数被触发）
*返回值*  | 无    | 无

***
###关于 *change_direction* 的描述
在 *juma_sdk_types.h* 中有定义如下

  GPIO_WATCH_LEVEL_LOW    = 0,
  GPIO_WATCH_LEVEL_HIGHT  = 1,

*返回值* 的数值|说明
:-------- | :------
GPIO_WATCH_LEVEL_LOW | 低电平
GPIO_WATCH_LEVEL_HIGHT| 高电平

***
##函数举例
```
	//回调函数
	void gpio_on_change(uint32_t pins_state);
	{
		...
		
		if(new_state == 0)
		{
			red_led_blink();	//伪指令
		}
		else
		{
			green_led_blink();	//伪指令
		}
		
		...
	}
	
	//当前函数
	void foo()
	{
		...
		
		//设置要检测的引脚，当指定的状态到达后会自动调用上面的回调函数
		gpio_watch(18, GPIO_WATCH_LEVEL_LOW);			//当18脚变为低电平时，上述的回调函数会被调用户
		
		...
	}

```