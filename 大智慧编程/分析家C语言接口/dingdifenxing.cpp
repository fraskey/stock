
#include "stdafx.h"
#include "FxjFunc.h"
#include <stdlib.h>         
#include <malloc.h>

float GetXieLv(CALCINFO1* pstCalcInfo,int n1,int n2);

/* ============================BeginningOfHeader===============================
/ ����: GetOneTopSharp
/ ����:
	Ѱ�����ж�MatrixMax MatrixMin���cur_pos N1_pos��N2_pos��N1_pos<N2_pos<cur_pos������֮���Ƿ����ȷ���Ķ�����M_sharp����׼����Ϊ׼
	����ֵΪ0ʱ��ʾN1_pos��N2_pos���ڼ䲻���ڶ����ͣ����򷵻ض����͵�λ�ã�
	������ʽ���۱Ƚ���ߵ�Ķ����ͣ��ҿ��ǵ��׵�K�ߺϲ������
/ �汾: 1.0
/ ����: xuejiayong
/ �����ʼ���fraskey@126.com
/ ����: 2011-5-01
/===============================EndingOFHeader=================================*/

OUTPUT int GetOneTopSharp(INPUT const CALCINFO* pCalcInfo,
INPUT unsigned int N1_pos,INPUT unsigned int N2_pos,INPUT unsigned int M_sharp)
{
	int ret_flag = 0;
	unsigned int i;
	unsigned int max_pos;/*�����͵�λ��*/
	float max_count = 0;/*��������Ϊ��������˳�ʼ�������ֵΪ0�Ǻ����*/

	float	tem_high ;
	float	tem_low;
	unsigned int fengxing_cnt;

	/*���������Ч����֤*/
	if(NULL == pCalcInfo || NULL == pCalcInfo->m_pStkData )
	{
		ret_flag = 0;
		return ret_flag;
	}
	if(!((0<=N1_pos) && (N1_pos<N2_pos) 
		&& (0<=M_sharp) && (2*M_sharp-1+N1_pos) <=N2_pos))
	{
		ret_flag = 0;
		return ret_flag;
	}


    max_pos= N2_pos;
    max_count = pCalcInfo->m_pStkData[N1_pos].m_fHigh;

	/*��һ����Ѱ��N1_pos��N2_pos�ڼ�����ֵ�������ͱ������ֵΪ����*/
	for(i = N1_pos;i <= N2_pos ;i++)
	{
		if(pCalcInfo->m_pStkData[i].m_fHigh > max_count)
		{
			max_count = pCalcInfo->m_pStkData[i].m_fHigh;
			max_pos = i;
		}
				
	}


	/*�ڶ��������ڴ������ֵ��ʱ��ȷ���Ƿ�Ϊ�����͵���ߵ㣬�жϷ����ǿ������������Ƿ�ֱ��γ���M_sharp���ȵı�׼�½�����*/

	/*���ȼ�Ҫ�ж��Ƿ����㶥���͵ĳ����������Լ�����ν�ļ�����*/
	if( !((max_pos >= (M_sharp-1+N1_pos)) && (N2_pos>=(M_sharp-1+max_pos))))
	{
		ret_flag = 0;
		return ret_flag;
	}
	
	tem_high = pCalcInfo->m_pStkData[max_pos].m_fHigh;
	tem_low = pCalcInfo->m_pStkData[max_pos].m_fLow;
	fengxing_cnt = 0;
	/*���Ҳ�����*/
	for(i=max_pos ; i<=N2_pos ;i++)
	{

        /*�γ�һ����׼�½�����*/
        if( (pCalcInfo->m_pStkData[i].m_fHigh < tem_high) && (pCalcInfo->m_pStkData[i].m_fLow < tem_low))
        {
            fengxing_cnt ++;
            tem_high = pCalcInfo->m_pStkData[i].m_fHigh;
            tem_low = pCalcInfo->m_pStkData[i].m_fLow;
        
        }
#if 0
        else if((pCalcInfo->m_pStkData[i].m_fHigh >= tem_high) && (pCalcInfo->m_pStkData[i].m_fLow <= tem_low))
        {
            /*������ǰһ��K�ߣ��п��ܻ��ƻ�ǰ���γɵ����У�Ϊ�˱���һ���ԣ��򵥴�����������*/
            tem_high = pCalcInfo->m_pStkData[i].m_fHigh;
            tem_low = pCalcInfo->m_pStkData[i].m_fLow;
        }
        else if((pCalcInfo->m_pStkData[i].m_fHigh >=tem_high) && (pCalcInfo->m_pStkData[i].m_fLow >= tem_low))
        {
            /*�γ���һ����׼�����������ˣ�������������¿����������֧*/
            tem_high = pCalcInfo->m_pStkData[i].m_fHigh;
            tem_low = pCalcInfo->m_pStkData[i].m_fLow;
            
        }

        else
        {
            tem_high = pCalcInfo->m_pStkData[i].m_fHigh;
            tem_low = pCalcInfo->m_pStkData[i].m_fLow;

            /*��ǰһ��k����������do nothing*/
            ;
        }
#else

        if(pCalcInfo->m_pStkData[i].m_fHigh < tem_high)
        {
            tem_high = pCalcInfo->m_pStkData[i].m_fHigh ;
        }

        if(pCalcInfo->m_pStkData[i].m_fLow < tem_low)
        {
            tem_low = pCalcInfo->m_pStkData[i].m_fLow ;
        }

#endif
		/*�ұߵķ��������Ѿ��㹻���ˣ������ټ���������ȥ�ˣ���ʡ������*/
		if(fengxing_cnt > M_sharp-1)
		{
			break;
		}		
	}
	
	/*�Ҳ������γɵ�����£��ɶ������м���*/		
	if(fengxing_cnt > M_sharp-1)
	{
		tem_high = pCalcInfo->m_pStkData[N1_pos].m_fHigh;
		tem_low = pCalcInfo->m_pStkData[N1_pos].m_fLow;
		fengxing_cnt = 0;
		/*���������*/
		for(i =N1_pos  ; i <= max_pos ; i++)
		{


            /*�γ�һ����׼��������*/
            if( (pCalcInfo->m_pStkData[i].m_fHigh > tem_high) && (pCalcInfo->m_pStkData[i].m_fLow > tem_low) )
            {
                fengxing_cnt ++;
                tem_high = pCalcInfo->m_pStkData[i].m_fHigh;
                tem_low = pCalcInfo->m_pStkData[i].m_fLow;
            }

#if 0
            else if((pCalcInfo->m_pStkData[i].m_fHigh >=tem_high) && (pCalcInfo->m_pStkData[i].m_fLow <=tem_low))
            {
                /*������ǰһ��K�ߣ��п��ܻ��ƻ�ǰ���γɵ����У���˼򵥴�����������*/
                tem_high = pCalcInfo->m_pStkData[i].m_fHigh;
                tem_low = pCalcInfo->m_pStkData[i].m_fLow;
                
            }
            else if((pCalcInfo->m_pStkData[i].m_fHigh <=tem_high) && (pCalcInfo->m_pStkData[i].m_fLow <=tem_low))
            {
                /*�γ���һ����׼���½������ˣ�������������¿����������֧*/
                tem_high = pCalcInfo->m_pStkData[i].m_fHigh;
                tem_low = pCalcInfo->m_pStkData[i].m_fLow;
                
            }
            else
            {
                tem_high = pCalcInfo->m_pStkData[i].m_fHigh;
                tem_low = pCalcInfo->m_pStkData[i].m_fLow;

                /*��ǰһ��k����������do nothing*/
                ;
            }
#else

        if(pCalcInfo->m_pStkData[i].m_fHigh > tem_high)
        {
            tem_high = pCalcInfo->m_pStkData[i].m_fHigh ;
        }

        if(pCalcInfo->m_pStkData[i].m_fLow > tem_low)
        {
            tem_low = pCalcInfo->m_pStkData[i].m_fLow ;
        }


#endif

			/*��ߵķ��������Ѿ��㹻���ˣ������ټ���������ȥ�ˣ���ʡ��������ͬʱ��ȷ�����߶��γ����½����У��������ͳ���*/
			if(fengxing_cnt > M_sharp-1)
			{
				ret_flag = max_pos;
			}	

		
		}
	
	}			
		
	return ret_flag;
}




/* ============================BeginningOfHeader===============================
/ ����: GetOneBottomSharp
/ ����:
	Ѱ�����ж�MatrixMax MatrixMin���cur_pos N1_pos��N2_pos��N1_pos<N2_pos<cur_pos������֮���Ƿ����ȷ���ĵף���M_sharp����׼����Ϊ׼
	����ֵΪ0ʱ��ʾN1_pos��N2_pos���ڼ䲻���ڵ׷��ͣ����򷵻ص׷��͵�λ�ã�
	������ʽ���۱Ƚ���ߵ�Ķ����ͣ��ҿ��ǵ��׵�K�ߺϲ������
/ �汾: 1.0
/ ����: xuejiayong
/ �����ʼ���fraskey@126.com
/ ����: 2011-5-01
/===============================EndingOFHeader=================================*/
 int GetOneBottomSharp( const CALCINFO* pCalcInfo,
 unsigned int N1_pos, unsigned int N2_pos, unsigned int M_sharp)
{
	 int ret_flag = 0;
	 unsigned int i;
	 unsigned int min_pos;/*�׷��͵�λ��*/
	 float min_count = 100.0;/*��������Ϊ�ɼۣ���˳�ʼ�������ֵΪ10000�Ǻ����*/

	 float	tem_high ;
	 float	tem_low;
	 unsigned int fengxing_cnt;

	
	/*���������Ч����֤*/
	if(NULL == pCalcInfo || NULL == pCalcInfo->m_pStkData )
	{
		ret_flag = 0;
		return ret_flag;
	}
	if(!((0<=N1_pos) && (N1_pos<N2_pos) 
		&& (0<=M_sharp) && (2*M_sharp-1+N1_pos) <=N2_pos))
	{
		ret_flag = 0;
		return ret_flag;
	}

    min_pos = N2_pos;
    min_count = pCalcInfo->m_pStkData[N1_pos].m_fLow;
	/*��һ����Ѱ��N1_pos��N2_pos�ڼ����Сֵ���׷��ͱ�����СֵΪ�׵�*/
    //min_count = pCalcInfo->m_pStkData[N1_pos].m_fLow;
	for(i = N1_pos;i <= N2_pos ;i++)
	{
		if(pCalcInfo->m_pStkData[i].m_fLow < min_count)
		{
			min_count = pCalcInfo->m_pStkData[i].m_fLow;
			min_pos = i;
		}
	}

	/*�ڶ��������ڴ�����ֵ��ʱ��ȷ���Ƿ�Ϊ�׷��͵���͵㣬�жϷ����ǿ������������Ƿ�ֱ��γ���M_sharp���ȵı�׼��������*/

	/*���ȼ�Ҫ�ж��Ƿ�����׷��͵ĳ����������Լ�����ν�ļ�����*/
	if( !((min_pos >= (M_sharp-1+N1_pos)) && (N2_pos>=(M_sharp-1+min_pos))))
	{
		ret_flag = 0;
		return ret_flag;
	}

    tem_low = pCalcInfo->m_pStkData[min_pos].m_fLow;
	tem_high = pCalcInfo->m_pStkData[min_pos].m_fHigh;
	fengxing_cnt = 0;

	/*���Ҳ�����*/
	for(i = min_pos ; i <= N2_pos ;i++)
	{
		/*�γ�һ����׼��������*/
		if( (pCalcInfo->m_pStkData[i].m_fHigh > tem_high) && (pCalcInfo->m_pStkData[i].m_fLow > tem_low) )
		{
			fengxing_cnt ++;
			tem_high = pCalcInfo->m_pStkData[i].m_fHigh;
			tem_low = pCalcInfo->m_pStkData[i].m_fLow;
		}
#if 0
        else if((pCalcInfo->m_pStkData[i].m_fHigh >=tem_high) && (pCalcInfo->m_pStkData[i].m_fLow <=tem_low))
        {
            /*������ǰһ��K�ߣ��п��ܻ��ƻ�ǰ���γɵ����У���˼򵥴�����������*/
            tem_high = pCalcInfo->m_pStkData[i].m_fHigh;
            tem_low = pCalcInfo->m_pStkData[i].m_fLow;
            
        }
        else if((pCalcInfo->m_pStkData[i].m_fHigh <=tem_high) && (pCalcInfo->m_pStkData[i].m_fLow <=tem_low))
        {
            /*�γ���һ����׼���½������ˣ�������������¿����������֧*/
            tem_high = pCalcInfo->m_pStkData[i].m_fHigh;
            tem_low = pCalcInfo->m_pStkData[i].m_fLow;
            
        }
        else
        {
            tem_high = pCalcInfo->m_pStkData[i].m_fHigh;
            tem_low = pCalcInfo->m_pStkData[i].m_fLow;

            /*��ǰһ��k����������do nothing*/
            ;
        }
#else

        if(pCalcInfo->m_pStkData[i].m_fHigh > tem_high)
        {
            tem_high = pCalcInfo->m_pStkData[i].m_fHigh ;
        }

        if(pCalcInfo->m_pStkData[i].m_fLow > tem_low)
        {
            tem_low = pCalcInfo->m_pStkData[i].m_fLow ;
        }


#endif
		/*�ұߵķ��������Ѿ��㹻���ˣ������ټ���������ȥ�ˣ���ʡ������*/
		if(fengxing_cnt > M_sharp-1)
		{
			break;
		}	
	}
	
 	/*�Ҳ������γɵ�����£��ɶ������м��㣬Ӧ����һ���½�����*/		
	if(fengxing_cnt > M_sharp-1)
	{
		tem_high = pCalcInfo->m_pStkData[N1_pos].m_fHigh;
		tem_low = pCalcInfo->m_pStkData[N1_pos].m_fLow;
		fengxing_cnt = 0;
		/*���������*/
		for(i =N1_pos  ; i <= min_pos ;i++)
		{
			/*�γ�һ����׼�½�����*/
			if( (pCalcInfo->m_pStkData[i].m_fHigh < tem_high) && (pCalcInfo->m_pStkData[i].m_fLow < tem_low))
			{
				fengxing_cnt ++;
                tem_high = pCalcInfo->m_pStkData[i].m_fHigh;
                tem_low = pCalcInfo->m_pStkData[i].m_fLow;

			}
#if 0
            else if((pCalcInfo->m_pStkData[i].m_fHigh >= tem_high) && (pCalcInfo->m_pStkData[i].m_fLow <= tem_low))
            {
                /*������ǰһ��K�ߣ��п��ܻ��ƻ�ǰ���γɵ����У�Ϊ�˱���һ���ԣ��򵥴�����������*/
                tem_high = pCalcInfo->m_pStkData[i].m_fHigh;
                tem_low = pCalcInfo->m_pStkData[i].m_fLow;
            }
            else if((pCalcInfo->m_pStkData[i].m_fHigh >=tem_high) && (pCalcInfo->m_pStkData[i].m_fLow >= tem_low))
            {
                /*�γ���һ����׼�����������ˣ�������������¿����������֧*/
                tem_high = pCalcInfo->m_pStkData[i].m_fHigh;
                tem_low = pCalcInfo->m_pStkData[i].m_fLow;
                
            }

            else
            {
                tem_high = pCalcInfo->m_pStkData[i].m_fHigh;
                tem_low = pCalcInfo->m_pStkData[i].m_fLow;

                /*��ǰһ��k����������do nothing*/
                ;
            }
#else

        if(pCalcInfo->m_pStkData[i].m_fHigh < tem_high)
        {
            tem_high = pCalcInfo->m_pStkData[i].m_fHigh ;
        }

        if(pCalcInfo->m_pStkData[i].m_fLow < tem_low)
        {
            tem_low = pCalcInfo->m_pStkData[i].m_fLow ;
        }

#endif

			/*��ߵķ��������Ѿ��㹻���ˣ������ټ���������ȥ�ˣ���ʡ��������ͬʱ��ȷ�����߶��γ����½����У��������ͳ���*/
			if(fengxing_cnt > M_sharp-1)
			{
				ret_flag = min_pos;
			}

		}
	
	}			

	return ret_flag;
	
}

/* ============================BeginningOfHeader===============================
/ ����:CalcuDingDiFenXing
/ 
����:���㶥���͵Ķ��͵׷��͵ĵף�������������ȫ�ֱ�������gmatrix_dingdi[]�У���ֵ100��ʾ����-100��ʾ�ף�0��ʾ�Ƕ��ǵ�
/ �汾: 1.0
/ ����: xuejiayong
/ �����ʼ���fraskey@126.com
/ ����: 2011-5-01
/===============================EndingOFHeader=================================*/

OUTPUT CalcuDingDiFenXing(INPUT const CALCINFO* pCalcInfo,OUTPUT float* 
gmatrix_dingdi,INPUT unsigned int fengxin_time)
{
    unsigned int i;
    unsigned int former_dingdi_pos = 0;/*ǰһ���������ڵ�λ��*/
	unsigned int top_flag=0;
	unsigned int bottom_flag=0;
	unsigned int old_top_flag=0;
	unsigned int old_bottom_flag=0;

	unsigned int cur_top_flag=0;
	unsigned int cur_bottom_flag=0;

	/*��ѵ�������в���*/
	for(i=0;i<(unsigned int)pCalcInfo->m_nNumData;i++)
	{
		/*��ʼ����ǰ��λΪ�Ƕ��ǵ�*/
        gmatrix_dingdi[i] = 0;

		/*Ѱ�ҵ�һ�������ߵף�ͬʱ���ֶ��͵׵�ʱ�����ȳ��ֶ����ߵ���ȷ���Ķ����ߵ�,����λ��0ΪĬ�϶���*/
		if(!former_dingdi_pos)
		{
			/*��ǰλ��ֻ���ڴ��ڷ������и���ʱ�ſ����е�һ�����׳���*/
			if(i  >= (fengxin_time + former_dingdi_pos))
			{
				top_flag = GetOneTopSharp(pCalcInfo,former_dingdi_pos,i,fengxin_time);
				bottom_flag = GetOneBottomSharp(pCalcInfo,former_dingdi_pos,i,fengxin_time);

                /*���ֶ�������ʼ������Ϊȷ�ϵĵ�*/
                if(top_flag &&!bottom_flag)
                {
                        gmatrix_dingdi[0] = -100;
                        gmatrix_dingdi[top_flag] = 100;
                        old_bottom_flag = 0;
                        former_dingdi_pos = top_flag;
                        bottom_flag = 0;
                        old_top_flag = 0;                       
               }


                 /*���ֵף�����ʼ������Ϊȷ�ϵĶ�*/
                 if(!top_flag &&bottom_flag)
                 {
                         gmatrix_dingdi[0] = 100;
                         gmatrix_dingdi[top_flag] = -100;
                         old_top_flag = 0;
                         former_dingdi_pos = bottom_flag;
                         top_flag = 0;
                         old_bottom_flag = 0;                       
                }


                /*��ʼ�׶�ͬʱ���ֶ��͵�*/
                if(top_flag &&bottom_flag)
                {
                    /*�ȳ��׶�����ֶ�*/
                    if(top_flag >bottom_flag)
                    {
                        /*����ʼλ������Ϊ��*/    
                        gmatrix_dingdi[0] = 100;
                        gmatrix_dingdi[bottom_flag] = -100;
                        gmatrix_dingdi[top_flag] = 100;

                        old_bottom_flag = bottom_flag;
                        former_dingdi_pos = top_flag;
                        bottom_flag = 0;
                        old_top_flag = 0;
                        
                    }
                    /*�ȳ��ֶ�����ֵ�*/
                    if(top_flag < bottom_flag)
                    {

                       /*����ʼλ������Ϊ��*/    
                       gmatrix_dingdi[0] = -100;
                        gmatrix_dingdi[bottom_flag] = -100;
                        gmatrix_dingdi[top_flag] = 100;

                        old_top_flag = top_flag;
                        former_dingdi_pos = bottom_flag;
                        top_flag = 0;
                        old_bottom_flag = 0;
                        
                    }


                }

			}			
		}
		/*Ѱ�Һ�����ֵĶ��͵�*/
		else
		{
			if(i >= (fengxin_time + former_dingdi_pos))
			{
				/*ǰһ�λ�ȡ�����Ƕ�����ô����ҵ��ǵ�*/
				if(top_flag)
				{
					/*���ڿ��ܳ��ֱ�ǰһ�εĶ����õĶ��������Ҫ����һ��*/
					cur_top_flag = GetOneTopSharp(pCalcInfo,old_bottom_flag,i,fengxin_time);

                    if(cur_top_flag > top_flag)
                    {

                        gmatrix_dingdi[top_flag] = 0;
                        gmatrix_dingdi[cur_top_flag] = 100;

                        top_flag = cur_top_flag;
                        former_dingdi_pos = cur_top_flag;
                    }

					bottom_flag = GetOneBottomSharp(pCalcInfo,former_dingdi_pos,i,fengxin_time);
					if(bottom_flag)
					{
                        old_top_flag = top_flag;
						top_flag = 0;
						former_dingdi_pos = bottom_flag;
                        gmatrix_dingdi[former_dingdi_pos] = -100;
					}
					
				}
				else if(bottom_flag)
				{

					
					cur_bottom_flag = GetOneBottomSharp(pCalcInfo,old_top_flag,i,fengxin_time);

                    if(cur_bottom_flag > bottom_flag)
                    {
                        gmatrix_dingdi[bottom_flag] = 0;
                        gmatrix_dingdi[cur_bottom_flag] = -100;
                        bottom_flag = cur_bottom_flag;
                        former_dingdi_pos = cur_bottom_flag;
                    }

					top_flag = GetOneTopSharp(pCalcInfo,former_dingdi_pos,i,fengxin_time);
					if(top_flag)
					{
                        old_bottom_flag=bottom_flag;
						bottom_flag = 0;
						former_dingdi_pos = top_flag;
                        gmatrix_dingdi[former_dingdi_pos] = 100;

					}
				}
				else
				{
					/*wrong branch*/
					;
				}

			}			
		}
	}
	return 0;
}


/*�ú�������С���ڵ���ֵ(Original_PriData)���������������ֵ(HLevel_PriData)���������1���ӵĸߵ͵�ɽ������м����5
����(zqzhuanhuanlv)�ĸߵ͵�ɽ�������*/
 int  ConstrPrivData(OUTPUT STPrivateData* HLevel_PriData, INPUT STPrivateData* 
Original_PriData,INPUT unsigned int zqzhuanhuanlv)
{

    CALCINFO1* pHLevel_pCalcInfo;
    CALCINFO1* pOriginal_pCalcInfo;
    
    int i,j;
    float temp_high = 0;
    float temp_low = 0;
    float temp_vol = 0;
    float temp_close = 0;


    /*���������Ч���ж�*/
    if(NULL == HLevel_PriData || NULL == Original_PriData || 0 == zqzhuanhuanlv)
    {
        return -1;
    }
    pHLevel_pCalcInfo = HLevel_PriData->pCalcInfo_level;
    pOriginal_pCalcInfo = Original_PriData->pCalcInfo_level;

    /*���������Ч���ж�*/
    if(NULL == pHLevel_pCalcInfo || NULL == pOriginal_pCalcInfo)
    {
        return -1;
    }


    /*�ж�Ԥ�����m_pStkData�ռ��Ƿ��㹻����������Ļ������¶�̬����*/
    
    if(HLevel_PriData->m_pStkData_len < ((pOriginal_pCalcInfo->m_nNumData/zqzhuanhuanlv)+1))
    {
        HLevel_PriData->m_pStkData_len = ((pOriginal_pCalcInfo->m_nNumData/zqzhuanhuanlv)+1);
        if(NULL != pHLevel_pCalcInfo->m_pStkData)
        {
            free(pHLevel_pCalcInfo->m_pStkData);
        }
        pHLevel_pCalcInfo->m_pStkData = (STKDATA *)malloc((sizeof(STKDATA))*HLevel_PriData->m_pStkData_len);
        
    }

    /*ȷ�����������еĸ������Լ������ڵĲ���*/
    pHLevel_pCalcInfo->m_nNumData = pOriginal_pCalcInfo->m_nNumData/zqzhuanhuanlv;

    for(i = 0; i < pHLevel_pCalcInfo->m_nNumData ; i++)    
    {
        temp_high = 0;
        temp_low = 0;
        temp_vol = 0;
        temp_close = 0;

        for (j = i * (int)zqzhuanhuanlv; j < (i+1) * (int)zqzhuanhuanlv; j++ )
        {
            
            temp_high +=  pOriginal_pCalcInfo->m_pStkData[j].m_fHigh;
            temp_low +=  pOriginal_pCalcInfo->m_pStkData[j].m_fLow;
            temp_vol +=  pOriginal_pCalcInfo->m_pStkData[j].m_fVolume;
            temp_close +=  pOriginal_pCalcInfo->m_pStkData[j].m_fClose;
        }
        
        pHLevel_pCalcInfo->m_pStkData[i].m_fHigh = temp_high/zqzhuanhuanlv;
        pHLevel_pCalcInfo->m_pStkData[i].m_fLow = temp_low/zqzhuanhuanlv;
        pHLevel_pCalcInfo->m_pStkData[i].m_fVolume = temp_vol;
        pHLevel_pCalcInfo->m_pStkData[i].m_fClose = temp_close/zqzhuanhuanlv;

    }


   /*���һ������������*/
    if(pOriginal_pCalcInfo->m_nNumData%zqzhuanhuanlv)
    {
         temp_high = 0;
         temp_low = 0;
         temp_vol = 0;
         temp_close = 0;

        for(j = pHLevel_pCalcInfo->m_nNumData*zqzhuanhuanlv; j < pOriginal_pCalcInfo->m_nNumData ; j++)    
        {
            temp_high +=  pOriginal_pCalcInfo->m_pStkData[j].m_fHigh;
            temp_low +=  pOriginal_pCalcInfo->m_pStkData[j].m_fLow;
            temp_vol +=  pOriginal_pCalcInfo->m_pStkData[j].m_fVolume;
            temp_close +=  pOriginal_pCalcInfo->m_pStkData[j].m_fClose;        
        }
        pHLevel_pCalcInfo->m_pStkData[i].m_fHigh = temp_high/(pOriginal_pCalcInfo->m_nNumData%zqzhuanhuanlv);
        pHLevel_pCalcInfo->m_pStkData[i].m_fLow = temp_low/(pOriginal_pCalcInfo->m_nNumData%zqzhuanhuanlv);
        pHLevel_pCalcInfo->m_pStkData[i].m_fVolume = (temp_vol/(pOriginal_pCalcInfo->m_nNumData%zqzhuanhuanlv))*zqzhuanhuanlv;
        pHLevel_pCalcInfo->m_pStkData[i].m_fClose = temp_close/(pOriginal_pCalcInfo->m_nNumData%zqzhuanhuanlv);

        pHLevel_pCalcInfo->m_nNumData++;

    }   

    return 0; 

}





#if 0


typedef struct tagPrivateData
{
	CALCINFO1* pCalcInfo_level;/*����ĳ���ڵĽṹ*/
	unsigned int m_pStkData_len;/*����ĳ���ڵĽṹm_pStkData�ṹ������������ϸ��������о��󳤶�һ��*/
	float  *matrix_dingdi_xulie;/*����ĳ���ڵĶ������У�100Ϊ����-100Ϊ�ף�0Ϊ�Ƕ���*/
	unsigned int dingdi_dui_pos_matrix[MAXIMCAREBI][2];/*�����ע�Ľ��ڱʵ���ʼ\�����㣬������ʾ����������ʾ��,��󲻳���40����
	ԭ�����������ϸ������������ڵ����ݣ�ԭ���Ͻ��������һ��������Ϊ�ٶ��ײ��붯��ѧ����*/
	unsigned int dingdi_bi_num;/*����������бʵĸ���*/
	
	int matrix_dingdi_changdu[MAXIMCAREBI]; /*�������еĶ������ߵĳ���*/
	float matrix_dingdi_fudu[MAXIMCAREBI]; /*�������еĶ������ߵķ���*/
	float matrix_dingdi_xielv[MAXIMCAREBI]; /*�������еĶ������ߵ�б��*/
	float matrix_dingdi_mianji[MAXIMCAREBI]; /*�������еĶ������ߵ����*/



} STPrivateData;

#endif


float mysum(float *matrix,unsigned int start,unsigned int end)
{
    unsigned int i;
    float ret = 0;
    for (i = start ; i <= end ; i++)
    {
        ret += matrix[i];
    }
    return ret;
}




/*����ĳ���ڵİ��ʻ��ֵĶ���ѧ���Բ��������糤�ȡ���ֵ��б�ʡ�����ȵ�*/
/*����-1ʱ�����ڲ����������������ж�*/
OUTPUT int CalcuDongLiXueValue(INPUT STPrivateData* PriData)
{
    STPrivateData* pstTemPriData = PriData;
    CALCINFO1* pstCalcInfo;
   /* float *pdingdixulie;*/
    int i,j,count;
    /*������Ч���ж�*/
    if(NULL == PriData)
    {
        return -1;
    }
    pstCalcInfo = pstTemPriData->pCalcInfo_level;
    /*������Ч���ж�*/
    if(NULL == pstCalcInfo)
    {
        return -1;
    }
    /*ĳ���ڵĶ������У�100Ϊ����-100Ϊ�ף�0Ϊ�Ƕ���*/
    pstTemPriData->matrix_dingdi_xulie;

	#if 0
    /*������Ч���ж�*/
    if(NULL == pdingdixulie)
    {
        return -1;
    }
#endif
    if(2 >= pstCalcInfo->m_nNumData)
    {
        return -1;
    }

    
    /*����������������Ч�ıʣ����赱ǰλ��Ҳ�������һ�ʵ��յ�*/
    count = 0;
    pstTemPriData->dingdi_dui_pos_matrix[0][0]= 0;

    for (i = 0; i < pstCalcInfo->m_nNumData ; i ++)
    {
        if (pstTemPriData->matrix_dingdi_xulie[pstCalcInfo->m_nNumData-1-i])
        {
            pstTemPriData->dingdi_dui_pos_matrix[count][1] = (pstCalcInfo->m_nNumData-1-i);
            if(count)
            {
                pstTemPriData->dingdi_dui_pos_matrix[count][0] = pstTemPriData->dingdi_dui_pos_matrix[count-1][1];
            }
            count++;        
        }
        if(count >= MAXIMCAREBI)
        {
            break;
        }
    }
    if(count)
    {
        pstTemPriData->dingdi_dui_pos_matrix[0][0] = (pstCalcInfo->m_nNumData-1);
        pstTemPriData->dingdi_bi_num = count;

    }

    else 
    {
        /*û���γ���Ч��һ����˸����ڲ����������������ж�*/
        pstTemPriData->dingdi_bi_num = 0;
        return -1;
        /*do nothing*/
        ;
    }
    
    /*����ÿһ�ʵĳ��ȡ����ȡ����ȱ�ֵ�������б�ʵȵ�*/

    for(i = 0; i < (int)pstTemPriData->dingdi_bi_num;i ++)
    {
        float tem1,tem2;
        /*����ÿһ�ʵ�ʱ�䳤��*/
        pstTemPriData->matrix_dingdi_changdu[i] = 
                (pstTemPriData->dingdi_dui_pos_matrix[i][0]-pstTemPriData->dingdi_dui_pos_matrix[i][1] +1);

        /*���г�����Ч���ж�*/
        if(2 > pstTemPriData->matrix_dingdi_changdu[i])
        {
            return -1;
        }
        
        /*����ÿһ�ʵ��ǵ�����*/
        pstTemPriData->matrix_dingdi_fudu[i] = pstCalcInfo->m_pStkData[(pstTemPriData->dingdi_dui_pos_matrix[i][0])].m_fClose 
- pstCalcInfo->m_pStkData[(pstTemPriData->dingdi_dui_pos_matrix[i][1])].m_fClose ;
        /*����ÿһ�ʵ����*/
		/*�������ķ����ܶ࣬����������̼ۼ�ȥ�߶�������̼۸�ĺ��ٳ������ʱ�����̾��۵ķ�ʽ�����Ծ��������ξ��Լ۸�ĸߵͶ������Ӱ��*/
        for ( j = (int)pstTemPriData->dingdi_dui_pos_matrix[i][1] ;j <= (int)pstTemPriData->dingdi_dui_pos_matrix[i][0];j++)
        {
            tem1 += pstCalcInfo->m_pStkData[j].m_fClose 
                    - pstCalcInfo->m_pStkData[(pstTemPriData->dingdi_dui_pos_matrix[i][1])].m_fClose ;
            tem2 += pstCalcInfo->m_pStkData[j].m_fClose ;
        }
        pstTemPriData->matrix_dingdi_mianji[i] = pstTemPriData->matrix_dingdi_changdu[i]*tem1/tem2;

        /*����ÿһ�ʵ��ǵ����ȱ�ֵ*/
        pstTemPriData->matrix_dingdi_fudu[i] = pstTemPriData->matrix_dingdi_fudu[i*pstTemPriData->matrix_dingdi_changdu[i]]/tem2;


        /*����ÿһ�ʵ�б�ʣ�ͨ����С���˷�*/
        pstTemPriData->matrix_dingdi_xielv[i] = GetXieLv(pstCalcInfo,pstTemPriData->dingdi_dui_pos_matrix[i][1],pstTemPriData->dingdi_dui_pos_matrix[i][0]);


    }


	return 0;

    /*�����������Ľṹ��ѡ�������ԭ����ȷ����Щ����Ϊ�����Ƚϣ��Լ����������㣻������С����ѡ�������������*/



}




/*�ع鷽�̣�y=ax+b,����a����б�ʣ�b����ؾ࣬��������n1��n2
��Χ���ڵ����ݽ������Իع顣
a[0]:���б��a��a[1]��Žؾ�b��n1
:��Χ���ڵĵ�һ�����������е�λ�ã�Ԫ���±꣩��n2
:��Χ���ڵ����һ�����������е�λ�ã�Ԫ���±꣩��x[],y[]
�ֱ�����ź����Ա�����������������ݵ�����,RR[0]������ϵ��R��ƽ��*/

/*���ڱ�׼����Ȼ������y[i]=i����˺����п���������ͬʱб��ֱ���Է���ֵ����*/
//���Ǳ������Ա�д���㷨����excel�е��㷨��ͬ
#if 1
float GetXieLv(CALCINFO1* pstCalcInfo,int n1,int n2)
{
    int i;
    float ret=0.0;
    //�ֱ���� ��Xi, ��Yi, ��(XiYi)����(Xi*Xi), ��(Yi*Yi), ��Xi*��Yi, (��X)*(��X), (��Y)*(��Y)
    float SumX=0,SumY=0,SumXY=0,SumXX=0,SumYY=0,SumXAndSumY,SumXAndSumX,
    SumYAndSumY;
    
    /*������Ч����֤*/
    if(n1==n2)
    {
       return 0;
    }
    if(n1>n2) //���n1��n2С���򽻻�������ֵ,�Ա�֤n1<n2
    {
        int tmp;
        tmp=n1;
        n1=n2;
        n2=tmp;
    }

    for(i=n1;i<=n2;i++) //�˴�����ʵ��������޸�
    {
        SumX=SumX+pstCalcInfo->m_pStkData[i].m_fClose;         //SumX=��xi
        SumY=SumY+i;         //SumY=��yi
        SumXY=SumXY+pstCalcInfo->m_pStkData[i].m_fClose*i;  //SumXY=��(XiYi)
        SumXX=SumXX+pstCalcInfo->m_pStkData[i].m_fClose*pstCalcInfo->m_pStkData[i].m_fClose;  //SumXX=��(Xi*Xi)
        SumYY=SumYY+i*i;  //SumYY=��(Yi*Yi)
    }
    SumXAndSumY=SumX*SumY;    //SumXAndSumY= ��Xi * ��Yi
    SumXAndSumX=SumX*SumX;    //SumXAndSumX=(��X)*(��X)
    SumYAndSumY=SumY*SumY;    //SumYAndSumY=(��Y)*(��Y)

    if((n2-n1+1)*SumXX-SumXAndSumX)
    {
        ret=((n2-n1+1)*SumXY-SumXAndSumY)/((n2-n1+1)*SumXX-SumXAndSumX);  //б��a
    //  a[1]=(SumY-a[0]*SumX)/(n2-n1+1);  //�ؾ�b
    }
    return ret;
}


#else
void GetCoefficient(float x[],float y[],int n1,int n2,float a[2],float RR[1])
{
  //�ֱ���� ��Xi, ��Yi, ��(XiYi)����(Xi*Xi), ��(Yi*Yi), ��Xi*��Yi, (��X)*(��X), (��Y)*(��Y)
  float SumX=0,SumY=0,SumXY=0,SumXX=0,SumYY=0,SumXAndSumY,SumXAndSumX,
SumYAndSumY;

  if(n1>n2) //���n1��n2С���򽻻�������ֵ,�Ա�֤n1<n2
  {
    int tmp;
    tmp=n1;
    n1=n2;
    n2=tmp;
  }

  for(int i=n1;i<=n2;i++) //�˴�����ʵ��������޸�
  {
    SumX=SumX+x[i];         //SumX=��xi
    SumY=SumY+y[i];         //SumY=��yi
    SumXY=SumXY+x[i]*y[i];  //SumXY=��(XiYi)
    SumXX=SumXX+x[i]*x[i];  //SumXX=��(Xi*Xi)
    SumYY=SumYY+y[i]*y[i];  //SumYY=��(Yi*Yi)
  }
  SumXAndSumY=SumX*SumY;    //SumXAndSumY= ��Xi * ��Yi
  SumXAndSumX=SumX*SumX;    //SumXAndSumX=(��X)*(��X)
  SumYAndSumY=SumY*SumY;    //SumYAndSumY=(��Y)*(��Y)

  if((n2-n1+1)*SumXX-SumXAndSumX)
  {
    a[0]=((n2-n1+1)*SumXY-SumXAndSumY)/((n2-n1+1)*SumXX-SumXAndSumX);  //б��a
    a[1]=(SumY-a[0]*SumX)/(n2-n1+1);  //�ؾ�b
  }
  if(((n2-n1+1)*SumXX-SumXAndSumX)*((n2-n1+1)*SumYY-SumYAndSumY))
//�������ϵ��R��ƽ��
    RR[0]=((n2-n1+1)*SumXY-SumXAndSumY)*((n2-n1+1)*SumXY-SumXAndSumY)/(((n2-n1
+1)*SumXX-SumXAndSumX)*((n2-n1+1)*SumYY-SumYAndSumY));//Rƽ��
}

#endif



