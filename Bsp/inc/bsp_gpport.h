#ifndef __BSP_GPPORT_H__
#define __BSP_GPPORT_H__

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
#endif

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32_ll_gpio.h"
#include "hc32_ll.h"

typedef struct gpport_obj_elecTypestruct
{
	uint8_t 	IO_PortSel;
	
	void (*fptr_PortOutputEnHandler)(uint8_t u8Port,en_functional_state_t enNewState);
	void (*fptr_PortReadHandler)(uint8_t u8Port);
	void (*fptr_PortOutputHandler)(uint8_t u8Port, uint16_t u16PortVal);
}port_ObjType;

//void GPIO_OutputCmd(uint8_t u8Port, uint16_t u16Pin, en_functional_state_t enNewState)
//uint16_t GPIO_ReadInputPort(uint8_t u8Port)
//void GPIO_WritePort(uint8_t u8Port, uint16_t u16PortVal)
#ifdef __cplusplus
}
#endif
#endif /* __BSP_GPPORT_H__ */

