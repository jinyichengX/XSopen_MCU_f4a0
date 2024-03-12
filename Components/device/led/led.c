#include "led.h"

//Functions declaration
void Io_Init(led_dev_t * led_dev);
void Io_SetP(led_dev_t * led_dev,int32_t on_t,int32_t off_t,uint32_t repeat_num);
void Io_Start(led_dev_t * led_dev);
//用户定义LED对象
led_dev_t led1 =
{
	.io_t.io_obj = 
					{	
						.IO_PortSel = GPIO_PORT_C,	//用户可配置
						.IO_PinSel = GPIO_PIN_09,							//用户可配置
						.GpioElec = 
									{ 
										PIN_STAT_RST,							
										PIN_DIR_OUT,
									},
					},
};

led_t led_ioctl = {
	.ioInit = Io_Init,
	.IoSetPara = Io_SetP,
	.IoOutLoop = Io_Start,
};

void Io_Init(led_dev_t * led_dev)
{
	led_dev->io_t.InitHandler = xs_GpioInit;					
	led_dev->io_t.OutHandler = xs_GpioOutput;					
	led_dev->io_t.ToggleHandler = xs_GpioToggle;	
	led_dev->io_t.GetbitHandler = xs_GpioGetBit;
	
	led_dev->io_t.InitHandler(&led_dev->io_t);
}

/*
 * 函数说明
 * on_time_ms: 点亮时间
 * off_time_ms: 熄灭时间
 * repeat_num: 执行次数
 * 说明：
 * （1）无限次数闪烁：on_time_ms > 0；off_time_ms > 0; repeat_num = 0xFF
 * （2）常亮：on_time_ms > 0; off_time_ms = 0; repeat_num = 0xFF
 * （3）常熄灭：on_time_ms = 0; off_time_ms ≥ 0; repeat_num = 0xFF
 * （4）若repeat_num为0，则忽略
 */
void Io_SetP(led_dev_t * led_dev,int32_t on_t,int32_t off_t,uint32_t repeat_num)
{
	if ((on_t == led_dev->on_time) && \
            (off_t == led_dev->off_time) && \
            (repeat_num == led_dev->repeat_num))
	{
		return;
	}
	
	led_dev->on_time = on_t;
	led_dev->off_time = off_t;
	led_dev->repeat_num = repeat_num;
}

//简单实现LED亮灭功能，待支持闪烁功能（见led_dev_t中的参数）
void Io_Start(led_dev_t * led_dev)
{
	led_dev->io_t.OutHandler(&led_dev->io_t,_LED_ON_);
#if 0
	led_dev->io_t.OutHandler(&led_dev->io_t,_LED_OFF_);
#endif
}
