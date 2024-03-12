#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

extern uint8_t _Ecrc_sum(uint8_t *msg, uint32_t len);
extern uint8_t _Ecrc_xor(uint8_t *Buf,uint8_t Len);
