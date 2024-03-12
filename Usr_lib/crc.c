#include "crc.h"

/**
 * Name:    _Ecrc_sum
 * Brief:   crc��У��
 * Input:
 *  @msg:   У�����ݰ�
 *  @len:   ���ݰ�����
 * Output:  У��ֵ
 */
uint8_t _Ecrc_sum(uint8_t *msg, uint32_t len)
{
    uint8_t crc = 0;
    while (len--)
    {
        crc += *msg++;
    }
    return crc;
}

/**
 * Name:    _Ecrc_xor
 * Brief:   crc���У��
 * Input:
 *  @msg:   У�����ݰ�
 *  @len:   ���ݰ�����
 * Output:  У��ֵ
 */
uint8_t _Ecrc_xor(uint8_t *Buf,uint8_t Len)
{
  uint8_t i =0;
  uint8_t x =0;
  
  for(i=0; i<Len; i++)
  {
    x = x^(*(Buf+i));
  }
  
  return x;
}
