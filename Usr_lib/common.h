#ifndef COMMON_H
#define COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

#include "hc32_ll.h"

typedef struct 
{
	uint32_t AvgCr;
	uint32_t MaxCur;
	uint32_t MinCur;
}Data_MIn_Max_Avg;

extern Data_MIn_Max_Avg GenerateAvg_Max_MinCur(uint32_t* pCur,uint32_t len);

#endif

