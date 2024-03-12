#ifndef LED_H
#define LED_H

#ifdef __cplusplus
extern "C" {
#endif

#include "bsp_gpio.h"
#include "obj_mem_malloc.h"

#define _LED_ON_ 1
#define _LED_OFF_ 0

typedef struct led_dev_attr
{
	io_HandlerType io_t;

	uint32_t state;
	uint32_t on_time;
    uint32_t off_time;
	uint32_t repeat_num;
	uint32_t repeat_cnt;
}led_dev_t;

typedef struct ledLoopCtr
{
	void (*ioInit)(led_dev_t * led_dev);
	void (*IoSetPara)(led_dev_t * led_dev,int32_t on_t,int32_t off_t,uint32_t repeat_num);
	void (*IoOutLoop)(led_dev_t * led_dev);
	void (*IoCtl)(led_dev_t * led_dev);
}led_t;
extern led_t led_ioctl;

extern led_dev_t led1,led2,led3;//... ...
#ifdef __cplusplus
}
#endif
#endif

