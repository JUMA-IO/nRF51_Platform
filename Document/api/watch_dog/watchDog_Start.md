
#看门狗开始工作
***
##函数声明
```
uint32_t watchDog_Start(uint32_t juma_wdt_timer_out_value);
```

***
##函数功能
看门狗开始工作

***
##函数参数
参数    | 数据类型   | 说明
:----- | :-------- | :------
*juma_wdt_timer_out_value*|uint32_t|触发看门狗回调函数需要经过的周期数
*返回值*  | 无    | 无

***
###关于 *juma_wdt_timer_out_value* 的描述
关于 juma_wdt_timer_out_value 的取值，可参考：
timeout [s] = ( juma_wdt_timer_out_value + 1 ) / 32768

***
##函数举例


```	
	// 定时器时间到后的处理函数
	void watchDog_on_timerout(uint32_t juma_wdt_statue)
	{
		...
			NVIC_SystemReset();
		...
	}

	//当前函数
	void foo()
	{
		...
		//看门狗的配置（使能了JUMA SDK内部的看门狗 和 用户 的看门狗1）
		watchDog_Config(JUMA_WDT_EN_SYS_SDK_DOG | JUMA_WDT_EN_USER_DOG1);
		//看是看门狗运行。	
		//（参数的选取应该按照实际的使用情况选取，如果选的值太大，可能不能达到很好的效果，如果太小，可能导致程序无法正常的使用)	
		watchDog_Start(7000);
		//喂狗操作，由于使能了用户看门狗1所以需要执行喂狗操作，否则，系统将会回调void watchDog_on_timerout(uint32_t juma_wdt_statue);
		//JUMA_WDT_EN_SYS_SDK_DOG 使能的看门狗不需要用户执行喂狗操作，由JUMA SDK内部完成喂狗动作。
		watchDog_user_dog1_RR();
		...
	}
```
