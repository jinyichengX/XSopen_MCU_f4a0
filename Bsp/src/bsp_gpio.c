#include "bsp_gpio.h"

/* IO初始化 */
void xs_GpioInit(io_HandlerType *ioHandler)
{
	(void)GPIO_Init(ioHandler->io_obj.IO_PortSel,ioHandler->io_obj.IO_PinSel,&(ioHandler->io_obj.GpioElec));
}

/* IO输出 */
void xs_GpioOutput(io_HandlerType *ioHandler,en_functional_state_t level)
{
	if(ENABLE == level){
		GPIO_SetPins(ioHandler->io_obj.IO_PortSel, ioHandler->io_obj.IO_PinSel);
	}
	else if(DISABLE == level){
		GPIO_ResetPins(ioHandler->io_obj.IO_PortSel, ioHandler->io_obj.IO_PinSel);
	}
}

/* IO输出翻转 */
void xs_GpioToggle(io_HandlerType *ioHandler)
{
	GPIO_TogglePins(ioHandler->io_obj.IO_PortSel, ioHandler->io_obj.IO_PinSel);
}

/* IO输入 */
en_pin_state_t xs_GpioGetBit(io_HandlerType *ioHandler)
{
	en_pin_state_t pin_state;
	
	pin_state = GPIO_ReadInputPins(ioHandler->io_obj.IO_PortSel, ioHandler->io_obj.IO_PinSel);
	
	return pin_state;
}

/* IO设置输入模式 */
void xs_GpioSetIn(io_HandlerType *ioHandler)
{
	ioHandler->io_obj.GpioElec.u16PinDir = PIN_DIR_IN;
	xs_GpioInit(ioHandler);
}

/* IO设置输出模式 */
void xs_GpioSetOut(io_HandlerType *ioHandler)
{
	ioHandler->io_obj.GpioElec.u16PinDir = PIN_DIR_OUT;
	xs_GpioInit(ioHandler);
}
