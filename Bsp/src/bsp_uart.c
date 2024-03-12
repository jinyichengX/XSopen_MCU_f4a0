#include "bsp_uart.h"
#include "led.h"
#include "bsp_timer0.h"
#include "bsp_dma.h"

#include "task_remote.h"

extern EventGroupHandle_t EventGroup1;
extern EventGroupHandle_t EventGroup2;
extern EventGroupHandle_t EventGroup3;
extern EventGroupHandle_t EventGroup4;

#if PRINT_DEBUG_INFO_EN

/* 告知连接器不从C库链接使用半主机的函数 */
#pragma import(__use_no_semihosting)

/* 标准库需要的支持类型 */
struct __FILE
{
    int handle;
};

FILE __stdout;
FILE __stdin;
/* 定义 _sys_exit() 以避免使用半主机模式 */
void _sys_exit(int x)
{
    x = x;
}
#endif

void _ttywrch(int ch)
{
	ch = ch;
}

//声明函数
void usart_Init(Usartx_Typedef * usart_dev_t);
void usart_Send(Usartx_Typedef * usart_dev_t,uint8_t *pxInd,uint32_t s_len,usart_tmode);

usart_t g_uart = {
	.fp_setup = usart_Init,
	.fp_send = usart_Send,
};


#if UART1_EN
/**
 * @brief  USART1 RX IRQ callback
 * @param  None
 * @retval None
 */
static void USART1_RxFull_IrqCallback(void)
{
		if (SET == USART_GetStatus(usart1.Uart_cfg_Info.Usart_Unit, USART_FLAG_RX_FULL )) {
			/* usr interrupt processing code begin */

			usart1.Uart_cfg_Info.rx_buf[usart1.Uart_cfg_Info.rx_len++] = (uint8_t)USART_ReadData(CM_USART1);
			/* usr interrupt processing code end */
		}
}

static void USART1_RxTimeOut_IrqCallback(void)
{
	
		if (SET == USART_GetStatus(CM_USART1, USART_FLAG_RX_TIMEOUT )) {
			/* usr interrupt processing code begin */

			USART_ClearStatus(CM_USART1, USART_FLAG_RX_TIMEOUT);
			TMR0_Stop(CM_TMR0_1, TMR0_CH_A);
			usart1.Uart_cfg_Info.rx_len = (uint32_t)(usart1.Uart_cfg_Info.Linked_DMABase->MONDAR0) \
																+ (0x40 * usart1.Uart_cfg_Info.Linked_DMA_rxChannel)\
																- (uint32_t)&usart1.Uart_cfg_Info.rx_buf;
			DMA_Reload_Dest_For_Uart(&usart1.Uart_cfg_Info);

			/* usr interrupt processing code end */
		}
		
		BaseType_t xResult;
		BaseType_t xHigherPriorityTaskWoken = pdFALSE;
		/* 向任务 vTaskMsgPro 发送事件标志 */
		xResult = xEventGroupSetBitsFromISR(EventGroup1, /* 事件标志组句柄 */
																				EventGroup1_BIT0 , /* 设置 bit0 */
																				&xHigherPriorityTaskWoken );
		/* 消息被成功发出 */
		if( xResult != pdFAIL )
		{
			/* 如果 xHigherPriorityTaskWoken = pdTRUE，那么退出中断后切到当前最高优先级任务执行 */
			portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
		}
}
/**
 * @brief  USART1 error IRQ callback.
 * @param  None
 * @retval None
 */
static void USART1_RxError_IrqCallback(void)
{
    if (SET == USART_GetStatus(CM_USART1, (USART_FLAG_PARITY_ERR | USART_FLAG_FRAME_ERR))) {
			/* usr interrupt processing code begin */

      (void)USART_ReadData(CM_USART1);
					
			/* usr interrupt processing code end */
    }
    USART_ClearStatus(CM_USART1, (USART_FLAG_PARITY_ERR | USART_FLAG_FRAME_ERR | USART_FLAG_OVERRUN));
		
}

/* 在串口使用DMA发送时，下面两个中断回调函数缺一不可 */
/**
 * @brief  DMA Transmit complete IRQ callback.
 * @param  None
 * @retval None
 */
void U1_TX_DMA_TC_IrqCallback(void)
{
	/* 使能串口发送完成中断 */
	USART_FuncCmd(usart1.Uart_cfg_Info.Usart_Unit, USART_INT_TX_CPLT, ENABLE);
	/* 停止DMA对应通道 */
	(void)DMA_ChCmd(usart1.Uart_cfg_Info.Linked_DMABase, usart1.Uart_cfg_Info.Linked_DMA_txChannel, DISABLE);
	/* 清除DMA 发送完成标志位 */
  DMA_ClearTransCompleteStatus(usart1.Uart_cfg_Info.Linked_DMABase, DMA_FLAG_TC_CH1);
	
}

/**
 * @brief  USART1 Transmit complete IRQ callback.
 * @param  None
 * @retval None
 */
void U1_TC_IrqCallback(void)
{
	/* 失能发送和发送完成中断 */
	USART_FuncCmd(usart1.Uart_cfg_Info.Usart_Unit, (USART_TX | USART_INT_TX_CPLT), DISABLE);
	/* 清除发送完成标志位 */
	USART_ClearStatus(usart1.Uart_cfg_Info.Usart_Unit, USART_FLAG_TX_CPLT);
}
#endif

#if UART2_EN
/**
 * @brief  USART2 RX IRQ callback
 * @param  None
 * @retval None
 */
static void USART2_RxFull_IrqCallback(void)
{
		if (SET == USART_GetStatus(usart2.Uart_cfg_Info.Usart_Unit, USART_FLAG_RX_FULL )) {
			/* usr interrupt processing code begin */

			usart2.Uart_cfg_Info.rx_buf[usart2.Uart_cfg_Info.rx_len++] = (uint8_t)USART_ReadData(CM_USART2);
			/* usr interrupt processing code end */
		}
}

static void USART2_RxTimeOut_IrqCallback(void)
{
		if (SET == USART_GetStatus(CM_USART2, USART_FLAG_RX_TIMEOUT )) {
			/* usr interrupt processing code begin */

			USART_ClearStatus(CM_USART2, USART_FLAG_RX_TIMEOUT);
			TMR0_Stop(CM_TMR0_1, TMR0_CH_B);
			DMA_Reload_For_Uart(&usart2.Uart_cfg_Info);
			/* usr interrupt processing code end */
		}
}
/**
 * @brief  USART2 error IRQ callback.
 * @param  None
 * @retval None
 */
static void USART2_RxError_IrqCallback(void)
{
    if (SET == USART_GetStatus(CM_USART2, (USART_FLAG_PARITY_ERR | USART_FLAG_FRAME_ERR))) {
			/* usr interrupt processing code begin */

      (void)USART_ReadData(CM_USART2);
					
			/* usr interrupt processing code end */
    }
    USART_ClearStatus(CM_USART2, (USART_FLAG_PARITY_ERR | USART_FLAG_FRAME_ERR | USART_FLAG_OVERRUN));
}

/* 在串口使用DMA发送时，下面两个中断回调函数缺一不可 */
/**
 * @brief  DMA Transmit complete IRQ callback.
 * @param  None
 * @retval None
 */
void U2_TX_DMA_TC_IrqCallback(void)
{
	/* 使能串口发送完成中断 */
	USART_FuncCmd(usart2.Uart_cfg_Info.Usart_Unit, USART_INT_TX_CPLT, ENABLE);
	/* 停止DMA对应通道 */
	(void)DMA_ChCmd(usart2.Uart_cfg_Info.Linked_DMABase, usart2.Uart_cfg_Info.Linked_DMA_txChannel, DISABLE);
	/* 清除DMA 发送完成标志位 */
  DMA_ClearTransCompleteStatus(usart2.Uart_cfg_Info.Linked_DMABase, DMA_FLAG_TC_CH1);
	
}
/**
 * @brief  USART2 Transmit complete IRQ callback.
 * @param  None
 * @retval None
 */
void U2_TC_IrqCallback(void)
{
	/* 失能发送和发送完成中断 */
	USART_FuncCmd(usart2.Uart_cfg_Info.Usart_Unit, (USART_TX | USART_INT_TX_CPLT), DISABLE);
	/* 清除发送完成标志位 */
	USART_ClearStatus(usart2.Uart_cfg_Info.Usart_Unit, USART_FLAG_TX_CPLT);
}
#endif

#if UART6_EN
/**
 * @brief  USART6 RX IRQ callback
 * @param  None
 * @retval None
 */
static void USART6_RxFull_IrqCallback(void)
{
		if (SET == USART_GetStatus(CM_USART6, USART_FLAG_RX_FULL )) {
			/* usr interrupt processing code begin */

			usart6.Uart_cfg_Info.rx_buf[usart6.Uart_cfg_Info.rx_len++] = (uint8_t)USART_ReadData(CM_USART6);
			
			/* usr interrupt processing code end */
		}
		
}
static void USART6_RxTimeOut_IrqCallback(void)
{
		if (SET == USART_GetStatus(CM_USART6, USART_FLAG_RX_TIMEOUT )) {
			/* usr interrupt processing code begin */

			USART_ClearStatus(CM_USART6, USART_FLAG_RX_TIMEOUT);
			TMR0_Stop(CM_TMR0_2, TMR0_CH_A);
			DMA_Reload_For_Uart(&usart6.Uart_cfg_Info);
			/* usr interrupt processing code end */
		}
}
/**
 * @brief  USART6 error IRQ callback.
 * @param  None
 * @retval None
 */
static void USART6_RxError_IrqCallback(void)
{
    if (SET == USART_GetStatus(CM_USART6, (USART_FLAG_PARITY_ERR | USART_FLAG_FRAME_ERR))) {
			/* usr interrupt processing code begin */
			
      (void)USART_ReadData(CM_USART6);
			
			/* usr interrupt processing code end */
    }
    USART_ClearStatus(CM_USART6, (USART_FLAG_PARITY_ERR | USART_FLAG_FRAME_ERR | USART_FLAG_OVERRUN));
		
}
#endif

#if UART10_EN
void U10_TX_DMA_TC_IrqCallback(void)
{
		/* 停止发送 */
		USART_FuncCmd(usart10.Uart_cfg_Info.Usart_Unit, USART_TX, DISABLE);
		/* 停止DMA */
//		(void)DMA_ChCmd(TX_DMA_UNIT, TX_DMA_CH, DISABLE);
		/* 清除DMA 发送完成标志位 */
    DMA_ClearTransCompleteStatus(usart10.Uart_cfg_Info.Linked_DMABase, DMA_FLAG_TC_CH1);
	
}
#endif
#if UART1_EN
//用户修改usart1~usart10
Usartx_Typedef usart1 = {
	.Uart_cfg_Info = {
		.Usart_Unit = CM_USART1,																	//串口实例，用户不可更改
		
		.FCGx_Periph = FCG3_PERIPH_USART1,												//串口时钟配置，用户不可更改
		
		.RxPortSel = GPIO_PORT_B,																	//串口接收功能映射的Port选择（参考数据手册），用户可更改
		.RxPinSel = GPIO_PIN_13,																	//串口接收功能映射的Pin选择（参考数据手册），用户可更改
		.USART_RX_GPIO_FUNC = GPIO_FUNC_33,												//IO复用功能选择（参考数据手册），用户可更改

		.TxPortSel = GPIO_PORT_B,																	//串口发送功能映射的Port选择（参考数据手册），用户可更改
		.TxPinSel = GPIO_PIN_12,																	//串口发送功能映射的Pin选择（参考数据手册），用户可更改
		.USART_TX_GPIO_FUNC = GPIO_FUNC_32,												//IO复用功能选择（参考数据手册），用户可更改
		
		.stcUartInit = {.u32ClockDiv = USART_CLK_DIV64,						//时钟分频，用户可更改
			.u32Baudrate = 115200,																	//波特率，用户可更改
			.u32OverSampleBit = USART_OVER_SAMPLE_8BIT,							//过采样倍率,不建议更改
			.u32StartBitPolarity = USART_START_BIT_LOW,							
		},
		
		.stcIrqSigninConfig[USART_MAX_IRQ_NUM - USART_MAX_IRQ_NUM] = {.enIRQn = INT000_IRQn,//中断号，用户可更改
			.enIntSrc = INT_SRC_USART1_EI,																//中断源，用户不可更改
			.pfnCallback = &USART1_RxError_IrqCallback,										//中断回调函数，用户可更改
		},
		
			.stcIrqSigninConfig[USART_MAX_IRQ_NUM - USART_MAX_IRQ_NUM +1] = {.enIRQn = INT001_IRQn,	//中断号，用户可更改
			.enIntSrc = INT_SRC_USART1_RI,																		//中断源，用户不可更改
			.pfnCallback = &USART1_RxFull_IrqCallback,											//中断回调函数，用户可更改
		},
			.stcIrqSigninConfig[USART_MAX_IRQ_NUM - USART_MAX_IRQ_NUM +2] = {.enIRQn = INT002_IRQn,	//中断号，用户可更改
			.enIntSrc = INT_SRC_USART1_RTO,																		//中断源，用户不可更改
			.pfnCallback = &USART1_RxTimeOut_IrqCallback,											//中断回调函数，用户可更改
		},
			.stcIrqSigninConfig[USART_MAX_IRQ_NUM - USART_MAX_IRQ_NUM +3] = {.enIRQn = INT003_IRQn,	//中断号，用户可更改
			.enIntSrc = INT_SRC_USART1_TCI,																		//中断源，用户不可更改
			.pfnCallback = &U1_TC_IrqCallback,											//中断回调函数，用户可更改
		},
		.Usart_Rx_Mode = USART_RX_MODE_DMA,				//串口接收数据接收方式选择（rx中断/DMA+timeout中断），用户可更改
		.IRQ_Prio = DDL_IRQ_PRIO_DEFAULT,					//中断优先级，用户可更改
		.usart_tx_timeoout_time = 200000,					//发送超时时间，根据发送数据量和系统时钟设定，用户可更改
		.usart_rx_timeoout_time = 100,						//接收超时时间，根据接收数据量和波特率设定，用户可更改
		.TimerBase = CM_TMR0_1,										//硬件决定，用户不可更改
		.TimerChannel = TMR0_CH_A,								//硬件决定，用户不可更改
		
		.Linked_DMABase = CM_DMA1,								//DMA选择，用户可更改
		.Linked_DMA_rxChannel = DMA_CH0,						//DMA通道选择，用户可更改
		.DMA_Trig_rxSource = EVT_SRC_USART1_RI,			//事件请求序号唯一，用户不可更改
		
		.Linked_DMA_txChannel = DMA_CH1,				//DMA通道选择，用户可更改
		.DMA_Trig_txSource = EVT_SRC_USART1_TI,			//事件请求序号唯一，用户不可更改
		.tx_dma_tc_enIRQn = INT004_IRQn,
		.TIMEOUT_ALLOWED = true,									//是否允许超时接收中断，硬件决定，用户不可更改
	},
};
#endif

#if UART2_EN
Usartx_Typedef usart2 = {
	.Uart_cfg_Info = {
		.Usart_Unit = CM_USART2,																	//串口实例，用户不可更改
		
		.FCGx_Periph = FCG3_PERIPH_USART2,												//串口时钟配置，用户不可更改
		
		.RxPortSel = GPIO_PORT_I,																	//串口接收功能映射的Port选择（参考数据手册），用户可更改
		.RxPinSel = GPIO_PIN_06,																	//串口接收功能映射的Pin选择（参考数据手册），用户可更改
		.USART_RX_GPIO_FUNC = GPIO_FUNC_35,												//IO复用功能选择（参考数据手册），用户可更改

		.TxPortSel = GPIO_PORT_I,																	//串口发送功能映射的Port选择（参考数据手册），用户可更改
		.TxPinSel = GPIO_PIN_07,																	//串口发送功能映射的Pin选择（参考数据手册），用户可更改
		.USART_TX_GPIO_FUNC = GPIO_FUNC_34,												//IO复用功能选择（参考数据手册），用户可更改
		
		.stcUartInit = {.u32ClockDiv = USART_CLK_DIV64,						//时钟分频，用户可更改
			.u32Baudrate = 115200,																	//波特率，用户可更改
			.u32OverSampleBit = USART_OVER_SAMPLE_8BIT,							//过采样倍率,不建议更改
			.u32StartBitPolarity = USART_START_BIT_LOW,							
		},
		
		.stcIrqSigninConfig[USART_MAX_IRQ_NUM - USART_MAX_IRQ_NUM] = {.enIRQn = INT005_IRQn,//中断号，用户可更改
			.enIntSrc = INT_SRC_USART2_EI,																//中断源，用户不可更改
			.pfnCallback = &USART2_RxError_IrqCallback,										//中断回调函数，用户可更改
		},
		
			.stcIrqSigninConfig[USART_MAX_IRQ_NUM - USART_MAX_IRQ_NUM +1] = {.enIRQn = INT006_IRQn,	//中断号，用户可更改
			.enIntSrc = INT_SRC_USART2_RI,																		//中断源，用户不可更改
			.pfnCallback = &USART2_RxFull_IrqCallback,											//中断回调函数，用户可更改
		},
			.stcIrqSigninConfig[USART_MAX_IRQ_NUM - USART_MAX_IRQ_NUM +2] = {.enIRQn = INT007_IRQn,	//中断号，用户可更改
			.enIntSrc = INT_SRC_USART2_RTO,																		//中断源，用户不可更改
			.pfnCallback = &USART2_RxTimeOut_IrqCallback,											//中断回调函数，用户可更改
		},
			.stcIrqSigninConfig[USART_MAX_IRQ_NUM - USART_MAX_IRQ_NUM +3] = {.enIRQn = INT008_IRQn,	//中断号，用户可更改
			.enIntSrc = INT_SRC_USART2_TCI,																		//中断源，用户不可更改
			.pfnCallback = &U2_TC_IrqCallback,											//中断回调函数，用户可更改
		},
		.Usart_Rx_Mode = USART_RX_MODE_DMA,				//串口接收数据接收方式选择（rx中断/DMA+timeout中断），用户可更改
		.IRQ_Prio = DDL_IRQ_PRIO_DEFAULT,					//中断优先级，用户可更改
		.usart_tx_timeoout_time = 200000,					//发送超时时间，根据发送数据量和系统时钟设定，用户可更改
		.usart_rx_timeoout_time = 2000,						//接收超时时间，根据接收数据量和波特率设定，用户可更改
		.TimerBase = CM_TMR0_1,										//硬件决定，用户不可更改
		.TimerChannel = TMR0_CH_B,								//硬件决定，用户不可更改
		
		.Linked_DMABase = CM_DMA1,								//DMA选择，用户可更改
		.Linked_DMA_rxChannel = DMA_CH2,						//DMA通道选择，用户可更改
		.DMA_Trig_rxSource = EVT_SRC_USART2_RI,			//事件请求序号唯一，用户不可更改
		
		.Linked_DMA_txChannel = DMA_CH3,				//DMA通道选择，用户可更改
		.DMA_Trig_txSource = EVT_SRC_USART2_TI,			//事件请求序号唯一，用户不可更改
		.tx_dma_tc_enIRQn = INT009_IRQn,
		
		.TIMEOUT_ALLOWED = true,									//是否允许超时接收中断，硬件决定，用户不可更改
	},

};
#endif

#if UART6_EN
Usartx_Typedef usart6 = {
		.Uart_cfg_Info = {
		.Usart_Unit = CM_USART6,																	//串口实例，用户不可更改
		
		.FCGx_Periph = FCG3_PERIPH_USART6,												//串口时钟配置，用户不可更改
		
		.RxPortSel = GPIO_PORT_E,																	//串口接收功能映射的Port选择（参考数据手册）
		.RxPinSel = GPIO_PIN_03,																	//串口接收功能映射的Pin选择（参考数据手册）
		.USART_RX_GPIO_FUNC = GPIO_FUNC_37,												//IO复用功能选择（参考数据手册）

		.TxPortSel = GPIO_PORT_E,																	//串口发送功能映射的Port选择（参考数据手册）
		.TxPinSel = GPIO_PIN_05,																	//串口发送功能映射的Pin选择（参考数据手册）
		.USART_TX_GPIO_FUNC = GPIO_FUNC_36,												//IO复用功能选择（参考数据手册）
		
		.stcUartInit = {
			.u32ClockDiv = USART_CLK_DIV64,													//时钟分频，用户可更改
			.u32Baudrate = 115200,																	//波特率，用户可更改
			.u32OverSampleBit = USART_OVER_SAMPLE_8BIT,							//过采样倍率,不建议更改
			.u32StartBitPolarity = USART_START_BIT_LOW,
		},
		
		.stcIrqSigninConfig[USART_MAX_IRQ_NUM - USART_MAX_IRQ_NUM] = {
			.enIRQn = INT002_IRQn,																				//中断号，用户可更改
			.enIntSrc = INT_SRC_USART6_EI,																//中断源，用户不可更改
			.pfnCallback = &USART6_RxError_IrqCallback,										//中断回调函数，用户可更改
		},
		
		.stcIrqSigninConfig[USART_MAX_IRQ_NUM - USART_MAX_IRQ_NUM +1] = {
			.enIRQn = INT003_IRQn,																					//中断号，用户可更改
			.enIntSrc = INT_SRC_USART6_RI,																		//中断源，用户不可更改
			.pfnCallback = &USART6_RxFull_IrqCallback,											//中断回调函数，用户可更改
		},
			.stcIrqSigninConfig[USART_MAX_IRQ_NUM - USART_MAX_IRQ_NUM +2] = {.enIRQn = INT002_IRQn,	//中断号，用户可更改
			.enIntSrc = INT_SRC_USART6_RTO,																		//中断源，用户不可更改
			.pfnCallback = &USART6_RxTimeOut_IrqCallback,											//中断回调函数，用户可更改
		},
		.Usart_Rx_Mode = USART_RX_MODE_IRQ,				//串口接收数据接收方式选择（rx中断/DMA+timeout中断），用户可更改
		.IRQ_Prio = DDL_IRQ_PRIO_DEFAULT,					//中断优先级，用户可更改
		.usart_tx_timeoout_time = 200000,					//发送超时时间，根据发送数据量和系统时钟设定，用户可更改
		.usart_rx_timeoout_time = 5000,						//接收超时时间，根据接收数据量和波特率设定，用户可更改
		
		.TimerBase = CM_TMR0_2,										//硬件决定，用户不可更改
		.TimerChannel = TMR0_CH_A,								//硬件决定，用户不可更改
		
		.Linked_DMABase = CM_DMA1,								//DMA选择，用户可更改
		.Linked_DMA_rxChannel = DMA_CH2,						//DMA通道选择，用户可更改
		.DMA_Trig_rxSource = EVT_SRC_USART6_RI,			//事件请求序号唯一，用户不可更改
		.TIMEOUT_ALLOWED = true,									//是否允许超时接收中断，硬件决定，用户不可更改
	},
};
#endif

#if UART10_EN
Usartx_Typedef usart10 = {
		.Uart_cfg_Info = {
		.Usart_Unit = CM_USART10,																	//串口实例，用户不可更改
		
		.FCGx_Periph = FCG3_PERIPH_USART10,												//串口时钟配置，用户不可更改
		
		.RxPortSel = GPIO_PORT_D,																	//串口接收功能映射的Port选择（参考数据手册）
		.RxPinSel = GPIO_PIN_02,																	//串口接收功能映射的Pin选择（参考数据手册）
		.USART_RX_GPIO_FUNC = GPIO_FUNC_39,												//IO复用功能选择（参考数据手册）

		.TxPortSel = GPIO_PORT_D,																	//串口发送功能映射的Port选择（参考数据手册）
		.TxPinSel = GPIO_PIN_03,																	//串口发送功能映射的Pin选择（参考数据手册）
		.USART_TX_GPIO_FUNC = GPIO_FUNC_38,												//IO复用功能选择（参考数据手册）
		
		.stcUartInit = {
			.u32ClockDiv = USART_CLK_DIV64,													//时钟分频，用户可更改
			.u32Baudrate = 9600,																	//波特率，用户可更改
			.u32OverSampleBit = USART_OVER_SAMPLE_8BIT,							//过采样倍率,不建议更改
			.u32StartBitPolarity = USART_START_BIT_LOW,
		},
		
		.usart_tx_timeoout_time = 200000,					//发送超时时间，根据发送数据量和系统时钟设定，用户可更改
		.TIMEOUT_ALLOWED = false,									//是否允许超时接收中断，硬件决定，用户不可更改
	},
};
#endif

static void INTC_IrqInstalHandler(const stc_irq_signin_config_t *pstcConfig, uint32_t u32Priority)
{
    if (NULL != pstcConfig) {
        (void)INTC_IrqSignIn(pstcConfig);
        NVIC_ClearPendingIRQ(pstcConfig->enIRQn);
        NVIC_SetPriority(pstcConfig->enIRQn, u32Priority);
        NVIC_EnableIRQ(pstcConfig->enIRQn);
    }
}

static void Usart_ObjInit(Usart_cfgInfo_Typedef * Usart_cfgInfo)
{
	uint8_t i;
	
	/* Enable usart peripheral Clock */
	FCG_Fcg3PeriphClockCmd(Usart_cfgInfo->FCGx_Periph, ENABLE);
	
	GPIO_SetFunc(Usart_cfgInfo->TxPortSel, Usart_cfgInfo->TxPinSel, Usart_cfgInfo->USART_TX_GPIO_FUNC);
	GPIO_SetFunc(Usart_cfgInfo->RxPortSel, Usart_cfgInfo->RxPinSel, Usart_cfgInfo->USART_RX_GPIO_FUNC);
	
#ifdef TMR0_FUNC_USART_TIMEOUT
	if(Usart_cfgInfo->TIMEOUT_ALLOWED == true)
		Usart_cfgInfo->stcUartInit.u32CKOutput = USART_CK_OUTPUT_ENABLE; //<<使能内部时钟源并设置为timer0输入
#endif
	if (LL_OK != USART_UART_Init(Usart_cfgInfo->Usart_Unit, &Usart_cfgInfo->stcUartInit, NULL)) {
			for (;;) {
			}
	}
	
	/* Enable usart IRQs */
	for(i = 0;i< USART_MAX_IRQ_NUM;i++){
			INTC_IrqInstalHandler(&Usart_cfgInfo->stcIrqSigninConfig[i], Usart_cfgInfo->IRQ_Prio);
	}
	
	/* Enable RX and RXIE function */
  USART_FuncCmd(Usart_cfgInfo->Usart_Unit, USART_RX, ENABLE);
	if(Usart_cfgInfo->Usart_Rx_Mode == USART_RX_MODE_IRQ){
		USART_FuncCmd(Usart_cfgInfo->Usart_Unit, USART_INT_RX, ENABLE);
	}else if(Usart_cfgInfo->Usart_Rx_Mode == USART_RX_MODE_DMA){
#ifdef TMR0_FUNC_USART_TIMEOUT
		//配置DMA,DMA配合timeout功能使用
		if(Usart_cfgInfo->TIMEOUT_ALLOWED == true){
			DMA_Config_For_Uart(Usart_cfgInfo);
		}
#endif
	}
	
#ifdef TMR0_FUNC_USART_TIMEOUT
	if(Usart_cfgInfo->TIMEOUT_ALLOWED == true){
		/* Enable RX Timeout and IRQ function */
		USART_FuncCmd(Usart_cfgInfo->Usart_Unit, USART_RX_TIMEOUT, ENABLE);
		USART_FuncCmd(Usart_cfgInfo->Usart_Unit, USART_INT_RX_TIMEOUT, ENABLE);
		
		/* 配置timer0 */
		BSP_TMR0_Config_For_UsartTimeOut(Usart_cfgInfo);
	}
	
#endif
}

/**
 * @brief  Usart send data（串口发送数据（阻塞方式,超时退出））
 * @param  Usart_cfgInfo_Typedef *（指向串口实例的指针）,uint8_t *（指向待发送的数据指针）,uint16_t（数据长度）,USART_TIMEOUT_TIME_TYPE（超时时间）
 * @retval None
 */
static Usart_SendFlag_Typedef Usart_Send_Pending(Usart_cfgInfo_Typedef * Usartx_cfgInfo,uint8_t *pxInd,uint32_t s_len,USART_TIMEOUT_TIME_TYPE s_timeout)
{
		s_len -= 1;
		*(USART_TIMEOUT_TIME_TYPE *)s_timeout = Usartx_cfgInfo->usart_tx_timeoout_time;
	
		if(Usartx_cfgInfo->Usart_Unit == NULL){
			Usartx_cfgInfo -> usart_sendFlag = UASRT_INIT_FAIL;
			return Usartx_cfgInfo -> usart_sendFlag;}
		
		/* Enable TX function */
		USART_FuncCmd(Usartx_cfgInfo->Usart_Unit, USART_TX, ENABLE);
			
		do{
			USART_WriteData(Usartx_cfgInfo->Usart_Unit, (uint16_t)(*pxInd ++));
			while(RESET ==  \
				USART_GetStatus(Usartx_cfgInfo->Usart_Unit,USART_FLAG_TX_CPLT))
			{
				s_timeout --;
				if(!s_timeout)
				{
					Usartx_cfgInfo->usart_sendFlag = USART_SEND_TOT;
					/* Disable TX function when sending timeout */
					USART_FuncCmd(Usartx_cfgInfo->Usart_Unit, USART_TX, DISABLE);
					return Usartx_cfgInfo->usart_sendFlag;
				}
			}
		}while(s_len --);
		
		/* Disable TX function */
		USART_FuncCmd(Usartx_cfgInfo->Usart_Unit, USART_TX, DISABLE);
		
		Usartx_cfgInfo->usart_sendFlag = USART_SEND_OK;
		
		pxInd = NULL;
		
		return Usartx_cfgInfo->usart_sendFlag;
}

/**
 * @brief  Usart send data with DMA（串口发送数据（DMA方式））
 * @param  Usart_cfgInfo_Typedef *（指向串口实例的指针）,uint8_t *（指向待发送的数据指针）,uint16_t（数据长度）
 * @retval None
 */
void Usart_Send_DMA(Usart_cfgInfo_Typedef * Usartx_cfgInfo,uint8_t *pxInd,uint16_t s_len)
{
		/* 重载DMA数据源地址 */
		DMA_Reload_Src_For_Uart(Usartx_cfgInfo, pxInd);
			
		/* 重载DMA传输数据块大小 */
		DMA_SetTransCount(Usartx_cfgInfo->Linked_DMABase, Usartx_cfgInfo->Linked_DMA_txChannel, s_len);
	
		/* 使能相关DMA通道，等待串口发送 */
		(void)DMA_ChCmd(Usartx_cfgInfo->Linked_DMABase, Usartx_cfgInfo->Linked_DMA_txChannel, ENABLE);
	
		/* 串口发送功能使能，开始发送 */
		USART_FuncCmd(Usartx_cfgInfo->Usart_Unit, USART_TX, ENABLE);
}

Usart_SendFlag_Typedef Usr_com_putchar(char *pxInd,Usart_cfgInfo_Typedef * Usartx_cfgInfo)
{
		if(Usartx_cfgInfo->Usart_Unit == NULL){
			Usartx_cfgInfo -> usart_sendFlag = UASRT_INIT_FAIL;
			return Usartx_cfgInfo -> usart_sendFlag;}
		
		do{
			USART_WriteData(Usartx_cfgInfo->Usart_Unit, (uint16_t)(*pxInd ++));
			while(RESET ==  \
				USART_GetStatus(Usartx_cfgInfo->Usart_Unit,USART_FLAG_TX_CPLT))
			{}
		}while(*pxInd != '\0');
			
		Usartx_cfgInfo->usart_sendFlag = USART_SEND_OK;
		
		pxInd = NULL;
		
		return Usartx_cfgInfo->usart_sendFlag;
}

/**
 * @brief  Init usart（串口初始化）
 * @param  Usartx_Typedef * （指向串口实例的指针）
 * @retval None
 */
void BSP_Usart_Init(Usartx_Typedef * Usartx)
{
	if(Usartx == NULL)
		return;
	
	Usartx->Uart_Handler.fptr_com_send_pending = Usart_Send_Pending;
	Usartx->Uart_Handler.fptr_init = Usart_ObjInit;
	Usartx->Uart_Handler.fptr_com_putchar = Usr_com_putchar;
	Usartx->Uart_Handler.fptr_com_send_dma = Usart_Send_DMA;
	
	Usartx->Uart_Handler.fptr_init(&Usartx->Uart_cfg_Info);
}

/**
 * @brief  Init usart（串口初始化再次封装）
 * @param  Usartx_Typedef * （指向串口实例的指针）
 * @retval None
 */
void usart_Init(Usartx_Typedef * usart_dev_t)
{
	BSP_Usart_Init(usart_dev_t);
}

void usart_Send(Usartx_Typedef * usart_dev_t,uint8_t *pxInd,uint32_t s_len,usart_tmode tmode)
{
	if(tmode == dma)
	{
		usart_dev_t->Uart_Handler.fptr_com_send_dma(&usart_dev_t->Uart_cfg_Info,pxInd,s_len);
	}
	else if(tmode == pend)
	{
		usart_dev_t->Uart_Handler.fptr_com_send_pending(&usart_dev_t->Uart_cfg_Info,pxInd,s_len,0);
	}
}

#if PRINT_DEBUG_INFO_EN
/*  Serial port redirection */
int fputc(int ch, FILE *f)
{
    while(RESET ==  \
				USART_GetStatus(DEBUG_USART.Uart_cfg_Info.Usart_Unit,USART_FLAG_TX_CPLT));

		USART_WriteData(DEBUG_USART.Uart_cfg_Info.Usart_Unit, (uint8_t)ch);
    return ch;
}
#endif

