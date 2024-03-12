#include "./common.h"

#define CUR_MAX_FILTER 5
#define CUR_FILT_PERIOD 10

static uint16_t filter_buf[128*(CUR_MAX_FILTER + 1)] = {0};							
							
//�����˲�
uint16_t Cur_Filter(uint8_t max_filter,uint16_t filt_period) 
{							

	static uint8_t tim_count = 0;																
																	
	uint8_t i,samp_channel;uint32_t filter_sum = 0;																
																	
	tim_count ++ ;																
																	
	max_filter = CUR_MAX_FILTER;filt_period = CUR_FILT_PERIOD;																
																	
	if(tim_count >= filt_period / 65)																
	{																
		for(samp_channel = 0;samp_channel < 128 ;samp_channel++) //��·������ѯ����															
		{															
			filter_sum = 0;														
																	
//			filter_buf[(max_filter+1)*samp_channel+max_filter] = Current.realCur[samp_channel];														
																	
			for(i = 0; i < max_filter; i++) 											 //12�������ۼ�			
			{														
				filter_buf[(max_filter+1)*samp_channel + i] = filter_buf[(max_filter+1)*samp_channel+ i + 1]; // �����������ƣ���λ�Ե�													
																	
				filter_sum += filter_buf[(max_filter+1)*samp_channel+i];				//�����ֵ									
			}														
//			Current.filter_realCur[samp_channel] = filter_sum / CUR_MAX_FILTER;	//����												
		}															
		tim_count = 0;															
	}																
	return 0;																
}																	

//�ֽ���ת��Ϊ������
float Byte2float(char *data)													
{													
	float f;												
	int i;												
	char *pdata;												//����һ��ָ��char�����ݵĵ�ַ��ָ��
													
	typedef union												
	{												
			float fdata;										
			unsigned char Bdata[4];										
													
	}uu;												
	uu SUI;												
													
	pdata = data;												
	for(i=0;i<4;i++)												
	{												
		SUI.Bdata[i] =  *(pdata+3-i); //��char�ͷ���������Bdata����											
	}												
	f = (float)SUI.fdata; 					//������ֵ��ֵ							
													
	return f;												
}													

//�������Сƽ��ֵ(32λ����)
Data_MIn_Max_Avg GenerateAvg_Max_MinCur(uint32_t* pCur,uint32_t len)			
{			
	uint8_t i = 0;		
	Data_MIn_Max_Avg Temp;		
	uint32_t total = 0;		
	uint32_t min = *pCur;		
	//�������ֵ		
	uint32_t max = *pCur;		
	for (i = 0; i < 9; i++) {		
		if ( (*pCur) > max ) {	
			max = *pCur;
		}	
		pCur++;	
	}		
			
	pCur -= 9;		
	//������Сֵ		
	for (i = 0; i < 9; i++) {		
		if ( (*pCur) < min ) {	
			min = *pCur;
		}	
		pCur++;	
	}		
			
	pCur -= 9;		
	//����ƽ��ֵ		
	for(i = 0;i < 9;i++)		
	{		
		total = total+(*pCur);	
		pCur++;	
	}		
	total = total-max;		
	Temp.AvgCr = total/9;		
	Temp.MaxCur = max;		
	Temp.MinCur = min;		
			
	return Temp;		
}			
