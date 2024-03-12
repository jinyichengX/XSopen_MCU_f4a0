#include <stdint.h>
extern uint8_t _crc_sum(uint8_t *msg, uint32_t len);

typedef struct ammeter_data
{
	float total_power;
	float lastday_power;
	float lastmonth_power;
	uint32_t yymmddww;
	uint32_t hhmmss;
}at_data_t;

