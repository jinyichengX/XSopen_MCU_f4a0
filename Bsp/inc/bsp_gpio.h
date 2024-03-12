#ifndef __BSP_GPIO_H__
#define __BSP_GPIO_H__

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
#endif


/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32_ll_gpio.h"
#include "hc32_ll.h"

typedef struct gpio_obj_elecTypestruct
{
	uint8_t 	IO_PortSel;
	uint16_t 	IO_PinSel;
	stc_gpio_init_t GpioElec;
}io_ObjType;

typedef struct gpio_stdfunc
{
	io_ObjType io_obj;
	void  (*InitHandler)(struct gpio_stdfunc *);
	void	(*IoSetIn)(struct gpio_stdfunc *);
	void  (*IoSetOut)(struct gpio_stdfunc *);
	void 	(*OutHandler)(struct gpio_stdfunc *, en_functional_state_t);
	void  (*ToggleHandler)(struct gpio_stdfunc *);
	en_pin_state_t (*GetbitHandler)(struct gpio_stdfunc *);
	

}io_HandlerType;

extern void xs_GpioInit(io_HandlerType *);
extern void xs_GpioOutput(io_HandlerType *, en_functional_state_t level);
extern void xs_GpioToggle(io_HandlerType *);
extern void xs_GpioSetIn(io_HandlerType *);
extern void xs_GpioSetOut(io_HandlerType *);
extern en_pin_state_t xs_GpioGetBit(io_HandlerType *ioHandler);
//  StartAddress=*((uint16 *)0xEFFE);
//  ( ( void(*)(void) )StartAddress )();
#ifdef __cplusplus
}
#endif
#endif /*__BSP_GPIO_H__*/

