#include <stdint.h>
#include <string.h>
#define xs_memset memset
#define bzero 0
typedef void(* xs_xxxPt)(void);
typedef unsigned int StackType_t;

typedef uint8_t univ_shor_typdef;

typedef univ_shor_typdef xs_NodeId_type;

#define xs_OS_MaxConfprionum (2^(sizeof(xs_NodeId_type)*8))
typedef struct xs_ListMdnode_t
{
    xs_NodeId_type sect_Id;
    struct xs_ListMdnode_t * xs_nextNode;
    struct xs_ListMdnode_t * xs_lastNode;
    xs_xxxPt xs_xxxPtInst;
    uint8_t tailFlag;
}xs_ListMdnode __attribute__((aligned(4)));

typedef struct
{
    uint8_t headerflag;
    uint8_t xs_ListnodeStat;
    univ_shor_typdef sect_Id;
    xs_ListMdnode* xs_nextNode;
    xs_ListMdnode* xs_lastNode;
    xs_xxxPt xs_xxxPtInst;
}xs_ListHdnode __attribute__((aligned(4)));

extern xs_ListHdnode LCB;

extern xs_ListHdnode * xs_ListCreatList(xs_ListHdnode *hdInit);
extern xs_ListMdnode * xs_ListAddNodeInorder(xs_ListHdnode *ListRoot,xs_ListMdnode *mdNodeInst,xs_NodeId_type node_id);
extern xs_ListMdnode * xs_ListAddNodePrio(xs_ListHdnode *ListRoot,xs_ListMdnode *mdNodeInst,xs_NodeId_type xs_priority);
extern xs_ListMdnode * xs_ListDelNode(xs_ListHdnode *ListRoot,xs_ListMdnode *mdNodeInst);
extern xs_ListMdnode * xs_ListPresNodeMovTail(xs_ListHdnode *ListRoot,xs_ListMdnode *mdNodeInst);
