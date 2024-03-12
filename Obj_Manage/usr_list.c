#include "usr_list.h"

xs_ListHdnode LCB;

xs_ListHdnode * xs_ListCreatList(xs_ListHdnode *hdInit)
{
    xs_ListHdnode *tmp = hdInit;

    xs_memset((uint8_t *)tmp,bzero,sizeof(xs_ListHdnode));

    tmp -> headerflag = 1ul;
    tmp -> xs_ListnodeStat = 1ul;
    tmp -> xs_nextNode = (xs_ListMdnode *)tmp;
    tmp -> xs_lastNode = (xs_ListMdnode *)tmp;
    tmp -> xs_xxxPtInst = NULL;

    return (xs_ListHdnode *)tmp;
}

xs_ListMdnode * xs_ListAddNodeInorder(xs_ListHdnode *ListRoot,xs_ListMdnode *mdNodeInst,xs_NodeId_type node_id)
{
    xs_ListMdnode * midtmp = mdNodeInst;

		if((ListRoot == NULL)||(node_id > xs_OS_MaxConfprionum)) return NULL;

    midtmp->sect_Id = node_id;
    midtmp->xs_lastNode = (xs_ListMdnode*)(ListRoot->xs_lastNode);
    midtmp->xs_lastNode->xs_nextNode = midtmp;
    if(ListRoot->xs_ListnodeStat>=2) {midtmp->xs_lastNode->tailFlag = 0;}
    midtmp->xs_nextNode = (xs_ListMdnode *)ListRoot;
    midtmp->xs_nextNode->xs_lastNode = (xs_ListMdnode *)midtmp;
    ListRoot->xs_ListnodeStat ++; midtmp->tailFlag = 1;
		
    return midtmp;
}

xs_ListMdnode * xs_ListAddNodePrio(xs_ListHdnode *ListRoot,xs_ListMdnode *mdNodeInst,xs_NodeId_type xs_priority)
{
	xs_ListMdnode * xs_pxNewtemp = mdNodeInst;
	xs_ListMdnode * xs_pxtrav = NULL;
	xs_NodeId_type travNum = 0;
	if((ListRoot == NULL)||(xs_priority > xs_OS_MaxConfprionum)) return NULL;
	
	if(ListRoot->xs_ListnodeStat < 2){
		xs_ListAddNodeInorder(ListRoot,mdNodeInst,xs_priority); return xs_pxNewtemp;
	}
	
	xs_pxNewtemp->sect_Id = xs_priority;
	
	//遍历链表找到适合节点插入的位置
	for(travNum = 0,xs_pxtrav = ListRoot->xs_nextNode;
			travNum < ListRoot->xs_ListnodeStat-1;
			travNum ++,xs_pxtrav = ListRoot->xs_nextNode->xs_nextNode){ 
		if(xs_pxtrav->sect_Id > xs_priority){ 
			xs_pxtrav = xs_pxtrav->xs_lastNode; break;}
	}
	
	//插入节点
	xs_pxNewtemp->xs_nextNode = xs_pxtrav->xs_nextNode;
	xs_pxtrav->xs_nextNode->xs_lastNode = xs_pxNewtemp;
	xs_pxtrav->xs_nextNode = xs_pxNewtemp;
	xs_pxNewtemp->xs_lastNode = xs_pxtrav;
	
	//链表节点计数自增
	ListRoot->xs_ListnodeStat ++;
	return xs_pxNewtemp;
}

xs_ListMdnode * xs_ListDelNode(xs_ListHdnode *ListRoot,xs_ListMdnode *mdNodeInst)
{
	xs_ListMdnode * xs_pxNewtemp = mdNodeInst;
	
	if((ListRoot == NULL)||(mdNodeInst == NULL)) return NULL;
	
	xs_pxNewtemp->xs_lastNode->xs_nextNode = xs_pxNewtemp->xs_nextNode;
	xs_pxNewtemp->xs_nextNode->xs_lastNode = xs_pxNewtemp->xs_lastNode;
	
	//链表节点计数自减
	ListRoot->xs_ListnodeStat ++;
	
	return xs_pxNewtemp;
}

xs_ListMdnode * xs_ListPresNodeMovTail(xs_ListHdnode *ListRoot,xs_ListMdnode *mdNodeInst)
{
	xs_ListMdnode * xs_pxNewtemp = mdNodeInst;
	
	xs_ListDelNode(ListRoot,mdNodeInst);
	xs_ListAddNodeInorder(ListRoot,mdNodeInst,xs_pxNewtemp->sect_Id);
	
	return xs_pxNewtemp;
}


