#include "udp_echo.h"

static char * st_buffer= "We get a data\n";

void Udp_demo_callback(void *arg,
                             struct udp_pcb *upcb,
                             struct pbuf *p,
                             const ip_addr_t *addr,
                             u16_t port)
{
	/* ���͹̶�����"I have received your message,Thank you!" */
	struct pbuf *q = NULL;
	const char* reply = "I have received your message,Thank you!";

	pbuf_free(p);

	q = pbuf_alloc(PBUF_TRANSPORT, strlen(reply)+1, PBUF_RAM);
	
	if (!q)
		return;

	memset(q->payload, 0 , q->len);
	memcpy(q->payload, reply, strlen(reply));
	udp_sendto(upcb, q, addr, port);
	pbuf_free(q);
	
	/* �����յ��ı��� */
	//	ip_addr_t my_ipaddr;
	//	unsigned char *temp = (unsigned char *)addr;
	//	IP4_ADDR(&my_ipaddr, temp[0], temp[1], temp[2], temp[3]); // ����ԴIP
	//	udp_sendto(upcb, p, &my_ipaddr, port); // �����ķ��ظ�ԭ����
	//	pbuf_free(p);
}
 
void Udp_Init(void)
{
	struct udp_pcb *udpecho_pcb;
	/* �½�һ�����ƿ�*/
	udpecho_pcb = udp_new();

	if( udpecho_pcb == NULL )
		return;

	/* �󶨶˿ں� */
	if(udp_bind(udpecho_pcb, IP_ADDR_ANY, UDP_ECHO_PORT) != ERR_OK)
		return;

	/* ע��������ݻص����� */
	udp_recv(udpecho_pcb, Udp_demo_callback, (void *)st_buffer);
}

