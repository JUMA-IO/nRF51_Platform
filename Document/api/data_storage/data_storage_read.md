#向数据仓库读数据
***
##函数声明
```
uint32_t data_storage_read(uint8_t data_id, uint8_t * data_len, uint8_t * data);
```


***
##函数功能
向数据仓库写入一段可以断电后保存的数据。

***
##函数参数
参数    | 数据类型   | 说明
:----- | :-------- | :------
*data_id*    | uint8_t  | 数据的索引值
*data_len*   | uint8_t * | 数据体的长度，由函数设置
*data*		|uint8_t * |要写入的数据实体
*返回值*  | void      | 无

***
###关于 *data_id* 的说明

juma sdk 中提供了一个用于掉电后存储数据的数据仓库，该仓库在使用过程中，采用的是按照索引值也就是 data_id 来进行存储和读取的方式，
该索引值 在存储和读取的时候都要使用，在仓库内部作为数据的唯一标识。


***
###关于 *data_len* 的说明

存储一个数据块的时候会写入一个数据长度到块描述信息部分，当读取数据块的时候，该部分的信息也会被返回出来。这里主要传入一个用于接收返回的数据块大小的地址就好。

***
##函数举例
```
	//当前函数
	void foo( void )
	{
		...
		
		uint32_t err_code;
		uint8_t buff[28];
		uint8_t len;
		
		err_code = data_storage_read(0, &len, buff);
		
		if(err_code > 0)
		{
			switch(err_code)
			{
				case 1: 
					//black is error
				break;
	
			}
		}
		
		...
	}
```	
	