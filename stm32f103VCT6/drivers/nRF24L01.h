#ifndef __SPI_RF24L01_H
#define __SPI_RF24L01_H

#include "stm32f10x.h"
#include <rtthread.h>


#define TX_ADR_WIDTH 	5  	//�����ַ����
#define TX_PLOAD_WIDTH  32   //��������ͨ����Ч���ݿ���0~32Byte 

#define RX_ADR_WIDTH    5
#define RX_PLOAD_WIDTH  32

#define CHANAL 0x20	//Ƶ��ѡ�� 

// SPI(nRF24L01) commands ,	RF24L01��SPI����궨�壬���NRF����ʹ���ĵ�
#define RF24L01_READ_REG    0x00  // Define read command to register
#define RF24L01_WRITE_REG   0x20  // Define write command to register
#define RD_RX_PLOAD 0x61  // Define RX payload register address
#define WR_TX_PLOAD 0xA0  // Define TX payload register address
#define FLUSH_TX    0xE1  // Define flush TX register command
#define FLUSH_RX    0xE2  // Define flush RX register command
#define REUSE_TX_PL 0xE3  // Define reuse TX payload register command
#define NOP         0xFF  // Define No Operation, might be used to read status register

// SPI(nRF24L01) registers(addresses) ��RF24L01 ��ؼĴ�����ַ�ĺ궨��
#define CONFIG      0x00  // 'Config' register address
#define EN_AA       0x01  // 'Enable Auto Acknowledgment' register address
#define EN_RXADDR   0x02  // 'Enabled RX addresses' register address
#define SETUP_AW    0x03  // 'Setup address width' register address
#define SETUP_RETR  0x04  // 'Setup Auto. Retrans' register address
#define RF_CH       0x05  // 'RF channel' register address
#define RF_SETUP    0x06  // 'RF setup' register address
#define STATUS      0x07  // 'Status' register address
#define OBSERVE_TX  0x08  // 'Observe TX' register address
#define CD          0x09  // 'Carrier Detect' register address
#define RX_ADDR_P0  0x0A  // 'RX address pipe0' register address
#define RX_ADDR_P1  0x0B  // 'RX address pipe1' register address
#define RX_ADDR_P2  0x0C  // 'RX address pipe2' register address
#define RX_ADDR_P3  0x0D  // 'RX address pipe3' register address
#define RX_ADDR_P4  0x0E  // 'RX address pipe4' register address
#define RX_ADDR_P5  0x0F  // 'RX address pipe5' register address
#define TX_ADDR     0x10  // 'TX address' register address
#define RX_PW_P0    0x11  // 'RX payload width, pipe0' register address
#define RX_PW_P1    0x12  // 'RX payload width, pipe1' register address
#define RX_PW_P2    0x13  // 'RX payload width, pipe2' register address
#define RX_PW_P3    0x14  // 'RX payload width, pipe3' register address
#define RX_PW_P4    0x15  // 'RX payload width, pipe4' register address
#define RX_PW_P5    0x16  // 'RX payload width, pipe5' register address
#define FIFO_STATUS 0x17  // 'FIFO Status Register' register address

#define MAX_RT      0x10 //�ﵽ����ط������жϱ�־λ
#define TX_DS		0x20 //��������жϱ�־λ	  // 

#define RX_DR		0x40 //���յ������жϱ�־λ

/* nRF24L01+�������� */
#define RF24L01_CSN_HIGH()      GPIO_SetBits(GPIOA, GPIO_Pin_4)
#define RF24L01_CSN_LOW()      GPIO_ResetBits(GPIOA, GPIO_Pin_4)		 //csn�õ�
#define RF24L01_CE_HIGH()	   GPIO_SetBits(GPIOB,GPIO_Pin_1)
#define RF24L01_CE_LOW()	   GPIO_ResetBits(GPIOB,GPIO_Pin_1)			  //CE�õ�
#define RF24L01_Read_IRQ()		GPIO_ReadInputDataBit ( GPIOB, GPIO_Pin_0) //�ж�����

rt_err_t nRF24L01_init(const char * spi_device_name);
void RF24L01_TX_Mode(void);
void RF24L01_RX_Mode(void);
u8 RF24l01_Rx_Dat(u8 *rxbuf);
u8 RF24L01_Tx_Dat(u8 *txbuf);
u8 RF24L01_Check(void); 

#endif /* __SPI_RF24L01_H */




















