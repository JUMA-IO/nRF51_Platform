#空闲运行
***
##函数声明
```
	void run_when_idle(function_t func, void* args);
```
***
##函数功能
向系统注册一个在系统空闲的时候运行的函数。  
要执行的函数对时间要求不高。

***
##函数参数
参数    | 数据类型   | 说明
:----- | :-------- | :------
*func*|function_t|要执行的函数的位置(函数指针)
*args*|void * |执行函数时传入函数的参数(只能是一个指针)
*返回值*  | void      | 无

***
###关于 *func* 参数
其格式的定义为  
```
	typedef void (*function_t)(void* args);
```  
***
##函数举例
```	
	//写一个需要运行的函数
	void led_on_task(void* args)
	{
		// Pull Up
		gpio_write(LED_1, 1);
		run_after_delay(led_off_task, NULL, 250);
	}

	//当前函数
	
	void foo()
	{
		...
	
		//调用本函数向系统注册一个在系统空闲时会被执行的任务
		run_when_idle(led_on_task, NULL)；
	
		...
	}
```