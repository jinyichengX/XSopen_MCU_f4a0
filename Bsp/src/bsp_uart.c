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

/* ��֪����������C������ʹ�ð������ĺ��� */
#pragma import(__use_no_semihosting)

/* ��׼����Ҫ��֧������ */
struct __FILE
{
    int handle;
};

FILE __stdout;
FILE __stdin;
/* ���� _sys_exit() �Ա���ʹ�ð�����ģʽ */
void _sys_exit(int x)
{
    x = x;
}
#endif

void _ttywrch(int ch)
{
	ch = ch;
}

//��������
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
		/* ������ vTaskMsgPro �����¼���־ */
		xResult = xEventGroupSetBitsFromISR(EventGroup1, /* �¼���־���� */
																				EventGroup1_BIT0 , /* ���� bit0 */
																				&xHigherPriorityTaskWoken );
		/* ��Ϣ���ɹ����� */
		if( xResult != pdFAIL )
		{
			/* ��� xHigherPriorityTaskWoken = pdTRUE����ô�˳��жϺ��е���ǰ������ȼ�����ִ�� */
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

/* �ڴ���ʹ��DMA����ʱ�����������жϻص�����ȱһ���� */
/**
 * @brief  DMA Transmit complete IRQ callback.
 * @param  None
 * @retval None
 */
void U1_TX_DMA_TC_IrqCallback(void)
{
	/* ʹ�ܴ��ڷ�������ж� */
	USART_FuncCmd(usart1.Uart_cfg_Info.Usart_Unit, USART_INT_TX_CPLT, ENABLE);
	/* ֹͣDMA��Ӧͨ�� */
	(void)DMA_ChCmd(usart1.Uart_cfg_Info.Linked_DMABase, usart1.Uart_cfg_Info.Linked_DMA_txChannel, DISABLE);
	/* ���DMA ������ɱ�־λ */
  DMA_ClearTransCompleteStatus(usart1.Uart_cfg_Info.Linked_DMABase, DMA_FLAG_TC_CH1);
	
}

/**
 * @brief  USART1 Transmit complete IRQ callback.
 * @param  None
 * @retval None
 */
void U1_TC_IrqCallback(void)
{
	/* ʧ�ܷ��ͺͷ�������ж� */
	USART_FuncCmd(usart1.Uart_cfg_Info.Usart_Unit, (USART_TX | USART_INT_TX_CPLT), DISABLE);
	/* ���������ɱ�־λ */
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

/* �ڴ���ʹ��DMA����ʱ�����������жϻص�����ȱһ���� */
/**
 * @brief  DMA Transmit complete IRQ callback.
 * @param  None
 * @retval None
 */
void U2_TX_DMA_TC_IrqCallback(void)
{
	/* ʹ�ܴ��ڷ�������ж� */
	USART_FuncCmd(usart2.Uart_cfg_Info.Usart_Unit, USART_INT_TX_CPLT, ENABLE);
	/* ֹͣDMA��Ӧͨ�� */
	(void)DMA_ChCmd(usart2.Uart_cfg_Info.Linked_DMABase, usart2.Uart_cfg_Info.Linked_DMA_txChannel, DISABLE);
	/* ���DMA ������ɱ�־λ */
  DMA_ClearTransCompleteStatus(usart2.Uart_cfg_Info.Linked_DMABase, DMA_FLAG_TC_CH1);
	
}
/**
 * @brief  USART2 Transmit complete IRQ callback.
 * @param  None
 * @retval None
 */
void U2_TC_IrqCallback(void)
{
	/* ʧ�ܷ��ͺͷ�������ж� */
	USART_FuncCmd(usart2.Uart_cfg_Info.Usart_Unit, (USART_TX | USART_INT_TX_CPLT), DISABLE);
	/* ���������ɱ�־λ */
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
		/* ֹͣ���� */
		USART_FuncCmd(usart10.Uart_cfg_Info.Usart_Unit, USART_TX, DISABLE);
		/* ֹͣDMA */
//		(void)DMA_ChCmd(TX_DMA_UNIT, TX_DMA_CH, DISABLE);
		/* ���DMA ������ɱ�־λ */
    DMA_ClearTransCompleteStatus(usart10.Uart_cfg_Info.Linked_DMABase, DMA_FLAG_TC_CH1);
	
}
#endif
#if UART1_EN
//�û��޸�usart1~usart10
Usartx_Typedef usart1 = {
	.Uart_cfg_Info = {
		.Usart_Unit = CM_USART1,																	//����ʵ�����û����ɸ���
		
		.FCGx_Periph = FCG3_PERIPH_USART1,												//����ʱ�����ã��û����ɸ���
		
		.RxPortSel = GPIO_PORT_B,																	//���ڽ��չ���ӳ���Portѡ�񣨲ο������ֲᣩ���û��ɸ���
		.RxPinSel = GPIO_PIN_13,																	//���ڽ��չ���ӳ���Pinѡ�񣨲ο������ֲᣩ���û��ɸ���
		.USART_RX_GPIO_FUNC = GPIO_FUNC_33,												//IO���ù���ѡ�񣨲ο������ֲᣩ���û��ɸ���

		.TxPortSel = GPIO_PORT_B,																	//���ڷ��͹���ӳ���Portѡ�񣨲ο������ֲᣩ���û��ɸ���
		.TxPinSel = GPIO_PIN_12,																	//���ڷ��͹���ӳ���Pinѡ�񣨲ο������ֲᣩ���û��ɸ���
		.USART_TX_GPIO_FUNC = GPIO_FUNC_32,												//IO���ù���ѡ�񣨲ο������ֲᣩ���û��ɸ���
		
		.stcUartInit = {.u32ClockDiv = USART_CLK_DIV64,						//ʱ�ӷ�Ƶ���û��ɸ���
			.u32Baudrate = 115200,																	//�����ʣ��û��ɸ���
			.u32OverSampleBit = USART_OVER_SAMPLE_8BIT,							//����������,���������
			.u32StartBitPolarity = USART_START_BIT_LOW,							
		},
		
		.stcIrqSigninConfig[USART_MAX_IRQ_NUM - USART_MAX_IRQ_NUM] = {.enIRQn = INT000_IRQn,//�жϺţ��û��ɸ���
			.enIntSrc = INT_SRC_USART1_EI,																//�ж�Դ���û����ɸ���
			.pfnCallback = &USART1_RxError_IrqCallback,										//�жϻص��������û��ɸ���
		},
		
			.stcIrqSigninConfig[USART_MAX_IRQ_NUM - USART_MAX_IRQ_NUM +1] = {.enIRQn = INT001_IRQn,	//�жϺţ��û��ɸ���
			.enIntSrc = INT_SRC_USART1_RI,																		//�ж�Դ���û����ɸ���
			.pfnCallback = &USART1_RxFull_IrqCallback,											//�жϻص��������û��ɸ���
		},
			.stcIrqSigninConfig[USART_MAX_IRQ_NUM - USART_MAX_IRQ_NUM +2] = {.enIRQn = INT002_IRQn,	//�жϺţ��û��ɸ���
			.enIntSrc = INT_SRC_USART1_RTO,																		//�ж�Դ���û����ɸ���
			.pfnCallback = &USART1_RxTimeOut_IrqCallback,											//�жϻص��������û��ɸ���
		},
			.stcIrqSigninConfig[USART_MAX_IRQ_NUM - USART_MAX_IRQ_NUM +3] = {.enIRQn = INT003_IRQn,	//�жϺţ��û��ɸ���
			.enIntSrc = INT_SRC_USART1_TCI,																		//�ж�Դ���û����ɸ���
			.pfnCallback = &U1_TC_IrqCallback,											//�жϻص��������û��ɸ���
		},
		.Usart_Rx_Mode = USART_RX_MODE_DMA,				//���ڽ������ݽ��շ�ʽѡ��rx�ж�/DMA+timeout�жϣ����û��ɸ���
		.IRQ_Prio = DDL_IRQ_PRIO_DEFAULT,					//�ж����ȼ����û��ɸ���
		.usart_tx_timeoout_time = 200000,					//���ͳ�ʱʱ�䣬���ݷ�����������ϵͳʱ���趨���û��ɸ���
		.usart_rx_timeoout_time = 100,						//���ճ�ʱʱ�䣬���ݽ����������Ͳ������趨���û��ɸ���
		.TimerBase = CM_TMR0_1,										//Ӳ���������û����ɸ���
		.TimerChannel = TMR0_CH_A,								//Ӳ���������û����ɸ���
		
		.Linked_DMABase = CM_DMA1,								//DMAѡ���û��ɸ���
		.Linked_DMA_rxChannel = DMA_CH0,						//DMAͨ��ѡ���û��ɸ���
		.DMA_Trig_rxSource = EVT_SRC_USART1_RI,			//�¼��������Ψһ���û����ɸ���
		
		.Linked_DMA_txChannel = DMA_CH1,				//DMAͨ��ѡ���û��ɸ���
		.DMA_Trig_txSource = EVT_SRC_USART1_TI,			//�¼��������Ψһ���û����ɸ���
		.tx_dma_tc_enIRQn = INT004_IRQn,
		.TIMEOUT_ALLOWED = true,									//�Ƿ�����ʱ�����жϣ�Ӳ���������û����ɸ���
	},
};
#endif

#if UART2_EN
Usartx_Typedef usart2 = {
	.Uart_cfg_Info = {
		.Usart_Unit = CM_USART2,																	//����ʵ�����û����ɸ���
		
		.FCGx_Periph = FCG3_PERIPH_USART2,												//����ʱ�����ã��û����ɸ���
		
		.RxPortSel = GPIO_PORT_I,																	//���ڽ��չ���ӳ���Portѡ�񣨲ο������ֲᣩ���û��ɸ���
		.RxPinSel = GPIO_PIN_06,																	//���ڽ��չ���ӳ���Pinѡ�񣨲ο������ֲᣩ���û��ɸ���
		.USART_RX_GPIO_FUNC = GPIO_FUNC_35,												//IO���ù���ѡ�񣨲ο������ֲᣩ���û��ɸ���

		.TxPortSel = GPIO_PORT_I,																	//���ڷ��͹���ӳ���Portѡ�񣨲ο������ֲᣩ���û��ɸ���
		.TxPinSel = GPIO_PIN_07,																	//���ڷ��͹���ӳ���Pinѡ�񣨲ο������ֲᣩ���û��ɸ���
		.USART_TX_GPIO_FUNC = GPIO_FUNC_34,												//IO���ù���ѡ�񣨲ο������ֲᣩ���û��ɸ���
		
		.stcUartInit = {.u32ClockDiv = USART_CLK_DIV64,						//ʱ�ӷ�Ƶ���û��ɸ���
			.u32Baudrate = 115200,																	//�����ʣ��û��ɸ���
			.u32OverSampleBit = USART_OVER_SAMPLE_8BIT,							//����������,���������
			.u32StartBitPolarity = USART_START_BIT_LOW,							
		},
		
		.stcIrqSigninConfig[USART_MAX_IRQ_NUM - USART_MAX_IRQ_NUM] = {.enIRQn = INT005_IRQn,//�жϺţ��û��ɸ���
			.enIntSrc = INT_SRC_USART2_EI,																//�ж�Դ���û����ɸ���
			.pfnCallback = &USART2_RxError_IrqCallback,										//�жϻص��������û��ɸ���
		},
		
			.stcIrqSigninConfig[USART_MAX_IRQ_NUM - USART_MAX_IRQ_NUM +1] = {.enIRQn = INT006_IRQn,	//�жϺţ��û��ɸ���
			.enIntSrc = INT_SRC_USART2_RI,																		//�ж�Դ���û����ɸ���
			.pfnCallback = &USART2_RxFull_IrqCallback,											//�жϻص��������û��ɸ���
		},
			.stcIrqSigninConfig[USART_MAX_IRQ_NUM - USART_MAX_IRQ_NUM +2] = {.enIRQn = INT007_IRQn,	//�жϺţ��û��ɸ���
			.enIntSrc = INT_SRC_USART2_RTO,																		//�ж�Դ���û����ɸ���
			.pfnCallback = &USART2_RxTimeOut_IrqCallback,											//�жϻص��������û��ɸ���
		},
			.stcIrqSigninConfig[USART_MAX_IRQ_NUM - USART_MAX_IRQ_NUM +3] = {.enIRQn = INT008_IRQn,	//�жϺţ��û��ɸ���
			.enIntSrc = INT_SRC_USART2_TCI,																		//�ж�Դ���û����ɸ���
			.pfnCallback = &U2_TC_IrqCallback,											//�жϻص��������û��ɸ���
		},
		.Usart_Rx_Mode = USART_RX_MODE_DMA,				//���ڽ������ݽ��շ�ʽѡ��rx�ж�/DMA+timeout�жϣ����û��ɸ���
		.IRQ_Prio = DDL_IRQ_PRIO_DEFAULT,					//�ж����ȼ����û��ɸ���
		.usart_tx_timeoout_time = 200000,					//���ͳ�ʱʱ�䣬���ݷ�����������ϵͳʱ���趨���û��ɸ���
		.usart_rx_timeoout_time = 2000,						//���ճ�ʱʱ�䣬���ݽ����������Ͳ������趨���û��ɸ���
		.TimerBase = CM_TMR0_1,										//Ӳ���������û����ɸ���
		.TimerChannel = TMR0_CH_B,								//Ӳ���������û����ɸ���
		
		.Linked_DMABase = CM_DMA1,								//DMAѡ���û��ɸ���
		.Linked_DMA_rxChannel = DMA_CH2,						//DMAͨ��ѡ���û��ɸ���
		.DMA_Trig_rxSource = EVT_SRC_USART2_RI,			//�¼��������Ψһ���û����ɸ���
		
		.Linked_DMA_txChannel = DMA_CH3,				//DMAͨ��ѡ���û��ɸ���
		.DMA_Trig_txSource = EVT_SRC_USART2_TI,			//�¼��������Ψһ���û����ɸ���
		.tx_dma_tc_enIRQn = INT009_IRQn,
		
		.TIMEOUT_ALLOWED = true,									//�Ƿ�����ʱ�����жϣ�Ӳ���������û����ɸ���
	},

};
#endif

#if UART6_EN
Usartx_Typedef usart6 = {
		.Uart_cfg_Info = {
		.Usart_Unit = CM_USART6,																	//����ʵ�����û����ɸ���
		
		.FCGx_Periph = FCG3_PERIPH_USART6,												//����ʱ�����ã��û����ɸ���
		
		.RxPortSel = GPIO_PORT_E,																	//���ڽ��չ���ӳ���Portѡ�񣨲ο������ֲᣩ
		.RxPinSel = GPIO_PIN_03,																	//���ڽ��չ���ӳ���Pinѡ�񣨲ο������ֲᣩ
		.USART_RX_GPIO_FUNC = GPIO_FUNC_37,												//IO���ù���ѡ�񣨲ο������ֲᣩ

		.TxPortSel = GPIO_PORT_E,																	//���ڷ��͹���ӳ���Portѡ�񣨲ο������ֲᣩ
		.TxPinSel = GPIO_PIN_05,																	//���ڷ��͹���ӳ���Pinѡ�񣨲ο������ֲᣩ
		.USART_TX_GPIO_FUNC = GPIO_FUNC_36,												//IO���ù���ѡ�񣨲ο������ֲᣩ
		
		.stcUartInit = {
			.u32ClockDiv = USART_CLK_DIV64,													//ʱ�ӷ�Ƶ���û��ɸ���
			.u32Baudrate = 115200,																	//�����ʣ��û��ɸ���
			.u32OverSampleBit = USART_OVER_SAMPLE_8BIT,							//����������,���������
			.u32StartBitPolarity = USART_START_BIT_LOW,
		},
		
		.stcIrqSigninConfig[USART_MAX_IRQ_NUM - USART_MAX_IRQ_NUM] = {
			.enIRQn = INT002_IRQn,																				//�жϺţ��û��ɸ���
			.enIntSrc = INT_SRC_USART6_EI,																//�ж�Դ���û����ɸ���
			.pfnCallback = &USART6_RxError_IrqCallback,										//�жϻص��������û��ɸ���
		},
		
		.stcIrqSigninConfig[USART_MAX_IRQ_NUM - USART_MAX_IRQ_NUM +1] = {
			.enIRQn = INT003_IRQn,																					//�жϺţ��û��ɸ���
			.enIntSrc = INT_SRC_USART6_RI,																		//�ж�Դ���û����ɸ���
			.pfnCallback = &USART6_RxFull_IrqCallback,											//�жϻص��������û��ɸ���
		},
			.stcIrqSigninConfig[USART_MAX_IRQ_NUM - USART_MAX_IRQ_NUM +2] = {.enIRQn = INT002_IRQn,	//�жϺţ��û��ɸ���
			.enIntSrc = INT_SRC_USART6_RTO,																		//�ж�Դ���û����ɸ���
			.pfnCallback = &USART6_RxTimeOut_IrqCallback,											//�жϻص��������û��ɸ���
		},
		.Usart_Rx_Mode = USART_RX_MODE_IRQ,				//���ڽ������ݽ��շ�ʽѡ��rx�ж�/DMA+timeout�жϣ����û��ɸ���
		.IRQ_Prio = DDL_IRQ_PRIO_DEFAULT,					//�ж����ȼ����û��ɸ���
		.usart_tx_timeoout_time = 200000,					//���ͳ�ʱʱ�䣬���ݷ�����������ϵͳʱ���趨���û��ɸ���
		.usart_rx_timeoout_time = 5000,						//���ճ�ʱʱ�䣬���ݽ����������Ͳ������趨���û��ɸ���
		
		.TimerBase = CM_TMR0_2,										//Ӳ���������û����ɸ���
		.TimerChannel = TMR0_CH_A,								//Ӳ���������û����ɸ���
		
		.Linked_DMABase = CM_DMA1,								//DMAѡ���û��ɸ���
		.Linked_DMA_rxChannel = DMA_CH2,						//DMAͨ��ѡ���û��ɸ���
		.DMA_Trig_rxSource = EVT_SRC_USART6_RI,			//�¼��������Ψһ���û����ɸ���
		.TIMEOUT_ALLOWED = true,									//�Ƿ�����ʱ�����жϣ�Ӳ���������û����ɸ���
	},
};
#endif

#if UART10_EN
Usartx_Typedef usart10 = {
		.Uart_cfg_Info = {
		.Usart_Unit = CM_USART10,																	//����ʵ�����û����ɸ���
		
		.FCGx_Periph = FCG3_PERIPH_USART10,												//����ʱ�����ã��û����ɸ���
		
		.RxPortSel = GPIO_PORT_D,																	//���ڽ��չ���ӳ���Portѡ�񣨲ο������ֲᣩ
		.RxPinSel = GPIO_PIN_02,																	//���ڽ��չ���ӳ���Pinѡ�񣨲ο������ֲᣩ
		.USART_RX_GPIO_FUNC = GPIO_FUNC_39,												//IO���ù���ѡ�񣨲ο������ֲᣩ

		.TxPortSel = GPIO_PORT_D,																	//���ڷ��͹���ӳ���Portѡ�񣨲ο������ֲᣩ
		.TxPinSel = GPIO_PIN_03,																	//���ڷ��͹���ӳ���Pinѡ�񣨲ο������ֲᣩ
		.USART_TX_GPIO_FUNC = GPIO_FUNC_38,												//IO���ù���ѡ�񣨲ο������ֲᣩ
		
		.stcUartInit = {
			.u32ClockDiv = USART_CLK_DIV64,													//ʱ�ӷ�Ƶ���û��ɸ���
			.u32Baudrate = 9600,																	//�����ʣ��û��ɸ���
			.u32OverSampleBit = USART_OVER_SAMPLE_8BIT,							//����������,���������
			.u32StartBitPolarity = USART_START_BIT_LOW,
		},
		
		.usart_tx_timeoout_time = 200000,					//���ͳ�ʱʱ�䣬���ݷ�����������ϵͳʱ���趨���û��ɸ���
		.TIMEOUT_ALLOWED = false,									//�Ƿ�����ʱ�����жϣ�Ӳ���������û����ɸ���
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
		Usart_cfgInfo->stcUartInit.u32CKOutput = USART_CK_OUTPUT_ENABLE; //<<ʹ���ڲ�ʱ��Դ������Ϊtimer0����
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
		//����DMA,DMA���timeout����ʹ��
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
		
		/* ����timer0 */
		BSP_TMR0_Config_For_UsartTimeOut(Usart_cfgInfo);
	}
	
#endif
}

/**
 * @brief  Usart send data�����ڷ������ݣ�������ʽ,��ʱ�˳�����
 * @param  Usart_cfgInfo_Typedef *��ָ�򴮿�ʵ����ָ�룩,uint8_t *��ָ������͵�����ָ�룩,uint16_t�����ݳ��ȣ�,USART_TIMEOUT_TIME_TYPE����ʱʱ�䣩
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
 * @brief  Usart send data with DMA�����ڷ������ݣ�DMA��ʽ����
 * @param  Usart_cfgInfo_Typedef *��ָ�򴮿�ʵ����ָ�룩,uint8_t *��ָ������͵�����ָ�룩,uint16_t�����ݳ��ȣ�
 * @retval None
 */
void Usart_Send_DMA(Usart_cfgInfo_Typedef * Usartx_cfgInfo,uint8_t *pxInd,uint16_t s_len)
{
		/* ����DMA����Դ��ַ */
		DMA_Reload_Src_For_Uart(Usartx_cfgInfo, pxInd);
			
		/* ����DMA�������ݿ��С */
		DMA_SetTransCount(Usartx_cfgInfo->Linked_DMABase, Usartx_cfgInfo->Linked_DMA_txChannel, s_len);
	
		/* ʹ�����DMAͨ�����ȴ����ڷ��� */
		(void)DMA_ChCmd(Usartx_cfgInfo->Linked_DMABase, Usartx_cfgInfo->Linked_DMA_txChannel, ENABLE);
	
		/* ���ڷ��͹���ʹ�ܣ���ʼ���� */
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
 * @brief  Init usart�����ڳ�ʼ����
 * @param  Usartx_Typedef * ��ָ�򴮿�ʵ����ָ�룩
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
 * @brief  Init usart�����ڳ�ʼ���ٴη�װ��
 * @param  Usartx_Typedef * ��ָ�򴮿�ʵ����ָ�룩
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

