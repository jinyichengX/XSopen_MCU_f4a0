#include "ethernetif.h"
#include "lwip/init.h"
#include "lwip/netif.h"
#include "lwip/timeouts.h"
#include "netif/etharp.h"
#include "ethernetif.h"
//#include "app_ethernet.h"
#include "lwip/debug.h"
#include "lwip/tcp.h"
#include "lwip/apps/httpd.h"
#include "lwip/udp.h"

#include "hc32_ll.h"
#include "hc32f4xx_conf.h"
#include "system_hc32f4a0.h"

#include <string.h>


#define UDP_ECHO_PORT 5001

extern void Udp_Init(void);
extern void Udp_demo_callback(void *arg,
                             struct udp_pcb *upcb,
                             struct pbuf *p,
                             const ip_addr_t *addr,
                             u16_t port);
														 
