/**
 *******************************************************************************
 * @file  gpio/gpio_output/source/main.c
 * @brief Main program of GPIO for the Device Driver Library.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2022-03-31       CDT             First version
 @endverbatim
 *******************************************************************************
 * Copyright (C) 2022, Xiaohua Semiconductor Co., Ltd. All rights reserved.
 *
 * This software component is licensed by XHSC under BSD 3-Clause license
 * (the "License"); You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                    opensource.org/licenses/BSD-3-Clause
 *
 *******************************************************************************
 */

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "main.h"
#include "systick.h"
#include "bsp_uart.h"
#include "bsp_timer0.h"
#include "bsp_timer2.h"
#include "bsp_extint_key.h"
#include "bsp_sdio.h"
#include "bsp_rtc.h"
#include "bsp_ots.h"
#include "bsp_dac.h"
#include "is62wv51216.h"
#include "ev_hc32f4a0_lqfp176_tca9539.h"
#include "wdt.h"
#include "led.h"
#include "at24cxx.h"
#include "sd.h"
#include "w25qxx.h"

#include "ff.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "croutine.h"
#include "event_groups.h"

#include "ethernetif.h"
#include "lwip/init.h"
#include "lwip/netif.h"
#include "lwip/timeouts.h"
#include "netif/etharp.h"
#include "ethernetif.h"
#include "lwip/debug.h"
#include "lwip/tcp.h"
#include "lwip/apps/httpd.h"
#include "lwip/udp.h"
#include "tcp_echo.h"
#include "udp_echo.h"
/**
 * @addtogroup HC32F4A0_DDL_Examples
 * @{
 */

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define EXAMPLE_PERIPH_WE               (LL_PERIPH_GPIO | LL_PERIPH_EFM | LL_PERIPH_FCG | \
                                         LL_PERIPH_PWC_CLK_RMU | LL_PERIPH_SRAM)
#define EXAMPLE_PERIPH_WP               (LL_PERIPH_EFM | LL_PERIPH_FCG | LL_PERIPH_SRAM)
/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
 
void BSP_CLK_Init(void);
void FileSystem_Test(void);

char *uart_test_string = "GTA6666456456456456456456456456456456456456456456456486456481582222456456454564564558888GTA6666456456456456456456456456456456456456456456456486456481582222456456454564564558888";
volatile void * test_ptr_void;
#define TEST_SIZE 128
#define W25_TEST_SIZE 256
int uart_printf_count = 0;
uint8_t at24cxx_txbuff[TEST_SIZE];
uint8_t at24cxx_rxbuf[TEST_SIZE] = {0};

uint8_t w25qxx_txbuff[W25_TEST_SIZE*2];
uint8_t w25qxx_rxbuff[W25_TEST_SIZE*2] = {0};

int32_t i32MultiBlockRet;
int32_t i32EraseRet = LL_OK;

static FATFS FlashFatFs; /* File system object */
static FIL TestFile; /* File object */
/* A work buffer for the fW_mkfs() */
volatile static uint8_t u8WorkBuffer[FF_MAX_SS];
MKFS_PARM opt;
char FlashPath[] = "0:";
FRESULT fRet;
uint32_t u32WBNbr, u32RBNbr;
uint8_t u8WriteText[] = "这是一个基于Nor的文件系统测试";
char u8ReadText[100];
int32_t i32Ret = LL_ERR;

//static FATFS SDFatFs; /* File system object */
//static FIL TestFile1; /* File object */
//MKFS_PARM opt1;
//uint32_t u32WBNbr1, u32RBNbr1;
//FRESULT fRet1;
//char SDPath[] = "1:";
//volatile static uint8_t u8WorkBuffer1[FF_MAX_SS];
//uint8_t u8WriteText1[] = "这是一个基于SD的文件系统测试";
//char u8ReadText1[100];
//int32_t i32Ret1 = LL_ERR;
/*
*************************************
						任务函数声明
*************************************
*/
extern void vTaskUart1(void *pvParameters);
extern void vTaskLED(void *para);
//extern void vTaskStart(void *pvParameters);

/* 创建任务 */
static void AppTaskCreate (void);
/* 创建任务通信机制 */
static void AppObjCreate(void);
/*
*************************************
						任务句柄
*************************************
*/
TaskHandle_t xHandleTaskRemote = NULL;
TaskHandle_t xHandleTaskControl = NULL;
TaskHandle_t xHandleTaskStart = NULL;

/* 创建任务间通信机制事件标志 */
EventGroupHandle_t EventGroup1 = NULL;
EventGroupHandle_t EventGroup2 = NULL;
EventGroupHandle_t EventGroup3 = NULL;
EventGroupHandle_t EventGroup4 = NULL;

 /*Static IP ADDRESS: IP_ADDR0.IP_ADDR1.IP_ADDR2.IP_ADDR3 */
 #define IP_ADDR0                    192
 #define IP_ADDR1                    168
 #define IP_ADDR2                      1
 #define IP_ADDR3                    122

 /*NETMASK*/
 #define NETMASK_ADDR0               255
 #define NETMASK_ADDR1               255
 #define NETMASK_ADDR2               255
 #define NETMASK_ADDR3                 0

 /*Gateway Address*/
 #define GW_ADDR0                    192
 #define GW_ADDR1                    168
 #define GW_ADDR2                      1
 #define GW_ADDR3                      1
static struct netif gnetif;
//void SysTick_Handler(void)
//{
//	SysTick_IncTick();
//}
void LwipTick_Config(void)
{
	/* SysTick configuration */
	(void)SysTick_Init(1000U);
	/* Set Systick Interrupt to the highest priority */
	NVIC_SetPriority(SysTick_IRQn, DDL_IRQ_PRIO_00);
}
 
static void Netif_Config(void)
{
	ip_addr_t ipaddr;
	ip_addr_t netmask;
	ip_addr_t gw;

	IP_ADDR4(&ipaddr, IP_ADDR0, IP_ADDR1, IP_ADDR2, IP_ADDR3);
	IP_ADDR4(&netmask, NETMASK_ADDR0, NETMASK_ADDR1, NETMASK_ADDR2,NETMASK_ADDR3);
	IP_ADDR4(&gw, GW_ADDR0, GW_ADDR1, GW_ADDR2, GW_ADDR3);
	
	/* Initialize the LwIP stack */
	lwip_init();
	/* Add the network interface */
	(void)netif_add(&gnetif, &ipaddr, &netmask, &gw, NULL, &ethernetif_init,&ethernet_input);
	/* Registers the default network interface */
	netif_set_default(&gnetif);
	if (netif_is_link_up(&gnetif))
	{
	/* When the netif is fully configured this function must be called */
	netif_set_up(&gnetif);
	}
	else
	{
	/* When the netif link is down this function must be called */
	netif_set_down(&gnetif);
	}
	/* Set the link callback function, this function is called on change of link status*/
	netif_set_link_callback(&gnetif, EthernetIF_LinkCallback);
}
/**
 * @brief  Main function of GPIO project
 * @param  None
 * @retval int32_t return value, if needed
 */
int main(void)
{
	__set_PRIMASK(1);
	/* System clock init */
	BSP_CLK_Init();
	
	/* Peripheral registers write unprotected */
	LL_PERIPH_WE(EXAMPLE_PERIPH_WE);
	
	/* Peripheral init begin */
	led_ioctl.ioInit(&led1);
	led_ioctl.IoOutLoop(&led1);
	
	g_uart.fp_setup(&usart1);
	g_uart.fp_send(&usart1,(uint8_t *)uart_test_string,strlen(uart_test_string),dma);
	/* Peripheral init end */
	
	/* Peripheral registers write protected */
	LL_PERIPH_WP(EXAMPLE_PERIPH_WP);
	__set_PRIMASK(0);

	/* 创建任务 */
	AppTaskCreate();
	
	/* 创建任务通信机制 */
	AppObjCreate();
	
	/* 启动调度， 开始执行任务 */
	vTaskStartScheduler();

	while(1)
	{
		/* Read a received packet from the ETH buffers and send it
		to the lwIP for handling */
//		ethernetif_input(&gnetif);
		/* Handle timeouts */
//		sys_check_timeouts();
		/* Handle periodic timers */
//		LwIP_PeriodicHandle(&gnetif);
	}
}

/*
*********************************************************************************************************
* 函 数 名: AppTaskCreate
* 功能说明: 创建应用任务
* 形 参: 无
* 返 回 值: 无
*********************************************************************************************************
*/
static void AppTaskCreate (void)
{
	xTaskCreate(	vTaskUart1,	 		/* 任务函数 */
								"vTaskUart1", 				/* 任务名 */
								512,					 				/* 任务栈大小，单位 word，也就是 4 字节 */
								NULL, 								/* 任务参数 */
								1, 										/* 任务优先级*/
								&xHandleTaskRemote ); /* 任务句柄 */
	
	xTaskCreate( 	vTaskLED, 						/* 任务函数 */
								"vTaskLED", 					/* 任务名 */
								512, 									/* 任务栈大小，单位 word，也就是 4 字节 */
								NULL, 								/* 任务参数 */
								2, 										/* 任务优先级*/
								&xHandleTaskControl );	 	/* 任务句柄 */
	
//	xTaskCreate( 	vTaskStart, 					/* 任务函数 */
//								"vTaskStart", 				/* 任务名 */
//								512, 									/* 任务栈大小，单位 word，也就是 4 字节 */
//								NULL, 								/* 任务参数 */
//								4, 										/* 任务优先级*/
//								&xHandleTaskStart ); 	/* 任务句柄 */
}

void AppObjCreate(void)
{
	/* 创建事件标志组 */
	EventGroup1 = xEventGroupCreate();
	EventGroup2 = xEventGroupCreate();
	EventGroup3 = xEventGroupCreate();
	EventGroup4 = xEventGroupCreate();
	
	if((EventGroup1 == NULL)||(EventGroup2 == NULL)||(EventGroup3 == NULL)||(EventGroup4 == NULL))
	{
		/* 没有创建成功，用户可以在这里加入创建失败的处理机制 */
	}
}
/**
 * @brief  BSP clock initialize.
 *         SET board system clock to PLLH@240MHz
 *         Flash: 5 wait
 *         SRAM_HS: 1 wait
 *         SRAM1_2_3_4_B: 2 wait
 *         PCLK0: 240MHz
 *         PCLK1: 120MHz
 *         PCLK2: 60MHz
 *         PCLK3: 60MHz
 *         PCLK4: 120MHz
 *         EXCLK: 120MHz
 *         HCLK:  240MHz
 * @param  None
 * @retval None
 */
static void BSP_CLK_Init(void)
{
		stc_clock_xtal_init_t stcXtalInit;
    stc_clock_pll_init_t stcPLLHInit;
		stc_clock_xtal32_init_t stcXtal32Init;
		/* MCU Peripheral registers write unprotected. */
    LL_PERIPH_WE(LL_PERIPH_GPIO | LL_PERIPH_FCG | LL_PERIPH_PWC_CLK_RMU | \
                 LL_PERIPH_EFM | LL_PERIPH_SRAM);
    CLK_SetClockDiv(CLK_BUS_CLK_ALL,
                    (CLK_PCLK0_DIV1 | CLK_PCLK1_DIV2 | CLK_PCLK2_DIV4 |
                     CLK_PCLK3_DIV4 | CLK_PCLK4_DIV2 | CLK_EXCLK_DIV2 |
                     CLK_HCLK_DIV1));
    (void)CLK_XtalStructInit(&stcXtalInit);
    /* Config Xtal and enable Xtal */
    stcXtalInit.u8Mode   = CLK_XTAL_MD_OSC;
    stcXtalInit.u8Drv    = CLK_XTAL_DRV_ULOW;
    stcXtalInit.u8State  = CLK_XTAL_ON;
    stcXtalInit.u8StableTime = CLK_XTAL_STB_2MS;
    (void)CLK_XtalInit(&stcXtalInit);

    (void)CLK_PLLStructInit(&stcPLLHInit);
    /* VCO = (8/1)*120 = 960MHz*/
    stcPLLHInit.u8PLLState      = CLK_PLL_ON;
    stcPLLHInit.PLLCFGR         = 0UL;
    stcPLLHInit.PLLCFGR_f.PLLM  = 1UL - 1UL;
    stcPLLHInit.PLLCFGR_f.PLLN  = 120UL - 1UL;//100-1
    stcPLLHInit.PLLCFGR_f.PLLP  = 4UL - 1UL;
    stcPLLHInit.PLLCFGR_f.PLLQ  = 4UL - 1UL;
    stcPLLHInit.PLLCFGR_f.PLLR  = 4UL - 1UL;
    stcPLLHInit.PLLCFGR_f.PLLSRC = CLK_PLL_SRC_XTAL;
    (void)CLK_PLLInit(&stcPLLHInit);

		(void)CLK_Xtal32StructInit(&stcXtal32Init);
    /* Configure Xtal32 */
    stcXtal32Init.u8State   = CLK_XTAL32_ON;
    stcXtal32Init.u8Drv     = CLK_XTAL32_DRV_MID;
    stcXtal32Init.u8Filter  = CLK_XTAL32_FILTER_OFF;
    (void)CLK_Xtal32Init(&stcXtal32Init);
		
    /* Highspeed SRAM set to 1 Read/Write wait cycle */
    SRAM_SetWaitCycle(SRAM_SRAMH, SRAM_WAIT_CYCLE1, SRAM_WAIT_CYCLE1);
    /* SRAM1_2_3_4_backup set to 2 Read/Write wait cycle */
    SRAM_SetWaitCycle((SRAM_SRAM123 | SRAM_SRAM4 | SRAM_SRAMB), SRAM_WAIT_CYCLE2, SRAM_WAIT_CYCLE2);
		
    /* 0-wait @ 40MHz */
    EFM_SetWaitCycle(EFM_WAIT_CYCLE5);
    /* 4 cycles for 200 ~ 250MHz */
    GPIO_SetReadWaitCycle(GPIO_RD_WAIT4);
    CLK_SetSysClockSrc(CLK_SYSCLK_SRC_PLL);
		//		/* MCU Peripheral registers write protected. */
    LL_PERIPH_WP(LL_PERIPH_GPIO | LL_PERIPH_FCG | LL_PERIPH_PWC_CLK_RMU | \
                 LL_PERIPH_EFM | LL_PERIPH_SRAM);
}
/**
 * @}
 */
void FileSystem_Test(void)
{
#if !USE_SD_FOR_FATFS
	int i;
	i32EraseRet      = SdCard_Erase();
	i32MultiBlockRet = SdCard_RdWrMultiBlockTest();

	if ((LL_OK != i32EraseRet) || (LL_OK != i32MultiBlockRet))	
	{	
		for(i = 0;i<4;i++)
		{
			DDL_DelayMS(1000);
			Led1_Toggle;
		}
	}

#else	
	
	/* Register the file system object to the FatFs module */
	if (FR_OK != f_mount(&FlashFatFs, (TCHAR const*)FlashPath, 0U))
	{
		usr_debug_printf("FatFs Initialization Error!\r\n");
	}
	else
	{
		(void)memset(&opt, 0, sizeof(MKFS_PARM));
		opt.fmt = (BYTE)FM_ANY;
		/* Create a FAT file system (format) on the logical drive */
		if (FR_OK != f_mkfs((TCHAR const*)FlashPath, &opt, (void *)u8WorkBuffer,
												sizeof(u8WorkBuffer)))
		{
			usr_debug_printf("FatFs Format Error!\r\n");
		}
		else
		{
			/* Create and Open a new text file object with write access */
			if (FR_OK != f_open(&TestFile, "0:Test.txt", ((BYTE)FA_CREATE_ALWAYS |
			(BYTE)FA_WRITE)))
			{
				usr_debug_printf("\"Test.txt\" file Open for write Error!\r\n");
			}
			else
			{
				/* Write data to the text file */
				fRet = f_write(&TestFile, u8WriteText, sizeof(u8WriteText), (void *)&u32WBNbr);
				if ((0UL == u32WBNbr) || (FR_OK != fRet))
				{
					usr_debug_printf("\"Test.txt\" file Write or EOF Error!\r\n");
				}
				else
				{
					/* Close the open text file */
					(void)f_close(&TestFile);
					/* Open the text file object with read access */
					if (FR_OK != f_open(&TestFile, "0:Test.txt", (BYTE)FA_READ))
					{
						usr_debug_printf("\"Test.txt\" file Open for read Error!\r\n");
					}
					else
					{
						(void)memset(u8ReadText, 0, sizeof(u8ReadText));
						/* Read data from the text file */
						fRet = f_read(&TestFile, u8ReadText, sizeof(u8ReadText),
						(UINT*)(uint32_t)&u32RBNbr);
						if ((0UL == u32RBNbr) || (FR_OK != fRet))
						{
							usr_debug_printf("\"Test.txt\" file Read or EOF Error!\r\n");
						}
						else
						{
							/* Close the open text file */
							(void)f_close(&TestFile);
							/* Compare read data with the expected data */
							if (u32RBNbr == u32WBNbr)
							{
							/* Check data value */
							if (0 == memcmp(u8WriteText, u8ReadText, u32RBNbr))
							{
							i32Ret = LL_OK;
							}
							}
						}
					}
				}
			}
		}
	}
	/* Unlink the micro SD disk I/O driver */
	f_mount(NULL, (TCHAR const*)FlashPath, 0U);
	if (LL_OK != i32Ret)
	{
		/* Test failed */
		usr_debug_printf("\"Test.txt\" file Read ERROR!\r\n");
	}
	else
	{
		/* Test success */
		usr_debug_printf("\"Test.txt\" file Read OK!\r\n");
		usr_debug_printf("Read buf is:\n");
		usr_debug_printf(u8ReadText);
	}
#endif
}
/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/


