#include "obj_mem_malloc.h"

#define  MEM_BLOCK_SIZE    128//每个内存块字节
#define  MEM_MAX_SIZE  32*1024  //内存池总大小 32K字节 要是内存块的整数倍
#define  MEM_NUM_DEFAULT 1

__align(4) u8 membase[MEM_MAX_SIZE];	//内存池数组 4字节对齐	
u16 memmapbase[MEM_MAX_SIZE/MEM_BLOCK_SIZE];	//内存状态表											

//设置内存
//*s:内存首地址
//c :要设置的值
//count:需要设置的内存大小(字节为单位)
void mymemset(void *s,u8 c,u32 count)  
{  
    u8 *xs = s;  
    while(count--)*xs++=c;  
}	   
//内存管理初始化  
//memx:所属内存块
void my_mem_init(u8 memx)  
{  
    mymemset(membase, 0,MEM_MAX_SIZE  );//内存状态表数据清零							
}  

//memx:所属内存块
//size:要分配的内存大小(字节)
//返回值:0XFFFFFFFF,代表错误;其他,内存偏移地址 
u32 my_mem_malloc(u8 memx,u32 size)  
{  
    signed long offset=0;  
    u32 nmemb;	//需要的内存块数  
	u32 cmemb=0;//连续空内存块数
    u32 i;   
    if(size==0)return 0XFFFFFFFF;//不需要分配
    nmemb=size/MEM_BLOCK_SIZE;      	//获取需要分配的连续内存块数
    if(size%MEM_BLOCK_SIZE)nmemb++;  
    for(offset=MEM_MAX_SIZE/MEM_BLOCK_SIZE;offset>=0;offset--)//搜索整个内存控制区  
    {     
			if(memmapbase[offset]==0)cmemb++;//连续空内存块数增加
			else cmemb=0;								//连续内存块清零
			if(cmemb==nmemb)							//找到了连续nmemb个空内存块
			{
							for(i=0;i<nmemb;i++)  					//标注内存块非空 
							{  
									memmapbase[offset+i]=nmemb;  
							}  
							return (offset*MEM_BLOCK_SIZE);//返回偏移地址  
			}
    }  
    return 0XFFFFFFFF;//未找到符合分配条件的内存块  
}  
//释放内存(内部调用) 
//memx:所属内存块
//offset:内存地址偏移
//返回值:0,释放成功;1,释放失败;  
u8 my_mem_free(u8 memx,u32 offset)  
{  
    int i;  
    if(offset<MEM_MAX_SIZE)//偏移在内存池内. 
    {  
        int index=offset/MEM_BLOCK_SIZE;			//偏移所在内存块号码  
        int nmemb=memmapbase[index];	        //内存块数量
        for(i=0;i<nmemb;i++)  						//内存块清零
        {  
            memmapbase[index+i]=0;  
        }  
        return 0;  
    }else return 2;//偏移超区了.  
}  
//释放内存(外部调用) 
//memx:所属内存块
//ptr:内存首地址 
void myfree(u8 memx,void *ptr)  
{  
	u32 offset;   
	if(ptr==NULL)return;//地址为0.  
 	offset=(u32)ptr-(u32)membase;     
    my_mem_free(memx,offset);	//释放内存      
}  
//分配内存(外部调用)
//memx:所属内存块
//size:内存大小(字节)
//返回值:分配到的内存首地址.
void *mymalloc(u8 memx,u32 size)  
{  
    u32 offset;   
	offset=my_mem_malloc(memx,size);  	   	 	   
    if(offset==0XFFFFFFFF)return NULL;  
    else return (void*)((u32)membase+offset);  
}  

