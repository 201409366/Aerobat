/**************************** nRF24L01.c *************************************
��ʱ��֪���������������SPI�豸��

��֪�������鵽��һ���豸�С�

**********************************************************************************/
#include "nRF24L01.h"
#include <drivers/spi.h>

#define FLASH_DEBUG

#ifdef FLASH_DEBUG
#define FLASH_TRACE         rt_kprintf
#else
#define FLASH_TRACE(...)
#endif /* #ifdef FLASH_DEBUG */

static struct rt_spi_device * rt_spi_device;

static u8 TX_ADDRESS[TX_ADR_WIDTH] = {0x34,0x43,0x10,0x10,0x01}; // ����һ����̬���͵�ַ
static u8 RX_ADDRESS[RX_ADR_WIDTH] = {0x34,0x43,0x10,0x10,0x01};
/*
 * ��������SPI_RF24L01_RW
 * ����  ��������RF24L01��/дһ�ֽ�����
 * ����  ��д�������
 * ���  ����ȡ�õ�����
 * ����  ���ڲ�����
 */
u8 SPI_RF24L01_RW(u8 dat)
{  	
//   /* �� SPI���ͻ������ǿ�ʱ�ȴ� */
//  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
//  
//   /* ͨ�� SPI2����һ�ֽ����� */
//  SPI_I2S_SendData(SPI1, dat);		
// 
//   /* ��SPI���ջ�����Ϊ��ʱ�ȴ� */
//  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);

  /* Return the byte read from the SPI bus */
	u8 val;
	
	if(rt_spi_device != RT_NULL) 
		rt_spi_transfer(rt_spi_device,&dat,&val,1);
	else
		val = 0;
	
  return val;
}

/*
 * ��������SPI_RF24L01_WriteReg
 * ����  ��������RF24L01�ض��ļĴ���д������
 * ����  ��reg:RF24L01������+�Ĵ�����ַ��
 		   dat:��Ҫ��Ĵ���д�������
 * ���  ��RF24L01��status�Ĵ�����״̬
 * ����  ���ڲ�����
 */
u8 SPI_RF24L01_WriteReg(u8 reg,u8 dat)
{
 	u8 sendBuff[2];
	u8 recvBuff[2];
	
	 RF24L01_CE_LOW();
	
	sendBuff[0] = reg;
	sendBuff[1] = dat;
	rt_spi_transfer(rt_spi_device,sendBuff,recvBuff,2);
	
  return recvBuff[0];
}


/*
 * ��������SPI_RF24L01_ReadReg
 * ����  �����ڴ�RF24L01�ض��ļĴ�����������
 * ����  ��reg:RF24L01������+�Ĵ�����ַ��
 * ���  ���Ĵ����е�����
 * ����  ���ڲ�����
 */
u8 SPI_RF24L01_ReadReg(u8 reg)
{
 	u8 reg_val;
	rt_spi_transfer(rt_spi_device,&reg,&reg_val,1);
	return reg_val;
}	


/*
 * ��������SPI_RF24L01_ReadBuf
 * ����  �����ڴ�RF24L01�ļĴ����ж���һ������
 * ����  ��reg:RF24L01������+�Ĵ�����ַ��
 		   pBuf�����ڴ洢���������ļĴ������ݵ����飬�ⲿ����
		   bytes: pBuf�����ݳ���	
 * ���  ��RF24L01��status�Ĵ�����״̬
 * ����  ���ⲿ����
 */
u8 SPI_RF24L01_ReadBuf(u8 reg,u8 *pBuf,u8 bytes)
{
 	u8 sendBuff[64];
	u8 recvBuff[64];
	u8 i;
	sendBuff[0] = reg;
	
	RF24L01_CE_LOW();

	rt_spi_transfer(rt_spi_device,sendBuff,recvBuff,bytes + 1);
	for(i = 0; i < bytes; i ++)
		*pBuf++ = recvBuff[1+ i];
		
 	return recvBuff[0];		//���ؼĴ���״ֵ̬
}



/*
 * ��������SPI_RF24L01_WriteBuf
 * ����  ��������RF24L01�ļĴ�����д��һ������
 * ����  ��reg:RF24L01������+�Ĵ�����ַ��
 		   pBuf���洢�˽�Ҫд��д�Ĵ������ݵ����飬�ⲿ����
		   bytes: pBuf�����ݳ���	
 * ���  ��RF24L01��status�Ĵ�����״̬
 * ����  ���ⲿ����
 */
u8 SPI_RF24L01_WriteBuf(u8 reg ,u8 *pBuf,u8 bytes)
{
 	u8 sendBuff[64];
	u8 recvBuff[64];
	u8 i;
	
	sendBuff[0] = reg;
	for(i = 0; i < bytes; i++)
		sendBuff[i+1] = *(pBuf + i);
	
	RF24L01_CE_LOW();

	rt_spi_transfer(rt_spi_device,sendBuff,recvBuff,bytes + 1);

 	return recvBuff[0];		//���ؼĴ���״ֵ̬
}

/*
 * ��������void RF24L01_Init(void)_Check
 * ����  ����Ҫ����void RF24L01_Init(void)��MCU�Ƿ���������
 * ����  ����	
 * ���  ��SUCCESS/ERROR ��������/����ʧ��
 * ����  ���ⲿ����
 */
u8 RF24L01_Check(void)
{
	u8 buf[5]={0xC2,0xC2,0xC2,0xC2,0xC2};
	u8 buf1[5];
	u8 i; 
	 
	/*д��5���ֽڵĵ�ַ.  */  
	SPI_RF24L01_WriteBuf(RF24L01_WRITE_REG+TX_ADDR,buf,5);

	/*����д��ĵ�ַ */
	SPI_RF24L01_ReadBuf(TX_ADDR,buf1,5); 
	 
	/*�Ƚ�*/               
	for(i=0;i<5;i++)
	{
		if(buf1[i]!=0xC2)
		break;
	} 
	       
	if(i==5)
		return RT_EOK ;        //MCU��void RF24L01_Init(void)�ɹ����� 
	else
		return RT_ERROR ;        //MCU��void RF24L01_Init(void)����������
}

/*
 * ��������void RF24L01_Init(void)_TX_Mode
 * ����  �����÷���ģʽ
 * ����  ����	
 * ���  ����
 * ����  ���ⲿ����
 */
void RF24L01_TX_Mode(void)
{  
	RF24L01_CE_LOW();		

   SPI_RF24L01_WriteBuf(RF24L01_WRITE_REG+TX_ADDR,TX_ADDRESS,TX_ADR_WIDTH);    //дTX�ڵ��ַ 

   SPI_RF24L01_WriteBuf(RF24L01_WRITE_REG+RX_ADDR_P0,RX_ADDRESS,RX_ADR_WIDTH); //����TX�ڵ��ַ,��ҪΪ��ʹ��ACK   

   SPI_RF24L01_WriteReg(RF24L01_WRITE_REG+EN_AA,0x01);     //ʹ��ͨ��0���Զ�Ӧ��    

   SPI_RF24L01_WriteReg(RF24L01_WRITE_REG+EN_RXADDR,0x01); //ʹ��ͨ��0�Ľ��յ�ַ  

   SPI_RF24L01_WriteReg(RF24L01_WRITE_REG+SETUP_RETR,0x1a);//�����Զ��ط����ʱ��:500us + 86us;����Զ��ط�����:10��

   SPI_RF24L01_WriteReg(RF24L01_WRITE_REG+RF_CH,CHANAL);       //����RFͨ��ΪCHANAL

   SPI_RF24L01_WriteReg(RF24L01_WRITE_REG+RF_SETUP,0x07);  //����TX�������,0db����,1Mbps,���������濪��   
	
   SPI_RF24L01_WriteReg(RF24L01_WRITE_REG+CONFIG,0x0e);    //���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ,���������ж�

/*CE���ߣ����뷢��ģʽ*/	
  RF24L01_CE_HIGH();
}

 /*
 * ��������void RF24L01_Init(void)_Tx_Dat
 * ����  ��������void RF24L01_Init(void)�ķ��ͻ�������д������
 * ����  ��txBuf���洢�˽�Ҫ���͵����ݵ����飬�ⲿ����	
 * ���  �����ͽ�����ɹ�����TXDS,ʧ�ܷ���MAXRT��ERROR
 * ����  ���ⲿ����
 */ 
u8 RF24L01_Tx_Dat(u8 *txbuf)
{
	u8 state;  

	 /*ceΪ�ͣ��������ģʽ1*/
	RF24L01_CE_LOW();
	/*д���ݵ�TX BUF ��� 32���ֽ�*/						
   SPI_RF24L01_WriteBuf(WR_TX_PLOAD,txbuf,TX_PLOAD_WIDTH);
      /*CEΪ�ߣ�txbuf�ǿգ��������ݰ� */   
 	 RF24L01_CE_HIGH();
	  /*�ȴ���������ж� */                            
	while(RF24L01_Read_IRQ()!=0); 	
	/*��ȡ״̬�Ĵ�����ֵ */                              
	state = SPI_RF24L01_ReadReg(STATUS);
	 /*���TX_DS��MAX_RT�жϱ�־*/                  
	SPI_RF24L01_WriteReg(RF24L01_WRITE_REG+STATUS,state); 	
	SPI_RF24L01_WriteReg(FLUSH_TX,NOP);    //���TX FIFO�Ĵ��� 
	
	RF24L01_CE_LOW();
	
	 /*�ж��ж�����*/    
	if(state&MAX_RT)                     //�ﵽ����ط�����
			 return MAX_RT; 

	else if(state&TX_DS)                  //�������
		 	return TX_DS;
	 else						  
			return ERROR;                 //����ԭ����ʧ��
} 


/*
 * ��������void RF24L01_Init(void)_RX_Mode
 * ����  �����ò��������ģʽ
 * ����  ����	
 * ���  ����
 * ����  ���ⲿ����
 */
void RF24L01_RX_Mode(void)

{
	RF24L01_CE_LOW();	

	SPI_RF24L01_WriteBuf(RF24L01_WRITE_REG+RX_ADDR_P0,RX_ADDRESS,RX_ADR_WIDTH);//дRX�ڵ��ַ

	SPI_RF24L01_WriteReg(RF24L01_WRITE_REG+EN_AA,0x00);    //ʹ��ͨ��0���Զ�Ӧ��    

	SPI_RF24L01_WriteReg(RF24L01_WRITE_REG+EN_RXADDR,0x01);//ʹ��ͨ��0�Ľ��յ�ַ    

	SPI_RF24L01_WriteReg(RF24L01_WRITE_REG+RF_CH,CHANAL);      //����RFͨ��Ƶ��    

	SPI_RF24L01_WriteReg(RF24L01_WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);//ѡ��ͨ��0����Ч���ݿ��      

	SPI_RF24L01_WriteReg(RF24L01_WRITE_REG+RF_SETUP,0x07); //����TX�������,0db����,1Mbps,���������濪��   

	SPI_RF24L01_WriteReg(RF24L01_WRITE_REG+CONFIG, 0x0f);  //���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ 

	
/*CE���ߣ��������ģʽ*/	
  RF24L01_CE_HIGH();
}

 /*
 * ��������void RF24L01_Init(void)_Rx_Dat
 * ����  �����ڴ�void RF24L01_Init(void)�Ľ��ջ������ж�������
 * ����  ��rxBuf�����ڽ��ո����ݵ����飬�ⲿ����	
 * ���  �����ս����
 * ����  ���ⲿ����
 */ 
u8 RF24l01_Rx_Dat(u8 *rxbuf)
{
	u8 state; 
	RF24L01_CE_HIGH();	 //�������״̬
	 /*�ȴ������ж�*/
	while(RF24L01_Read_IRQ()!=0){
		rt_thread_delay(1);
	} 
	
	RF24L01_CE_LOW();  	 //�������״̬
	/*��ȡstatus�Ĵ�����ֵ  */               
	state=SPI_RF24L01_ReadReg(STATUS);
	 
	/* ����жϱ�־*/      
	SPI_RF24L01_WriteReg(RF24L01_WRITE_REG+STATUS,state);

	/*�ж��Ƿ���յ�����*/
	if(state&RX_DR)                                 //���յ�����
	{
	  SPI_RF24L01_ReadBuf(RD_RX_PLOAD,rxbuf,RX_PLOAD_WIDTH);//��ȡ����
	     SPI_RF24L01_WriteReg(FLUSH_RX,NOP);          //���RX FIFO�Ĵ���
	  return RX_DR; 
	}
	else    
		return ERROR;                    //û�յ��κ�����
}

//-------------------------------------Ӳ������--------------------------------------------------------------
static void RF24L01_IO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
   
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

  /*����SPI_RF24L01_SPI��CE���ţ�GPIOA^4*/
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

   /*����SPI_RF24L01_SPI��IRQ���ţ�GPIOC^5*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU ;  //��������
  GPIO_Init(GPIOC, &GPIO_InitStructure); 
}


/*
	��Ҫ��SPI BUS���渽��һ��SPI�豸������ͨ�š���������Ҫʵ�ַ��ͺͽ��ա�
*/
rt_err_t nRF24L01_init(const char * spi_device_name){
	RF24L01_IO_Init();
	
	rt_spi_device = (struct rt_spi_device *)rt_device_find(spi_device_name);
	if(rt_spi_device == RT_NULL)
	{
			FLASH_TRACE("spi device %s not found!\r\n", spi_device_name);
			return -RT_ENOSYS;
	}

	/* config spi */
	{
			struct rt_spi_configuration cfg;
			cfg.data_width = 8;
			cfg.mode = RT_SPI_MODE_0 | RT_SPI_MSB; /* SPI Compatible: Mode 0 and Mode 3 */
			cfg.max_hz = 9 * 1000 * 1000; /* 9M */
			//cfg.max_hz = 50 * 1000 * 1000; /* 9M */
			rt_spi_configure(rt_spi_device, &cfg);
	}
//	SPI_RF24L01_RW(0x5a);	
	
	if(RF24L01_Check() == RT_EOK) {
		//����ֱ����һ���߳�������������ݡ�
		rt_kprintf("nRF24L01 is connected !\n");
		return	RT_EOK;
	}	else {
		rt_kprintf("nRF24L01 is not connected !\n");
		return RT_ERROR ;        //MCU��void RF24L01_Init(void)����������		
	}
	
}

//#ifdef RT_USING_FINSH
//#include <finsh.h>

//u8 spiTest(u8 value)
//{
//	return SPI_RF24L01_RW(value);
//}
//FINSH_FUNCTION_EXPORT(spiTest, send then recv a byte.)
////FINSH_FUNCTION_EXPORT_ALIAS(led, ld, set led[0 - 1] on[1] or off[0].)
////FINSH_VAR_EXPORT(led_inited, finsh_type_uchar, just a led_inited for test)
//#endif

