#ifndef __BSP_SPI_H__
#define __BSP_SPI_H__

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
#endif

#include "hc32_ll_spi.h"
#include "hc32_ll_gpio.h"
#include "hc32_ll_pwc.h"
#include "hc32_ll.h"

#include "bsp_gpio.h"

#include <string.h>

#define SOFT_SPI_ENABLE 1

#if !SOFT_SPI_ENABLE
///* SPI definition */
//#define SPI_UNIT                        (CM_SPI1)
//#define SPI_CLK                         (FCG1_PERIPH_SPI1)
//#define SPI_TX_EVT_SRC                  (EVT_SRC_SPI1_SPTI)
//#define SPI_RX_EVT_SRC                  (EVT_SRC_SPI1_SPRI)

///* DMA definition */
//#define DMA_UNIT                        (CM_DMA1)
//#define DMA_CLK                         (FCG0_PERIPH_DMA1 | FCG0_PERIPH_AOS)
//#define DMA_TX_CH                       (DMA_CH0)
//#define DMA_TX_TRIG_CH                  (AOS_DMA1_0)

//#define DMA_RX_CH                       (DMA_CH1)
//#define DMA_RX_INT_CH                   (DMA_INT_TC_CH1)
//#define DMA_RX_TRIG_CH                  (AOS_DMA1_1)
//#define DMA_RX_INT_SRC                  (INT_SRC_DMA1_TC1)
//#define DMA_RX_IRQ_NUM                  (INT006_IRQn)

///* CS = PC10 */
//#define SPI_CS_PORT                     (GPIO_PORT_C)
//#define SPI_CS_PIN                      (GPIO_PIN_10)
//#define SPI_CS_FUNC                     (GPIO_FUNC_47)
///* SCK = PC12 */
//#define SPI_SCK_PORT                    (GPIO_PORT_C)
//#define SPI_SCK_PIN                     (GPIO_PIN_12)
//#define SPI_SCK_FUNC                    (GPIO_FUNC_40)
///* MOSI = PB5 */
//#define SPI_MOSI_PORT                   (GPIO_PORT_B)
//#define SPI_MOSI_PIN                    (GPIO_PIN_05)
//#define SPI_MOSI_FUNC                   (GPIO_FUNC_41)
///* MISO = PB7 */
//#define SPI_MISO_PORT                   (GPIO_PORT_B)
//#define SPI_MISO_PIN                    (GPIO_PIN_07)
//#define SPI_MISO_FUNC                   (GPIO_FUNC_42)
typedef struct 
{
	uint8_t port;
	uint16_t pin;
	uint8_t func;
}spi_io_t;

typedef struct
{	
	/* spi instance and clk */
	CM_SPI_TypeDef * unit;
	uint32_t clk_src;
	
	/* DMA related */
	CM_DMA_TypeDef * linked_dma_unit;
	
	uint16_t tx_link_dma_ch;
	uint32_t dma_tx_trig_ch;
	
	uint16_t rx_link_dma_ch;
	uint32_t dma_rx_trig_ch;
	
	/* spi io */
	spi_io_t spi_cs;
	spi_io_t spi_clk;
	spi_io_t spi_mosi;
	spi_io_t spi_miso;
	
	/* spi tx/rx buffer */
	uint32_t tx_buf[1024];
	uint32_t rx_buf[1024];
}spi_cfg_t;

extern void BSP_SPI_Init(spi_cfg_t spi_cfg);
extern int32_t BSP_SPI_WriteRead(const void *pvTxBuf, void *pvRxBuf, uint32_t u32Len, uint32_t u32Timeout);
#else
typedef struct
{	
	io_HandlerType SPI_SCK;
	io_HandlerType SPI_CS;
	io_HandlerType SPI_MOSI;
	io_HandlerType SPI_MISO;
}SPI_InitCfg_Typedef;

extern void SPI_Delay(void);
extern void BSP_SPI_Port_Initialize(SPI_InitCfg_Typedef * SPI_InitCfg);
extern uint8_t BSP_SPI_WriteReadByte(SPI_InitCfg_Typedef * SPI_InitCfg,uint8_t data);
extern void BSP_SPI_TabSelCmd(SPI_InitCfg_Typedef * SPI_InitCfg,en_functional_state_t state);
#endif
#ifdef __cplusplus
}
#endif
#endif /* __BSP_SPI_H__ */

/**********************************************************************

硬件spi主机--多从机结构，使用此结构将SPI_CR1.SPIMDS设置为1（SPI模式选择3线式）

|		   主机	|							 |从机0    	 | 
|  		 SCK	|------------->|SCK				 |
|						|						|	 |					 |
|			 MOSI	|------------->|MOSI			 |
|						|				|	|	|  |           |
|			 MISO	|------------->|MISO			 |
|						|				|	|	|  |           |
|			 SSO	|------------->|SS				 | 
|						|				|	|	|
|           |				|	|	|	 |从机1      |
|			 SS1	|-----|	|	|	-->|SCK				 |
|						|			|	|	|--->|MOSI			 |
|			 SS2	|			|	|----->|MISO       |
|			 SS3	|			|------->|SS         |

														............
														............
**********************************************************************/
