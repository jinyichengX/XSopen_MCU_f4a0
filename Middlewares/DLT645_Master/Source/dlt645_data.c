#include "dlt645_data.h"


/**
 * Name:    _crc
 * Brief:   crc和校验
 * Input:
 *  @msg:   校验数据包
 *  @len:   数据包长度
 * Output:  校验值
 */
uint8_t _crc_sum(uint8_t *msg, uint32_t len)
{
    uint8_t crc = 0;
    while (len--)
    {
        crc += *msg++;
    }
    return crc;
}


