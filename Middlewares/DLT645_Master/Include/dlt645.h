#include <stdint.h>
#include <string.h>

#define DLT645_START_CODE 0x68
#define DLT645_STOP_CODE 0x16
/* DLT645设备地址长度,默认为6*/
#define DLT645_ADDR_LEN 6

#define DLT645_START_CODE_POS 0
#define DLT645_START_CODE_POS1 7

/* DLT645-2007相关 */
#define DLT2007_RD_CMD_LEN 16	//DLT645读指令长度
#define DLT2007_CTL_CMD_POS 8	//DLT645控制码位置
#define DLT2007_DATA_FIELD_LEN_POS 9//DLT645数据域长度位置
#define DLT2007_DATA_FIELD_POS 10//DLT645数据域位置
#define DLT2007_RD_CODE 	0X11//DLT645控制码（读）

typedef enum
{
	dlt_mode97,
	dlt_mode07
}dlt_mode_e;

typedef struct dlt645
{
	uint8_t addr[DLT645_ADDR_LEN];
	void (*send)(uint8_t * buf,uint8_t len);
	int (*recv)(struct dlt645 *,uint8_t * buf);
	dlt_mode_e dlt_mode;
}dlt645_t;
