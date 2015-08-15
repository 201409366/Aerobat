#include "24cxx.h" 
#include <rtdevice.h>

struct rt_i2c_bus_device *i2c_bus;
unsigned char i2c_addr = (0xA0 >> 1);

//初始化IIC接口
void AT24CXX_Init(void)
{
	i2c_bus = (struct rt_i2c_bus_device *)rt_device_find("i2c1");
}
//在AT24CXX指定地址读出一个数据
//ReadAddr:开始读数的地址  
//返回值  :读到的数据
u8 AT24CXX_ReadOneByte(u16 ReadAddr)
{				  
	u8 temp=0,address;	
	struct rt_i2c_msg msgs;
	
	address = ReadAddr;
	msgs.addr = i2c_addr;
	msgs.flags = RT_I2C_WR;
	msgs.buf   = &address;
	msgs.len   = 1;	
	rt_i2c_transfer(i2c_bus, &msgs, 1);
	
	msgs.addr = i2c_addr;
	msgs.flags = RT_I2C_RD; /* Read from slave */	
	msgs.len   = 1;	
	msgs.buf   = &temp;	

	rt_i2c_transfer(i2c_bus, &msgs, 1);
	return temp;	
	
}
//在AT24CXX指定地址写入一个数据
//WriteAddr  :写入数据的目的地址    
//DataToWrite:要写入的数据
void AT24CXX_WriteOneByte(u16 WriteAddr,u8 DataToWrite)
{				
	u8 data[2];	
	struct rt_i2c_msg msgs;
	
	data[0] = WriteAddr;
	data[1] = DataToWrite;
	
	msgs.addr = i2c_addr;
	msgs.flags = RT_I2C_WR;
	msgs.buf   = &data[0];
	msgs.len   = 2;
	rt_i2c_transfer(i2c_bus, &msgs, 1);		
}
//在AT24CXX里面的指定地址开始写入长度为Len的数据
//该函数用于写入16bit或者32bit的数据.
//WriteAddr  :开始写入的地址  
//DataToWrite:数据数组首地址
//Len        :要写入数据的长度2,4
void AT24CXX_WriteLenByte(u16 WriteAddr,u32 DataToWrite,u8 Len)
{  	
	u8 t;
	for(t=0;t<Len;t++)
	{
		AT24CXX_WriteOneByte(WriteAddr+t,(DataToWrite>>(8*t))&0xff);
	}												    
}

//在AT24CXX里面的指定地址开始读出长度为Len的数据
//该函数用于读出16bit或者32bit的数据.
//ReadAddr   :开始读出的地址 
//返回值     :数据
//Len        :要读出数据的长度2,4
u32 AT24CXX_ReadLenByte(u16 ReadAddr,u8 Len)
{  	
	u8 t;
	u32 temp=0;
	for(t=0;t<Len;t++)
	{
		temp<<=8;
		temp+=AT24CXX_ReadOneByte(ReadAddr+Len-t-1); 	 				   
	}
	return temp;												    
}
//检查AT24CXX是否正常
//这里用了24XX的最后一个地址(255)来存储标志字.
//如果用其他24C系列,这个地址要修改
//返回1:检测失败
//返回0:检测成功
u8 AT24CXX_Check(void)
{
	u8 temp;
	temp=AT24CXX_ReadOneByte(0xCC);//避免每次开机都写AT24CXX			   
	if(temp==0XAC)return 0;		   
	
	else//排除第一次初始化的情况
	{
		AT24CXX_WriteOneByte(0xCC,0XAC);
		rt_thread_delay(1);
	  temp=AT24CXX_ReadOneByte(0xCC);	  
		
		if(temp==0XAC)return 0;
	}
	return 1;											  
}

//在AT24CXX里面的指定地址开始读出指定个数的数据
//ReadAddr :开始读出的地址 对24c02为0~255
//pBuffer  :数据数组首地址
//NumToRead:要读出数据的个数
void AT24CXX_Read(u16 ReadAddr,u8 *pBuffer,u16 NumToRead)
{
	while(NumToRead)
	{
		*pBuffer++=AT24CXX_ReadOneByte(ReadAddr++);	
		NumToRead--;
	}
}  
//在AT24CXX里面的指定地址开始写入指定个数的数据
//WriteAddr :开始写入的地址 对24c02为0~255
//pBuffer   :数据数组首地址
//NumToWrite:要写入数据的个数
void AT24CXX_Write(u16 WriteAddr,u8 *pBuffer,u16 NumToWrite)
{
	while(NumToWrite--)
	{
		AT24CXX_WriteOneByte(WriteAddr,*pBuffer);
		WriteAddr++;
		pBuffer++;
	}
}
 











