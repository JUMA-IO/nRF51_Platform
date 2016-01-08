#看门狗的回调函数
***
##函数声明
```
void watchDog_on_timerout(uint32_t juma_wdt_statue);
```

***
##函数功能
当看起看门狗，而没有及时的执行喂狗操作的话，那么这个函数将会被调用。

***
###函数参数
参数    | 数据类型   | 说明
:----- | :-------- | :------
*juma_wdt_statue*|uint32_t|看门狗的状态
*返回值*  | 无    | 无

###关于 *juma_wdt_statue* 的描述
关于juma_wdt_statue的使用，可与在juma_sdk_types.h中定义的:
```
#define JUMA_WDT_USER_DOG1      0x01
#define JUMA_WDT_USER_DOG2      0x02  
#define JUMA_WDT_USER_DOG3      0x04
#define JUMA_WDT_USER_DOG4      0x08
```
做 &操作，可得知当前的看门狗超时是由于哪个计时器没有被及时清除。
如果这些都不是的话，那么就是JUMA SDK内部的看门狗超时。

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