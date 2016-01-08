
#看门狗配置
***
##函数声明
```
uint32_t watchDog_Config(uint32_t juma_wdt_en);
```

***
##函数功能
看门狗模块的配置。

***
##函数特点
在JUMA SDK中，提供看门狗的使用接口，通过该接口可以快速简单的配置看门狗模块。

***
##函数参数
参数    | 数据类型   | 说明
:----- | :-------- | :------
*juma_wdt_en*|uint32_t|将要使能的看门狗
*返回值*  | 无    | 无

***
###关于 *juma_wdt_en* 的描述
关于juma_wdt_en可取得值，在juma_sdk_types.h中有如下的定义:
```
#define JUMA_WDT_EN_USER_DOG1   0x01
#define JUMA_WDT_EN_USER_DOG2   0x02
#define JUMA_WDT_EN_USER_DOG3   0x04
#define JUMA_WDT_EN_USER_DOG4   0x08
              
#define JUMA_WDT_EN_SYS_SDK_DOG 0x80
```
其中 JUMA_WDT_EN_USER_DOG1 ~ JUMA_WDT_EN_USER_DOG4 是提供给用户使用的。用户在使能了这些看门狗后，需要用对应的喂狗函数来进行喂狗，负责程序在看门狗时钟到达后会产生void watchDog_on_timerout(uint32_t juma_wdt_statue);的回调。没有使能的看门狗不需要进行喂狗。喂狗函数如下所示。详情请参照该函数具体的位置。
```
void watchDog_user_dog1_RR(void);
void watchDog_user_dog2_RR(void);
void watchDog_user_dog3_RR(void);
void watchDog_user_dog4_RR(void);
```

而其中 JUMA_WDT_EN_SYS_SDK_DOG 则对应的是 JUMA SDK部分，如果使能了该看门狗，那么当 juma sdk内部发生错误的时候，也会像用户的看门狗一样产生void watchDog_on_timerout(uint32_t juma_wdt_statue);的回调。

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
