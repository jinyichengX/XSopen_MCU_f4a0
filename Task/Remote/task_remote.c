#include "task_remote.h"
#include "led.h"
#include "bsp_uart.h"

extern EventGroupHandle_t EventGroup1;
extern EventGroupHandle_t EventGroup2;
extern EventGroupHandle_t EventGroup3;
extern EventGroupHandle_t EventGroup4;

void vTaskUart1(void *para)
{
	/* 设置事件标志超时时间500，如果设置为portMAX_DELAY表示无限等待下去 */
	TickType_t xTicksToWait = 500;
	
	while(1)
	{
		BaseType_t xResult;
		
		xResult = xEventGroupWaitBits(EventGroup1,EventGroup1_BIT0,pdTRUE,pdTRUE,xTicksToWait);//portMAX_DELAY
		
		if(xResult == EventGroup1_BIT0)
		{
			char *ret_buffer[2] = {"I have received your msg,thank you!","Oh no,i am joking!"};
			usart1.Uart_Handler.fptr_com_send_dma(&usart1.Uart_cfg_Info,(uint8_t *)ret_buffer[1],strlen(ret_buffer[1]));
		}
		
		vTaskDelay(1000);
	}
}

