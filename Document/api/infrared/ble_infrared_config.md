#红外发射参数配置


***
##函数声明
```
uint32_t ble_infrared_config(ble_infrared_init_struct * Init_struct);
```

***
##函数功能
对接下来要发送的红外内容进行必要的参数配置。

***
##函数特点
在JUMA SDK的封装中，为用户提供了一个简易的红外发送功能，通过这个简易的红外发送装置，可以实现对常见的大多数的设备的控制。

另外，在本功能中采用的是将调制信号，和信号分开的方案，在红外发射管的正极加红外信号，在红外发射管的负极加调制信号，这样由于红外发射管是一个二极管，所以发送的信号也就等效于是将两个信号混合到一起进行发射了。
这样做的另外一个好处就是可以将红外的信号脚直接与原一体化的接收头进过一个简单的非门连接，从而达到将原来需要红外控制的设备改装成蓝牙控制的。

***
##函数参数
参数    | 数据类型   | 说明
:----- | :-------- | :------
*Init_struct*|ble_infrared_init_struct * |红外配置的结构体
*返回值*  | 无    | 无

***
###关于 *Init_struct* 的描述
这个参数就是要用来对红外发送设备进行配置的全部内容，其整体定义在juma_sdk_types.h中有对其的详细说明。
```
typedef enum
{
  PWM_HEAD_END = 0				//当前只完成了对最常见的红外遥控的编码	
}infraredProtocal;
```

```
typedef struct ble_infrared_init_struct_
{
  infraredProtocal Protocal_type;
	
  union {
    ble_infrared_pwm_config_struct pwm;
  }type_data;
	
}ble_infrared_init_struct;
```

```
typedef struct ble_infrared_pwm_config_struct_
{
	uint8_t infrared_signal_pin;          //pin num for infrared signal pin
	uint8_t infrared_shock_pin;           //pin num for shock signal pin
	
	//carrier
	uint16_t carrier_period;              //the time value of carrier period
	uint16_t carrier_plus;	              //the time value of carrier hight time
                
	//guidance              
	uint16_t guidance_burst;              //the carrier period number of guidance burst
	uint16_t guidance_space;              //the carrier period number of guidance space
                
	uint16_t logic_0_burst;	              //the carrier period number of logic 0 sending burst
	uint16_t logic_0_space;	              //the carrier period number of logic 0 sending space
                
	//logic 1             
	uint16_t logic_1_burst;	              //the carrier period number of logic 1 sending burst
	uint16_t logic_1_space;	              //the carrier period number of logic 1 sending space
	
	//end
	uint16_t end_burst;                   //the carrier period number of end gap burst 
	
}ble_infrared_pwm_config_struct;

```
上面结构体中各个参数代表的意义分别是：
```
infrared_signal_pin  //表示的是在使用过程中红外信号要输出的引脚
```
`````````
infrared_shock_pin	//表示的是红外的调制信号要输出的引脚
```
```
carrier_period		//表示的是载波调制信号的周期.
					//其计算公式为 period = (1/8 us)*carrier_period ;
				 	//当carrier_period取值为214的时候计算值为 26us, 频率为38.461，实测为37.9多点.
					//能够非常好的支持现在大多数常见的红外一体化接受管
```

```
carrier_plus		//用于设定红外发射载波的占空比，一般来说载波占空比。
					//一般来说采用 carrier_period的1/3可取得良好的发射效果.
					//当然也可以根据具体的情况来决定如何来使用。
```
```
guidance_burst		//起始信号的高电平时间，取值表示的红外载波周期的倍数。实际高电平时间为 guidance_burst*载波周期
					//之所以这样取值，是因为如果信号的变化和周期的变化不同步的话，出来的红外信号一般各个的间隔会有差异。
					//而且红外信号相对于载波周期来说，一般是比较长的，所以，不多的几个周期的差别在实际的项目中是完全可被容忍的。
```
```
guidance_space		//起始信号的低电平的时间，其取值为红外载波周期的倍数。实际高电平时间为 guidance_space*载波周期
					//其他的特性与guidance_burst一样
```
```
logic_0_burst		//逻辑0 高电平的时间，其他特性与 guidance_burst与guidance_space类似.
```
```
logic_0_space		//逻辑0 低电平的时间，其他特性与 guidance_burst与guidance_space类似.
```
```
logic_1_burst		//逻辑1 高电平的时间，其他特性与 guidance_burst与guidance_space类似.
```
```
logic_1_space		//逻辑1 低电平的时间，其他特性与 guidance_burst与guidance_space类似.
```
```
end_burst			//结束时的高电平时间，其他特性与 guidance_burst与guidance_space类似.
```

经过上面的配置后，即可调用其他的发射接口来进行发射了，当发射完毕后会进入一段时间的低电平时间，此时可以进行适当的延时（延时由用户决定）之后继续发送其他的内容。

***
##函数举例

```	
	//当前函数
	void foo()
	{
		...
		ble_infrared_init_struct init_struct;
		
		init_struct.Protocal_type = PWM_HEAD_END;
		
		init_struct.type_data.pwm.infrared_shock_pin = 6;
		init_struct.type_data.pwm.infrared_signal_pin = 5;
		
		init_struct.type_data.pwm.carrier_period = 214;
		init_struct.type_data.pwm.carrier_plus   = 150;
		
		init_struct.type_data.pwm.guidance_burst = 342;
		init_struct.type_data.pwm.guidance_space = 170;
		
		init_struct.type_data.pwm.logic_0_burst  = 21;
		init_struct.type_data.pwm.logic_0_space  = 21;
		
		init_struct.type_data.pwm.logic_1_burst  = 21;
		init_struct.type_data.pwm.logic_1_space  = 62;
		        
		init_struct.type_data.pwm.end_burst      = 21;
		
		ble_infrared_config(&init_struct);

		
		ble_infrared_send("MSB byte", 32);
		...
	}
```
