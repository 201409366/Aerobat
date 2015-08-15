#include "24cxx.h" 
#include <rtdevice.h>

struct rt_i2c_bus_device *i2c_bus;
unsigned char i2c_addr = (0xA0 >> 1);

//��ʼ��IIC�ӿ�
void AT24CXX_Init(void)
{
	i2c_bus = (struct rt_i2c_bus_device *)rt_device_find("i2c1");
}
//��AT24CXXָ����ַ����һ������
//ReadAddr:��ʼ�����ĵ�ַ  
//����ֵ  :����������
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
//��AT24CXXָ����ַд��һ������
//WriteAddr  :д�����ݵ�Ŀ�ĵ�ַ    
//DataToWrite:Ҫд�������
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
//��AT24CXX�����ָ����ַ��ʼд�볤��ΪLen������
//�ú�������д��16bit����32bit������.
//WriteAddr  :��ʼд��ĵ�ַ  
//DataToWrite:���������׵�ַ
//Len        :Ҫд�����ݵĳ���2,4
void AT24CXX_WriteLenByte(u16 WriteAddr,u32 DataToWrite,u8 Len)
{  	
	u8 t;
	for(t=0;t<Len;t++)
	{
		AT24CXX_WriteOneByte(WriteAddr+t,(DataToWrite>>(8*t))&0xff);
	}												    
}

//��AT24CXX�����ָ����ַ��ʼ��������ΪLen������
//�ú������ڶ���16bit����32bit������.
//ReadAddr   :��ʼ�����ĵ�ַ 
//����ֵ     :����
//Len        :Ҫ�������ݵĳ���2,4
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
//���AT24CXX�Ƿ�����
//��������24XX�����һ����ַ(255)���洢��־��.
//���������24Cϵ��,�����ַҪ�޸�
//����1:���ʧ��
//����0:���ɹ�
u8 AT24CXX_Check(void)
{
	u8 temp;
	temp=AT24CXX_ReadOneByte(0xCC);//����ÿ�ο�����дAT24CXX			   
	if(temp==0XAC)return 0;		   
	
	else//�ų���һ�γ�ʼ�������
	{
		AT24CXX_WriteOneByte(0xCC,0XAC);
		rt_thread_delay(1);
	  temp=AT24CXX_ReadOneByte(0xCC);	  
		
		if(temp==0XAC)return 0;
	}
	return 1;											  
}

//��AT24CXX�����ָ����ַ��ʼ����ָ������������
//ReadAddr :��ʼ�����ĵ�ַ ��24c02Ϊ0~255
//pBuffer  :���������׵�ַ
//NumToRead:Ҫ�������ݵĸ���
void AT24CXX_Read(u16 ReadAddr,u8 *pBuffer,u16 NumToRead)
{
	while(NumToRead)
	{
		*pBuffer++=AT24CXX_ReadOneByte(ReadAddr++);	
		NumToRead--;
	}
}  
//��AT24CXX�����ָ����ַ��ʼд��ָ������������
//WriteAddr :��ʼд��ĵ�ַ ��24c02Ϊ0~255
//pBuffer   :���������׵�ַ
//NumToWrite:Ҫд�����ݵĸ���
void AT24CXX_Write(u16 WriteAddr,u8 *pBuffer,u16 NumToWrite)
{
	while(NumToWrite--)
	{
		AT24CXX_WriteOneByte(WriteAddr,*pBuffer);
		WriteAddr++;
		pBuffer++;
	}
}
 











