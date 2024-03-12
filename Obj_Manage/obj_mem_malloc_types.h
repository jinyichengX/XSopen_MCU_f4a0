#include "stdio.h"
#include "string.h"
#include "xs_dpmcr.h"

#define xs_memset memset
#define bzero 0

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

#define xs_OS_MaxConfprionum (2^(sizeof(xs_NodeId_type)*8))
	
