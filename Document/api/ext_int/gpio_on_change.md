#GIPO状态变化事件
***
##函数声明
```
void gpio_on_change(uint32_t pins_state);
```

***
##函数功能
在外部 GPIO 的状态被改变后由系统调用。 
（当前仅支持一个外部的引脚，若同时使用两个及以上可能导致全部失效） 
用于处理 GIPOI 状态变化后的任务。

***
##函数参数

参数 |数据类型 |说明
:-------- | :------ | :------
*pins_state*|uint8_t|当前引脚的状态
*返回值*  | void      | 无

***
###关于 *new_state* 参数

*new_state* 的数值|说明
:-------- | :------
0-0xffffff|当前的引脚的状态 

***
##函数举例
```
	//当前函数
	void	gpio_on_change(uint32_t  pins_state)
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
```