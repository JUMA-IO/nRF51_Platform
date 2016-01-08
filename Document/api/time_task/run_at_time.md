#定时运行

***
##函数声明
	void run_at_time(function_t func, void* args, uint32_t time);

***
##函数功能
向系统注册一个在指定的时间点上运行的函数。

***
##函数参数
参数    | 数据类型   | 说明
:----- | :-------- | :------
*func*|function_t|要执行的函数的位置(函数指针)
*args*|void * |执行函数时传入函数的参数(只能是一个指针)
*time*|uint32_t|函数执行的时间点
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
		...
	}

	//当前函数
	void foo()
	{
		...
	
		//调用本函数向系统注册一个在指定时间点上会被执行的任务
		run_at_time(led_on_task, NULL, current_time() + 100)；
	
		...
	}
```