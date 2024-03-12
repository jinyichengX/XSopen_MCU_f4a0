#ifndef __BSP_UART_H__
#define __BSP_UART_H__

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
#endif

#include "hc32_ll_usart.h"
#include "hc32_ll_gpio.h"
#include "hc32_ll.h"
#include "hc32_ll_dma.h"
#include <stdbool.h>
#include <stdio.h>

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/
typedef unsigned int USART_TIMEOUT_TIME_TYPE;

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/* usart max IRQ num definition */
#define USART_MAX_IRQ_NUM 4 //分别是接收中断，timeout中断，错误中断，发送完成中断

//需要串口打印调试信息则在此处修改宏定义
#define PRINT_DEBUG_INFO_EN 1

#if PRINT_DEBUG_INFO_EN
	//用户修改打印调试信息的串口(usart1~usart10)
	#define DEBUG_USART usart10
#endif

#if PRINT_DEBUG_INFO_EN
	#define DEBUG_COM DEBUG_USART
	#define usr_debug_printf printf
#else
	#define usr_debug_printf(...)
#endif

#define	UART1_EN	1
#define	UART2_EN	0
#define	UART3_EN	0
#define	UART4_EN	0
#define	UART5_EN	0
#define	UART6_EN	0
#define UART7_EN	0
#define UART8_EN 	0
#define UART9_EN	0
#define UART10_EN 1

/* serial port send state enumeration */
typedef enum
{
	USART_SEND_OK,	 ///< 0 串口发送成功
	USART_SEND_TOT,	 ///< 1 串口发送超时
	UASRT_INIT_FAIL  ///< 2 串口未初始化
}Usart_SendFlag_Typedef;

/* serial port receive data mode enumeration */
typedef enum 
{
	USART_RX_MODE_IRQ = 1,
	USART_RX_MODE_DMA = 2,
}UASRT_RX_MODE_SELEC;

/* serial port link DMA dis(en)able enumeration begin */
typedef enum
{
	USART_LINK_DMA_DISABLE = false,
	USART_LINK_DMA_ENABLE = true,
}USART_IFLINK_DMA_Typedef;
/* serial port link DMA dis(en)able enumeration end */


/* serial port rx iqr fptr struct begin */
typedef struct
{
	func_ptr_t Usartx_ReciveError_IRQCallBack;
	func_ptr_t Usartx_ReciveTOU_IRQCallBack;
	func_ptr_t Usartx_ReciveOneByte_IRQCallBack;
}Usartx_CallBack_Typedef;
/* serial port rx iqr fptr struct end */


/* serial port configuration info and state struct begin*/
typedef struct Usart_Configuration_info
{
	CM_USART_TypeDef* Usart_Unit;
	
	uint32_t FCGx_Periph;
	
	uint8_t RxPortSel;
	uint16_t RxPinSel;
	uint8_t TxPortSel;
	uint16_t TxPinSel;
	uint8_t USART_TX_GPIO_FUNC;
	uint8_t USART_RX_GPIO_FUNC;
	
	stc_usart_uart_init_t stcUartInit;
	
	stc_irq_signin_config_t stcIrqSigninConfig[USART_MAX_IRQ_NUM];
	uint8_t IRQ_Prio; 
	
	USART_TIMEOUT_TIME_TYPE usart_tx_timeoout_time;
	USART_TIMEOUT_TIME_TYPE usart_rx_timeoout_time;
	Usart_SendFlag_Typedef usart_sendFlag;
	
	UASRT_RX_MODE_SELEC Usart_Rx_Mode;
	
	bool TIMEOUT_ALLOWED;
	CM_TMR0_TypeDef * TimerBase;
	uint8_t TimerChannel;
	
	CM_DMA_TypeDef *Linked_DMABase;
	uint8_t Linked_DMA_rxChannel;
	int16_t DMA_Trig_rxSource;
	
	uint8_t Linked_DMA_txChannel;
	int16_t DMA_Trig_txSource;
	IRQn_Type tx_dma_tc_enIRQn;
	uint8_t rx_len;
	uint8_t rx_flag;
	uint8_t rx_buf[512];
	bool fifo_en; //无timeout中断的uart使用
}Usart_cfgInfo_Typedef;
/* serial port configuration info and state struct end */


/* serial port handle functions struct definition begin */
typedef struct Usart_Handler
{
//	void (* fptr_ClkCmd)(Usart_cfgInfo_Typedef *,bool);
	void (* fptr_init)(Usart_cfgInfo_Typedef *);
	Usart_SendFlag_Typedef (* fptr_com_send_pending)(Usart_cfgInfo_Typedef *,uint8_t *,uint32_t,USART_TIMEOUT_TIME_TYPE);
	void (* fptr_com_send_dma)(Usart_cfgInfo_Typedef *,uint8_t *,uint16_t);
	Usart_SendFlag_Typedef (* fptr_com_putchar)(char *pxInd,Usart_cfgInfo_Typedef * Usartx_cfgInfo);
	Usartx_CallBack_Typedef Usartx_IRQCallBack;
}Usart_Handler_Typedef;
/* serial port handle functions struct definition end */


/* serial port obj struct definition begin */
typedef struct Usartx_struct
{
	Usart_cfgInfo_Typedef Uart_cfg_Info;
	Usart_Handler_Typedef	Uart_Handler;
}Usartx_Typedef;
/* serial port obj struct definition end */

typedef enum
{
	pend,
	dma,
}usart_tmode;

typedef struct usartglobalhandlestruct
{
	void (* fp_setup)(Usartx_Typedef *);
	void (* fp_send)(Usartx_Typedef *,uint8_t *,uint32_t,usart_tmode);
}usart_t;

extern usart_t g_uart;
/* extern serial port instances */
extern Usartx_Typedef usart1,usart2,usart3,usart4,usart5,usart6,usart7,usart8,usart9,usart10;

/* extern serial port functions */
extern void BSP_Usart_Init(Usartx_Typedef * Usartx);
extern Usart_SendFlag_Typedef Usr_com_putchar(char *pxInd,Usart_cfgInfo_Typedef * Usartx_cfgInfo);
extern void Usart_Send_DMA(Usart_cfgInfo_Typedef * Usartx_cfgInfo,uint8_t *pxInd,uint16_t s_len);


extern void U1_TX_DMA_TC_IrqCallback(void);
extern void U2_TX_DMA_TC_IrqCallback(void);
extern void U3_TX_DMA_TC_IrqCallback(void);
extern void U4_TX_DMA_TC_IrqCallback(void);
extern void U5_TX_DMA_TC_IrqCallback(void);
extern void U6_TX_DMA_TC_IrqCallback(void);
extern void U7_TX_DMA_TC_IrqCallback(void);
extern void U8_TX_DMA_TC_IrqCallback(void);
extern void U9_TX_DMA_TC_IrqCallback(void);
extern void U10_TX_DMA_TC_IrqCallback(void);
#ifdef __cplusplus
}
#endif
#endif /*__BSP_UART_H__*/

