#include "led.h"

//Functions declaration
void Io_Init(led_dev_t * led_dev);
void Io_SetP(led_dev_t * led_dev,int32_t on_t,int32_t off_t,uint32_t repeat_num);
void Io_Start(led_dev_t * led_dev);
//�û�����LED����
led_dev_t led1 =
{
	.io_t.io_obj = 
					{	
						.IO_PortSel = GPIO_PORT_C,	//�û�������
						.IO_PinSel = GPIO_PIN_09,							//�û�������
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
 * ����˵��
 * on_time_ms: ����ʱ��
 * off_time_ms: Ϩ��ʱ��
 * repeat_num: ִ�д���
 * ˵����
 * ��1�����޴�����˸��on_time_ms > 0��off_time_ms > 0; repeat_num = 0xFF
 * ��2��������on_time_ms > 0; off_time_ms = 0; repeat_num = 0xFF
 * ��3����Ϩ��on_time_ms = 0; off_time_ms �� 0; repeat_num = 0xFF
 * ��4����repeat_numΪ0�������
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

//��ʵ��LED�����ܣ���֧����˸���ܣ���led_dev_t�еĲ�����
void Io_Start(led_dev_t * led_dev)
{
	led_dev->io_t.OutHandler(&led_dev->io_t,_LED_ON_);
#if 0
	led_dev->io_t.OutHandler(&led_dev->io_t,_LED_OFF_);
#endif
}
