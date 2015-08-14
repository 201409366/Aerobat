#include <board.h>
#include <rtthread.h>
#include "nRF24L01.h"
#include "led.h"

static void app_2401_thread_entry(void* parameter)
{
		u8 rxbuf[RX_PLOAD_WIDTH+1];		//�������ݻ���
//		u8 txbuf[TX_PLOAD_WIDTH+1];		//�������ݻ���
		u8 status;
	
    nRF24L01_init("nRF24L01");
		
		if(RF24L01_Check() != 0)
			rt_kprintf("nRF24L01 is connected !\r\n");
		else {
			rt_kprintf("nRF24L01 is not connected !\r\n");
			return;
		}
		
    while (1)
    {
			RF24L01_RX_Mode();
			status = RF24l01_Rx_Dat(rxbuf);
			switch(status)
			{
			case ERROR:
				rt_kprintf("\r\n δ֪ԭ���½���ʧ�ܡ� \r\n");
				break;

			case RX_DR:
				//printf("\r\n ������ ���յ� �ӻ��� ��Ӧ���źţ����ͳɹ��� \r\n");	
				rxbuf[32] = 0;
				rt_kprintf("\r\n Receive data is:%s \r\n",rxbuf);//��ӡ���յ�������			 
				rt_hw_led_on(1);
				break;  								
			}	
		}			
}

void rt_hw_2401_init(void) {
	rt_thread_t tid;
	tid = rt_thread_create("2401",
												 app_2401_thread_entry, RT_NULL,
												 2048, 8, 20);
	if (tid != RT_NULL)
        rt_thread_startup(tid);
}

