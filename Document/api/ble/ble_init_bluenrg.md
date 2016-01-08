
#初始化BlueNRG
***
##函数声明

###ST
```
tBleStatus ble_init_bluenrg(void);
```

***
##函数功能
GATT初始化，GAP初始化，添加特定Service。
在广播参数设置完成后调用。  

***
##函数参数
###ST
参数    | 数据类型   | 说明
:----- | :-------- | :------
*返回值*  | ret    | status

***
##函数举例

```	

	//当前函数
	void foo()
	{
		...
		tBleStatus ret；
		//调用本函数初始化BlueNRG
		ret = ble_init_bluenrg(void);
	
		...
	}
```
