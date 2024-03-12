#include "obj_mem_malloc.h"

#define  MEM_BLOCK_SIZE    128//ÿ���ڴ���ֽ�
#define  MEM_MAX_SIZE  32*1024  //�ڴ���ܴ�С 32K�ֽ� Ҫ���ڴ���������
#define  MEM_NUM_DEFAULT 1

__align(4) u8 membase[MEM_MAX_SIZE];	//�ڴ������ 4�ֽڶ���	
u16 memmapbase[MEM_MAX_SIZE/MEM_BLOCK_SIZE];	//�ڴ�״̬��											

//�����ڴ�
//*s:�ڴ��׵�ַ
//c :Ҫ���õ�ֵ
//count:��Ҫ���õ��ڴ��С(�ֽ�Ϊ��λ)
void mymemset(void *s,u8 c,u32 count)  
{  
    u8 *xs = s;  
    while(count--)*xs++=c;  
}	   
//�ڴ�����ʼ��  
//memx:�����ڴ��
void my_mem_init(u8 memx)  
{  
    mymemset(membase, 0,MEM_MAX_SIZE  );//�ڴ�״̬����������							
}  

//memx:�����ڴ��
//size:Ҫ������ڴ��С(�ֽ�)
//����ֵ:0XFFFFFFFF,�������;����,�ڴ�ƫ�Ƶ�ַ 
u32 my_mem_malloc(u8 memx,u32 size)  
{  
    signed long offset=0;  
    u32 nmemb;	//��Ҫ���ڴ����  
	u32 cmemb=0;//�������ڴ����
    u32 i;   
    if(size==0)return 0XFFFFFFFF;//����Ҫ����
    nmemb=size/MEM_BLOCK_SIZE;      	//��ȡ��Ҫ����������ڴ����
    if(size%MEM_BLOCK_SIZE)nmemb++;  
    for(offset=MEM_MAX_SIZE/MEM_BLOCK_SIZE;offset>=0;offset--)//���������ڴ������  
    {     
			if(memmapbase[offset]==0)cmemb++;//�������ڴ��������
			else cmemb=0;								//�����ڴ������
			if(cmemb==nmemb)							//�ҵ�������nmemb�����ڴ��
			{
							for(i=0;i<nmemb;i++)  					//��ע�ڴ��ǿ� 
							{  
									memmapbase[offset+i]=nmemb;  
							}  
							return (offset*MEM_BLOCK_SIZE);//����ƫ�Ƶ�ַ  
			}
    }  
    return 0XFFFFFFFF;//δ�ҵ����Ϸ����������ڴ��  
}  
//�ͷ��ڴ�(�ڲ�����) 
//memx:�����ڴ��
//offset:�ڴ��ַƫ��
//����ֵ:0,�ͷųɹ�;1,�ͷ�ʧ��;  
u8 my_mem_free(u8 memx,u32 offset)  
{  
    int i;  
    if(offset<MEM_MAX_SIZE)//ƫ�����ڴ����. 
    {  
        int index=offset/MEM_BLOCK_SIZE;			//ƫ�������ڴ�����  
        int nmemb=memmapbase[index];	        //�ڴ������
        for(i=0;i<nmemb;i++)  						//�ڴ������
        {  
            memmapbase[index+i]=0;  
        }  
        return 0;  
    }else return 2;//ƫ�Ƴ�����.  
}  
//�ͷ��ڴ�(�ⲿ����) 
//memx:�����ڴ��
//ptr:�ڴ��׵�ַ 
void myfree(u8 memx,void *ptr)  
{  
	u32 offset;   
	if(ptr==NULL)return;//��ַΪ0.  
 	offset=(u32)ptr-(u32)membase;     
    my_mem_free(memx,offset);	//�ͷ��ڴ�      
}  
//�����ڴ�(�ⲿ����)
//memx:�����ڴ��
//size:�ڴ��С(�ֽ�)
//����ֵ:���䵽���ڴ��׵�ַ.
void *mymalloc(u8 memx,u32 size)  
{  
    u32 offset;   
	offset=my_mem_malloc(memx,size);  	   	 	   
    if(offset==0XFFFFFFFF)return NULL;  
    else return (void*)((u32)membase+offset);  
}  

