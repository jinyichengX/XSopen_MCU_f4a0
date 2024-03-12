#ifndef __BSP_SDIO_H__
#define __BSP_SDIO_H__

#ifdef __cplusplus
extern "C"
#endif

#include "hc32_ll.h"
#include "hc32_ll_gpio.h"
#include "sd.h"
#include "diskio.h"
#include "ff.h"
#include <string.h>

#define USE_SD_FOR_FATFS 1
/* SD transfer mode */
#define SD_TRANS_MD_POLLING             (0U)
#define SD_TRANS_MD_INT                 (1U)
#define SD_TRANS_MD_DMA                 (2U)
/* Populate the following macro with an value, reference "SD transfer mode" */
#define SD_TRANS_MD                     (SD_TRANS_MD_POLLING)

/* SDIOC DMA configuration define */
#define SDIOC_DMA_UNIT                  (CM_DMA1)
#define SDIOC_DMA_CLK                   (FCG0_PERIPH_DMA1 | FCG0_PERIPH_AOS)
#define SDIOC_DMA_TX_CH                 (DMA_CH0)
#define SDIOC_DMA_RX_CH                 (DMA_CH1)
#define SDIOC_DMA_TX_TRIG_CH            (AOS_DMA1_0)
#define SDIOC_DMA_RX_TRIG_CH            (AOS_DMA1_1)
#define SDIOC_DMA_TX_TRIG_SRC           (EVT_SRC_SDIOC1_DMAW)
#define SDIOC_DMA_RX_TRIG_SRC           (EVT_SRC_SDIOC1_DMAR)

/* SDIOC configuration define */
#define SDIOC_SD_UINT                   (CM_SDIOC1)
#define SDIOC_SD_CLK                    (FCG1_PERIPH_SDIOC1)
#define SIDOC_SD_INT_SRC                (INT_SRC_SDIOC1_SD)
#define SIDOC_SD_IRQ                    (INT006_IRQn)
/* CK = PC12 */
#define SDIOC_CK_PORT                   (GPIO_PORT_C)
#define SDIOC_CK_PIN                    (GPIO_PIN_12)
/* CMD = PD02 */
#define SDIOC_CMD_PORT                  (GPIO_PORT_D)
#define SDIOC_CMD_PIN                   (GPIO_PIN_02)
/* D0 = PB07 */
#define SDIOC_D0_PORT                   (GPIO_PORT_B)
#define SDIOC_D0_PIN                    (GPIO_PIN_07)
/* D1 = PA08 */
#define SDIOC_D1_PORT                   (GPIO_PORT_A)
#define SDIOC_D1_PIN                    (GPIO_PIN_08)
/* D2 = PC10 */
#define SDIOC_D2_PORT                   (GPIO_PORT_C)
#define SDIOC_D2_PIN                    (GPIO_PIN_10)
/* D3 = PB05 */
#define SDIOC_D3_PORT                   (GPIO_PORT_B)
#define SDIOC_D3_PIN                    (GPIO_PIN_05)

#define SDIOC_CD_PORT										(GPIO_PORT_H)
#define SDIOC_CD_PIN										(GPIO_PIN_15)

/* SD card read/write/erase */
#define SD_CARD_BLK_SIZE                (512UL)
#define SD_CARD_BLK_NUM                 (10U)

/* SD read/write timeout time */
#define SD_RW_TIMEOUT_TIME 							(30000UL)

extern uint8_t u8WriteBlocks[SD_CARD_BLK_SIZE * SD_CARD_BLK_NUM];
extern uint8_t u8ReadBlocks[SD_CARD_BLK_SIZE * SD_CARD_BLK_NUM];

extern uint32_t SdCard_Config(void);
#if !USE_SD_FOR_FATFS
extern int32_t SD_Write(stc_sd_handle_t *handle, uint32_t u32BlockAddr, uint16_t u16BlockCount,uint8_t *pu8Data, uint32_t u32Timeout);
extern int32_t SD_Read(stc_sd_handle_t *handle, uint32_t u32BlockAddr, uint16_t u16BlockCount,uint8_t *pu8Data, uint32_t u32Timeout);
extern int32_t SdCard_Erase(void);
extern int32_t SdCard_RdWrMultiBlockTest(void);
#else
extern DRESULT SD_Initialize(BYTE lun);//SDHC初始化
extern DRESULT SD_Read(BYTE lun, BYTE *buff, DWORD sector, UINT count);//SDHC读出
extern DRESULT SD_Write(BYTE lun, const BYTE *buff, DWORD sector, UINT count);//SDHC写入
extern DRESULT SD_Ioctl(BYTE lun, BYTE cmd, void *buff);//SDHC信息获取
#endif
#ifdef __cplusplus
}
#endif
#endif /* __BSP_SDIO_H__ */

