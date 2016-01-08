#数据仓库存储完成后返回
***
##函数声明
```
void data_storage_on_finish(uint8_t op_code);
```

***
##函数功能
juma sdk 支持一个数据仓库，以方便用户存储一些掉电需要保存的数据，该仓库只需要用户给定一个索引值，就可以存放一段长度不超过28个字节的内容，由于flash存储的异步性，当写flash操作时并不能立刻写完成，因此设计该函数，当写flash的操作在内部完成时，该函数会被自动的调用。

***
##函数参数
参数    | 数据类型   | 说明
:----- | :-------- | :------
*op_code*|uint8_t|为扩展以后留用当前并没有什么作用
*返回值*  | void      | 无


***
##函数举例
```
	//当前函数
	void data_storage_on_finish(uint8_t op_code)
	{
	
	}
	
	void fun()
	{
		//当该操作完成后上述的函数会被调用
		data_storage_write(0, 16, "juma_sdk_storage";
	}
	
```