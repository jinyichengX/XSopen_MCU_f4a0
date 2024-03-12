#ifndef __BSP_DMA_H__
#define __BSP_DMA_H__

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
#endif

#include "hc32_ll_dma.h"
#include "hc32_ll.h"
#include "hc32_ll_usart.h"
#include "hc32_ll_gpio.h"
#include "hc32_ll_pwc.h"


#include "bsp_uart.h"
#include "bsp_i2c.h"

extern void DMA_Config_For_Uart(Usart_cfgInfo_Typedef *);
extern void DMA_Reload_Dest_For_Uart(Usart_cfgInfo_Typedef *Usart_cfgInfo);
extern void DMA_Reload_Src_For_Uart(Usart_cfgInfo_Typedef *Usart_cfgInfo, uint8_t *p_tx);
#ifdef __cplusplus
}
#endif
#endif /* __BSP_DMA_H__ */


