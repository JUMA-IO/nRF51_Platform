#向数据仓库写数据
***
##函数声明
```
uint32_t data_storage_write(uint8_t data_id, uint8_t data_len, uint8_t * data);
```


***
##函数功能
向数据仓库写入一段可以断电后保存的数据。

***
##函数参数
参数    | 数据类型   | 说明
:----- | :-------- | :------
*data_id*    | uint8_t  | 数据的索引值
*data_len*   | uint8_t  | 要写入的数据的长度
*data*		|uint8_t * |要写入的数据实体
*返回值*  | void      | 无

***
###关于 *data_id* 的说明

juma sdk 中提供了一个用于掉电后存储数据的数据仓库，该仓库在使用过程中，采用的是按照索引值也就是 data_id 来进行存储和读取的方式，
该索引值 在存储和读取的时候都要使用，在仓库内部作为数据的唯一标识。


***
###关于 *data_len* 的说明

juma sdk 数据仓库采用的是按照块来存储的的方式，每一个块的大小为32字节，其中有4个字节用来存储了juma内部的块说明信息，所以每一个存储块的大小最大为28字节。如果超过这个大小，会返回一个错误码 2

***
##函数举例
```
	//当前函数
	void foo( void )
	{
		...
		
		uint32_t err_code;
		
		err_code = data_storage_write(0, 16, "juma_sdk_storage");
		
		if(err_code > 0)
		{
			switch(err_code)
			{
				case 1: 
					//data_id is too many
				break;
				case 2: 
					//data_len is too many
				break;
			}
		}
		
		...
	}
```	
	