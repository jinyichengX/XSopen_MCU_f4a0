#include "dlt645.h"
#include "dlt645Conf.h"
#include "dlt645_data.h"

void dlt645_set_addr(dlt645_t *ctx, uint8_t *addr)
{
	uint8_t i;
	uint8_t *p_addr = addr;
	
	memset(&ctx->addr,0,DLT645_ADDR_LEN);
	for(i = 0;i < DLT645_ADDR_LEN; i ++)
	{
		ctx->addr[DLT645_ADDR_LEN - i - 1] = *p_addr;
		p_addr ++;
	}
}

int dlt645_2007_read_data(dlt645_t *ctx,
                          uint32_t code,
                          uint8_t *read_data)
{
		int ret = 0;
    uint8_t send_buf[DLT2007_RD_CMD_LEN];
    uint8_t read_buf[50];
#if DLT07_USE_LEADING_HEAD== 1
		uint8_t send_head[DLT07_LEADING_HEAD_LEN];
		memset(send_head,0xfe,DLT07_LEADING_HEAD_LEN);
#endif	
    memset(read_buf, 0, sizeof(read_buf));
    memset(send_buf, 0, sizeof(send_buf));

		/* send_buf填充数据帧 */
		send_buf[DLT645_START_CODE_POS] = DLT645_START_CODE;
		send_buf[DLT645_START_CODE_POS1] = DLT645_START_CODE;
	
    memcpy(send_buf + 1, ctx->addr, DLT645_ADDR_LEN);

    send_buf[DLT2007_CTL_CMD_POS] = DLT2007_RD_CODE;
	
    send_buf[DLT2007_DATA_FIELD_POS] = 4;

    uint8_t send_code[4] = {0};
    send_code[3] = (code & 0xff) + 0x33;
    send_code[2] = ((code >> 8) & 0xff) + 0x33;
    send_code[1] = ((code >> 16) & 0xff) + 0x33;
    send_code[0] = ((code >> 24) & 0xff) + 0x33;

    memcpy(send_buf + DLT2007_DATA_FIELD_POS, send_code, 4);
		
		*(send_buf+DLT2007_RD_CMD_LEN-1) = DLT645_STOP_CODE;
		*(send_buf+DLT2007_RD_CMD_LEN-2) = _crc_sum(send_buf, DLT2007_RD_CMD_LEN-2);

#if DLT07_USE_LEADING_HEAD== 1
		ctx->send(ctx,send_head,DLT07_LEADING_HEAD_LEN);
#endif	
		ctx->send(send_buf,DLT2007_RD_CMD_LEN);
		
		if(ctx->recv(ctx,read_buf))
		{
//			ret = dlt645_data_parsing(code,read_buf);
		}

		return ret;
}

