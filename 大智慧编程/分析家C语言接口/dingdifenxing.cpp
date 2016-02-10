
#include "stdafx.h"
#include "FxjFunc.h"
#include <stdlib.h>         
#include <malloc.h>

float GetXieLv(CALCINFO1* pstCalcInfo,int n1,int n2);

/* ============================BeginningOfHeader===============================
/ 名称: GetOneTopSharp
/ 功能:
	寻找序列对MatrixMax MatrixMin距今cur_pos N1_pos和N2_pos（N1_pos<N2_pos<cur_pos）周期之间是否存在确定的顶，以M_sharp个标准序列为准
	返回值为0时表示N1_pos到N2_pos周期间不存在顶分型，否则返回顶分型的位置；
	按照禅式理论比较最高点的顶分型，且考虑到底点K线合并的情况
/ 版本: 1.0
/ 作者: xuejiayong
/ 电子邮件：fraskey@126.com
/ 日期: 2011-5-01
/===============================EndingOFHeader=================================*/

OUTPUT int GetOneTopSharp(INPUT const CALCINFO* pCalcInfo,
INPUT unsigned int N1_pos,INPUT unsigned int N2_pos,INPUT unsigned int M_sharp)
{
	int ret_flag = 0;
	unsigned int i;
	unsigned int max_pos;/*顶分型的位置*/
	float max_count = 0;/*由于序列为正数，因此初始化的最大值为0是合理的*/

	float	tem_high ;
	float	tem_low;
	unsigned int fengxing_cnt;

	/*输入参数有效性验证*/
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

	/*第一步：寻找N1_pos到N2_pos期间的最大值，顶分型必以最大值为顶点*/
	for(i = N1_pos;i <= N2_pos ;i++)
	{
		if(pCalcInfo->m_pStkData[i].m_fHigh > max_count)
		{
			max_count = pCalcInfo->m_pStkData[i].m_fHigh;
			max_pos = i;
		}
				
	}


	/*第二步：对于存在最大值点时，确认是否为顶分型的最高点，判断方法是看其左右两边是否分别形成了M_sharp长度的标准下降序列*/

	/*首先简要判断是否满足顶分型的初步特征，以减少无谓的计算量*/
	if( !((max_pos >= (M_sharp-1+N1_pos)) && (N2_pos>=(M_sharp-1+max_pos))))
	{
		ret_flag = 0;
		return ret_flag;
	}
	
	tem_high = pCalcInfo->m_pStkData[max_pos].m_fHigh;
	tem_low = pCalcInfo->m_pStkData[max_pos].m_fLow;
	fengxing_cnt = 0;
	/*对右侧运算*/
	for(i=max_pos ; i<=N2_pos ;i++)
	{

        /*形成一个标准下降序列*/
        if( (pCalcInfo->m_pStkData[i].m_fHigh < tem_high) && (pCalcInfo->m_pStkData[i].m_fLow < tem_low))
        {
            fengxing_cnt ++;
            tem_high = pCalcInfo->m_pStkData[i].m_fHigh;
            tem_low = pCalcInfo->m_pStkData[i].m_fLow;
        
        }
#if 0
        else if((pCalcInfo->m_pStkData[i].m_fHigh >= tem_high) && (pCalcInfo->m_pStkData[i].m_fLow <= tem_low))
        {
            /*包含了前一跟K线，有可能会破坏前期形成的序列，为了保持一致性，简单处理，不做修正*/
            tem_high = pCalcInfo->m_pStkData[i].m_fHigh;
            tem_low = pCalcInfo->m_pStkData[i].m_fLow;
        }
        else if((pCalcInfo->m_pStkData[i].m_fHigh >=tem_high) && (pCalcInfo->m_pStkData[i].m_fLow >= tem_low))
        {
            /*形成了一个标准的上升序列了，不严厉的情况下可以走这个分支*/
            tem_high = pCalcInfo->m_pStkData[i].m_fHigh;
            tem_low = pCalcInfo->m_pStkData[i].m_fLow;
            
        }

        else
        {
            tem_high = pCalcInfo->m_pStkData[i].m_fHigh;
            tem_low = pCalcInfo->m_pStkData[i].m_fLow;

            /*被前一跟k线所包含，do nothing*/
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
		/*右边的分型序列已经足够多了，不用再继续进行下去了，节省运算量*/
		if(fengxing_cnt > M_sharp-1)
		{
			break;
		}		
	}
	
	/*右侧序列形成的情况下，可对左侧进行计算*/		
	if(fengxing_cnt > M_sharp-1)
	{
		tem_high = pCalcInfo->m_pStkData[N1_pos].m_fHigh;
		tem_low = pCalcInfo->m_pStkData[N1_pos].m_fLow;
		fengxing_cnt = 0;
		/*对左侧运算*/
		for(i =N1_pos  ; i <= max_pos ; i++)
		{


            /*形成一个标准上升序列*/
            if( (pCalcInfo->m_pStkData[i].m_fHigh > tem_high) && (pCalcInfo->m_pStkData[i].m_fLow > tem_low) )
            {
                fengxing_cnt ++;
                tem_high = pCalcInfo->m_pStkData[i].m_fHigh;
                tem_low = pCalcInfo->m_pStkData[i].m_fLow;
            }

#if 0
            else if((pCalcInfo->m_pStkData[i].m_fHigh >=tem_high) && (pCalcInfo->m_pStkData[i].m_fLow <=tem_low))
            {
                /*包含了前一跟K线，有可能会破坏前期形成的序列，因此简单处理，不做修正*/
                tem_high = pCalcInfo->m_pStkData[i].m_fHigh;
                tem_low = pCalcInfo->m_pStkData[i].m_fLow;
                
            }
            else if((pCalcInfo->m_pStkData[i].m_fHigh <=tem_high) && (pCalcInfo->m_pStkData[i].m_fLow <=tem_low))
            {
                /*形成了一个标准的下降序列了，不严厉的情况下可以走这个分支*/
                tem_high = pCalcInfo->m_pStkData[i].m_fHigh;
                tem_low = pCalcInfo->m_pStkData[i].m_fLow;
                
            }
            else
            {
                tem_high = pCalcInfo->m_pStkData[i].m_fHigh;
                tem_low = pCalcInfo->m_pStkData[i].m_fLow;

                /*被前一跟k线所包含，do nothing*/
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

			/*左边的分型序列已经足够多了，不用再继续进行下去了，节省运算量；同时可确认两边都形成了下降序列，即顶分型成立*/
			if(fengxing_cnt > M_sharp-1)
			{
				ret_flag = max_pos;
			}	

		
		}
	
	}			
		
	return ret_flag;
}




/* ============================BeginningOfHeader===============================
/ 名称: GetOneBottomSharp
/ 功能:
	寻找序列对MatrixMax MatrixMin距今cur_pos N1_pos和N2_pos（N1_pos<N2_pos<cur_pos）周期之间是否存在确定的底，以M_sharp个标准序列为准
	返回值为0时表示N1_pos到N2_pos周期间不存在底分型，否则返回底分型的位置；
	按照禅式理论比较最高点的顶分型，且考虑到底点K线合并的情况
/ 版本: 1.0
/ 作者: xuejiayong
/ 电子邮件：fraskey@126.com
/ 日期: 2011-5-01
/===============================EndingOFHeader=================================*/
 int GetOneBottomSharp( const CALCINFO* pCalcInfo,
 unsigned int N1_pos, unsigned int N2_pos, unsigned int M_sharp)
{
	 int ret_flag = 0;
	 unsigned int i;
	 unsigned int min_pos;/*底分型的位置*/
	 float min_count = 100.0;/*由于序列为股价，因此初始化的最大值为10000是合理的*/

	 float	tem_high ;
	 float	tem_low;
	 unsigned int fengxing_cnt;

	
	/*输入参数有效性验证*/
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
	/*第一步：寻找N1_pos到N2_pos期间的最小值，底分型必以最小值为底点*/
    //min_count = pCalcInfo->m_pStkData[N1_pos].m_fLow;
	for(i = N1_pos;i <= N2_pos ;i++)
	{
		if(pCalcInfo->m_pStkData[i].m_fLow < min_count)
		{
			min_count = pCalcInfo->m_pStkData[i].m_fLow;
			min_pos = i;
		}
	}

	/*第二步：对于存在最值点时，确认是否为底分型的最低点，判断方法是看其左右两边是否分别形成了M_sharp长度的标准上升序列*/

	/*首先简要判断是否满足底分型的初步特征，以减少无谓的计算量*/
	if( !((min_pos >= (M_sharp-1+N1_pos)) && (N2_pos>=(M_sharp-1+min_pos))))
	{
		ret_flag = 0;
		return ret_flag;
	}

    tem_low = pCalcInfo->m_pStkData[min_pos].m_fLow;
	tem_high = pCalcInfo->m_pStkData[min_pos].m_fHigh;
	fengxing_cnt = 0;

	/*对右侧运算*/
	for(i = min_pos ; i <= N2_pos ;i++)
	{
		/*形成一个标准上升序列*/
		if( (pCalcInfo->m_pStkData[i].m_fHigh > tem_high) && (pCalcInfo->m_pStkData[i].m_fLow > tem_low) )
		{
			fengxing_cnt ++;
			tem_high = pCalcInfo->m_pStkData[i].m_fHigh;
			tem_low = pCalcInfo->m_pStkData[i].m_fLow;
		}
#if 0
        else if((pCalcInfo->m_pStkData[i].m_fHigh >=tem_high) && (pCalcInfo->m_pStkData[i].m_fLow <=tem_low))
        {
            /*包含了前一跟K线，有可能会破坏前期形成的序列，因此简单处理，不做修正*/
            tem_high = pCalcInfo->m_pStkData[i].m_fHigh;
            tem_low = pCalcInfo->m_pStkData[i].m_fLow;
            
        }
        else if((pCalcInfo->m_pStkData[i].m_fHigh <=tem_high) && (pCalcInfo->m_pStkData[i].m_fLow <=tem_low))
        {
            /*形成了一个标准的下降序列了，不严厉的情况下可以走这个分支*/
            tem_high = pCalcInfo->m_pStkData[i].m_fHigh;
            tem_low = pCalcInfo->m_pStkData[i].m_fLow;
            
        }
        else
        {
            tem_high = pCalcInfo->m_pStkData[i].m_fHigh;
            tem_low = pCalcInfo->m_pStkData[i].m_fLow;

            /*被前一跟k线所包含，do nothing*/
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
		/*右边的分型序列已经足够多了，不用再继续进行下去了，节省运算量*/
		if(fengxing_cnt > M_sharp-1)
		{
			break;
		}	
	}
	
 	/*右侧序列形成的情况下，可对左侧进行计算，应该是一个下降序列*/		
	if(fengxing_cnt > M_sharp-1)
	{
		tem_high = pCalcInfo->m_pStkData[N1_pos].m_fHigh;
		tem_low = pCalcInfo->m_pStkData[N1_pos].m_fLow;
		fengxing_cnt = 0;
		/*对左侧运算*/
		for(i =N1_pos  ; i <= min_pos ;i++)
		{
			/*形成一个标准下降序列*/
			if( (pCalcInfo->m_pStkData[i].m_fHigh < tem_high) && (pCalcInfo->m_pStkData[i].m_fLow < tem_low))
			{
				fengxing_cnt ++;
                tem_high = pCalcInfo->m_pStkData[i].m_fHigh;
                tem_low = pCalcInfo->m_pStkData[i].m_fLow;

			}
#if 0
            else if((pCalcInfo->m_pStkData[i].m_fHigh >= tem_high) && (pCalcInfo->m_pStkData[i].m_fLow <= tem_low))
            {
                /*包含了前一跟K线，有可能会破坏前期形成的序列，为了保持一致性，简单处理，不做修正*/
                tem_high = pCalcInfo->m_pStkData[i].m_fHigh;
                tem_low = pCalcInfo->m_pStkData[i].m_fLow;
            }
            else if((pCalcInfo->m_pStkData[i].m_fHigh >=tem_high) && (pCalcInfo->m_pStkData[i].m_fLow >= tem_low))
            {
                /*形成了一个标准的上升序列了，不严厉的情况下可以走这个分支*/
                tem_high = pCalcInfo->m_pStkData[i].m_fHigh;
                tem_low = pCalcInfo->m_pStkData[i].m_fLow;
                
            }

            else
            {
                tem_high = pCalcInfo->m_pStkData[i].m_fHigh;
                tem_low = pCalcInfo->m_pStkData[i].m_fLow;

                /*被前一跟k线所包含，do nothing*/
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

			/*左边的分型序列已经足够多了，不用再继续进行下去了，节省运算量；同时可确认两边都形成了下降序列，即顶分型成立*/
			if(fengxing_cnt > M_sharp-1)
			{
				ret_flag = min_pos;
			}

		}
	
	}			

	return ret_flag;
	
}

/* ============================BeginningOfHeader===============================
/ 名称:CalcuDingDiFenXing
/ 
功能:计算顶分型的顶和底分型的底，计算结果保存在全局变量数组gmatrix_dingdi[]中，数值100表示顶，-100表示底，0表示非顶非底
/ 版本: 1.0
/ 作者: xuejiayong
/ 电子邮件：fraskey@126.com
/ 日期: 2011-5-01
/===============================EndingOFHeader=================================*/

OUTPUT CalcuDingDiFenXing(INPUT const CALCINFO* pCalcInfo,OUTPUT float* 
gmatrix_dingdi,INPUT unsigned int fengxin_time)
{
    unsigned int i;
    unsigned int former_dingdi_pos = 0;/*前一个顶底所在的位置*/
	unsigned int top_flag=0;
	unsigned int bottom_flag=0;
	unsigned int old_top_flag=0;
	unsigned int old_bottom_flag=0;

	unsigned int cur_top_flag=0;
	unsigned int cur_bottom_flag=0;

	/*轮训所有序列参数*/
	for(i=0;i<(unsigned int)pCalcInfo->m_nNumData;i++)
	{
		/*初始化当前点位为非顶非底*/
        gmatrix_dingdi[i] = 0;

		/*寻找第一个顶或者底，同时出现顶和底的时候，最先出现顶或者底是确定的顶或者底,假设位置0为默认顶底*/
		if(!former_dingdi_pos)
		{
			/*当前位置只有在大于分型序列个数时才可能有第一个顶底出现*/
			if(i  >= (fengxin_time + former_dingdi_pos))
			{
				top_flag = GetOneTopSharp(pCalcInfo,former_dingdi_pos,i,fengxin_time);
				bottom_flag = GetOneBottomSharp(pCalcInfo,former_dingdi_pos,i,fengxin_time);

                /*出现顶，将起始点设置为确认的底*/
                if(top_flag &&!bottom_flag)
                {
                        gmatrix_dingdi[0] = -100;
                        gmatrix_dingdi[top_flag] = 100;
                        old_bottom_flag = 0;
                        former_dingdi_pos = top_flag;
                        bottom_flag = 0;
                        old_top_flag = 0;                       
               }


                 /*出现底，将起始点设置为确认的顶*/
                 if(!top_flag &&bottom_flag)
                 {
                         gmatrix_dingdi[0] = 100;
                         gmatrix_dingdi[top_flag] = -100;
                         old_top_flag = 0;
                         former_dingdi_pos = bottom_flag;
                         top_flag = 0;
                         old_bottom_flag = 0;                       
                }


                /*开始阶段同时出现顶和底*/
                if(top_flag &&bottom_flag)
                {
                    /*先出底顶后出现顶*/
                    if(top_flag >bottom_flag)
                    {
                        /*将起始位置设置为顶*/    
                        gmatrix_dingdi[0] = 100;
                        gmatrix_dingdi[bottom_flag] = -100;
                        gmatrix_dingdi[top_flag] = 100;

                        old_bottom_flag = bottom_flag;
                        former_dingdi_pos = top_flag;
                        bottom_flag = 0;
                        old_top_flag = 0;
                        
                    }
                    /*先出现顶后出现底*/
                    if(top_flag < bottom_flag)
                    {

                       /*将起始位置设置为底*/    
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
		/*寻找后面出现的顶和底*/
		else
		{
			if(i >= (fengxin_time + former_dingdi_pos))
			{
				/*前一次获取到的是顶，那么这次找的是底*/
				if(top_flag)
				{
					/*由于可能出现比前一次的顶更好的顶，因此需要修正一下*/
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


/*该函数根据小周期的数值(Original_PriData)计算出来大周期数值(HLevel_PriData)，比如根据1分钟的高低点成交量序列计算出5
分钟(zqzhuanhuanlv)的高低点成交量序列*/
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


    /*输入参数有效性判断*/
    if(NULL == HLevel_PriData || NULL == Original_PriData || 0 == zqzhuanhuanlv)
    {
        return -1;
    }
    pHLevel_pCalcInfo = HLevel_PriData->pCalcInfo_level;
    pOriginal_pCalcInfo = Original_PriData->pCalcInfo_level;

    /*输入参数有效性判断*/
    if(NULL == pHLevel_pCalcInfo || NULL == pOriginal_pCalcInfo)
    {
        return -1;
    }


    /*判断预分配的m_pStkData空间是否足够大，如果不够的话再重新动态调整*/
    
    if(HLevel_PriData->m_pStkData_len < ((pOriginal_pCalcInfo->m_nNumData/zqzhuanhuanlv)+1))
    {
        HLevel_PriData->m_pStkData_len = ((pOriginal_pCalcInfo->m_nNumData/zqzhuanhuanlv)+1);
        if(NULL != pHLevel_pCalcInfo->m_pStkData)
        {
            free(pHLevel_pCalcInfo->m_pStkData);
        }
        pHLevel_pCalcInfo->m_pStkData = (STKDATA *)malloc((sizeof(STKDATA))*HLevel_PriData->m_pStkData_len);
        
    }

    /*确定大周期序列的个数，以及大周期的参数*/
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


   /*最后一个参数做修正*/
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
	CALCINFO1* pCalcInfo_level;/*定义某周期的结构*/
	unsigned int m_pStkData_len;/*定义某周期的结构m_pStkData结构体个数，理论上跟顶底序列矩阵长度一致*/
	float  *matrix_dingdi_xulie;/*定义某周期的顶底序列，100为顶；-100为底；0为非顶底*/
	unsigned int dingdi_dui_pos_matrix[MAXIMCAREBI][2];/*定义关注的近期笔的起始\结束点，正数表示顶，负数表示底,最大不超过40个，
	原则上最多分析上个周期两笔以内的数据，原则上将序列最后一个数据作为假顶底参与动力学计算*/
	unsigned int dingdi_bi_num;/*定义近期所有笔的个数*/
	
	int matrix_dingdi_changdu[MAXIMCAREBI]; /*计算所有的顶底连线的长度*/
	float matrix_dingdi_fudu[MAXIMCAREBI]; /*计算所有的顶底连线的幅度*/
	float matrix_dingdi_xielv[MAXIMCAREBI]; /*计算所有的顶底连线的斜率*/
	float matrix_dingdi_mianji[MAXIMCAREBI]; /*计算所有的顶底连线的面积*/



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




/*计算某周期的按笔划分的动力学属性参数，比如长度、幅值、斜率、面积等等*/
/*返回-1时该周期不参与后续买卖点的判断*/
OUTPUT int CalcuDongLiXueValue(INPUT STPrivateData* PriData)
{
    STPrivateData* pstTemPriData = PriData;
    CALCINFO1* pstCalcInfo;
   /* float *pdingdixulie;*/
    int i,j,count;
    /*参数有效性判断*/
    if(NULL == PriData)
    {
        return -1;
    }
    pstCalcInfo = pstTemPriData->pCalcInfo_level;
    /*参数有效性判断*/
    if(NULL == pstCalcInfo)
    {
        return -1;
    }
    /*某周期的顶底序列，100为顶；-100为底；0为非顶底*/
    pstTemPriData->matrix_dingdi_xulie;

	#if 0
    /*参数有效性判断*/
    if(NULL == pdingdixulie)
    {
        return -1;
    }
#endif
    if(2 >= pstCalcInfo->m_nNumData)
    {
        return -1;
    }

    
    /*计算出来最近几个有效的笔，假设当前位置也属于最后一笔的终点*/
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
        /*没有形成有效的一笔因此该周期不参与后期买卖点的判断*/
        pstTemPriData->dingdi_bi_num = 0;
        return -1;
        /*do nothing*/
        ;
    }
    
    /*计算每一笔的长度、幅度、幅度比值、面积、斜率等等*/

    for(i = 0; i < (int)pstTemPriData->dingdi_bi_num;i ++)
    {
        float tem1,tem2;
        /*计算每一笔的时间长度*/
        pstTemPriData->matrix_dingdi_changdu[i] = 
                (pstTemPriData->dingdi_dui_pos_matrix[i][0]-pstTemPriData->dingdi_dui_pos_matrix[i][1] +1);

        /*队列长度有效性判断*/
        if(2 > pstTemPriData->matrix_dingdi_changdu[i])
        {
            return -1;
        }
        
        /*计算每一笔的涨跌幅度*/
        pstTemPriData->matrix_dingdi_fudu[i] = pstCalcInfo->m_pStkData[(pstTemPriData->dingdi_dui_pos_matrix[i][0])].m_fClose 
- pstCalcInfo->m_pStkData[(pstTemPriData->dingdi_dui_pos_matrix[i][1])].m_fClose ;
        /*计算每一笔的面积*/
		/*面积计算的方法很多，这里采用收盘价减去线段起点收盘价格的和再除以这段时间收盘均价的方式，除以均价是屏蔽绝对价格的高低对面积的影响*/
        for ( j = (int)pstTemPriData->dingdi_dui_pos_matrix[i][1] ;j <= (int)pstTemPriData->dingdi_dui_pos_matrix[i][0];j++)
        {
            tem1 += pstCalcInfo->m_pStkData[j].m_fClose 
                    - pstCalcInfo->m_pStkData[(pstTemPriData->dingdi_dui_pos_matrix[i][1])].m_fClose ;
            tem2 += pstCalcInfo->m_pStkData[j].m_fClose ;
        }
        pstTemPriData->matrix_dingdi_mianji[i] = pstTemPriData->matrix_dingdi_changdu[i]*tem1/tem2;

        /*计算每一笔的涨跌幅度比值*/
        pstTemPriData->matrix_dingdi_fudu[i] = pstTemPriData->matrix_dingdi_fudu[i*pstTemPriData->matrix_dingdi_changdu[i]]/tem2;


        /*计算每一笔的斜率，通过最小二乘法*/
        pstTemPriData->matrix_dingdi_xielv[i] = GetXieLv(pstCalcInfo,pstTemPriData->dingdi_dui_pos_matrix[i][1],pstTemPriData->dingdi_dui_pos_matrix[i][0]);


    }


	return 0;

    /*计算基于中枢的结构，选择中枢的原因是确定哪些笔作为后续比较，以及几类买卖点；对于中小周期选择二三类买卖点*/



}




/*回归方程：y=ax+b,其中a代表斜率，b代表截距，对数组中n1到n2
范围段内的数据进行线性回归。
a[0]:存放斜率a，a[1]存放截距b。n1
:范围段内的第一个点在数组中的位置（元素下标），n2
:范围段内的最后一个点在数组中的位置（元素下标），x[],y[]
分别代表存放横轴自变量和纵轴因变量数据的数组,RR[0]存放相关系数R的平方*/

/*对于标准的自然数序列y[i]=i，因此函数中可以做精简，同时斜率直接以返回值体现*/
//这是本人亲自编写的算法，与excel中的算法相同
#if 1
float GetXieLv(CALCINFO1* pstCalcInfo,int n1,int n2)
{
    int i;
    float ret=0.0;
    //分别代表 ∑Xi, ∑Yi, ∑(XiYi)、∑(Xi*Xi), ∑(Yi*Yi), ∑Xi*∑Yi, (∑X)*(∑X), (∑Y)*(∑Y)
    float SumX=0,SumY=0,SumXY=0,SumXX=0,SumYY=0,SumXAndSumY,SumXAndSumX,
    SumYAndSumY;
    
    /*参数有效性验证*/
    if(n1==n2)
    {
       return 0;
    }
    if(n1>n2) //如果n1比n2小，则交换二者数值,以保证n1<n2
    {
        int tmp;
        tmp=n1;
        n1=n2;
        n2=tmp;
    }

    for(i=n1;i<=n2;i++) //此处根据实际情况可修改
    {
        SumX=SumX+pstCalcInfo->m_pStkData[i].m_fClose;         //SumX=∑xi
        SumY=SumY+i;         //SumY=∑yi
        SumXY=SumXY+pstCalcInfo->m_pStkData[i].m_fClose*i;  //SumXY=∑(XiYi)
        SumXX=SumXX+pstCalcInfo->m_pStkData[i].m_fClose*pstCalcInfo->m_pStkData[i].m_fClose;  //SumXX=∑(Xi*Xi)
        SumYY=SumYY+i*i;  //SumYY=∑(Yi*Yi)
    }
    SumXAndSumY=SumX*SumY;    //SumXAndSumY= ∑Xi * ∑Yi
    SumXAndSumX=SumX*SumX;    //SumXAndSumX=(∑X)*(∑X)
    SumYAndSumY=SumY*SumY;    //SumYAndSumY=(∑Y)*(∑Y)

    if((n2-n1+1)*SumXX-SumXAndSumX)
    {
        ret=((n2-n1+1)*SumXY-SumXAndSumY)/((n2-n1+1)*SumXX-SumXAndSumX);  //斜率a
    //  a[1]=(SumY-a[0]*SumX)/(n2-n1+1);  //截距b
    }
    return ret;
}


#else
void GetCoefficient(float x[],float y[],int n1,int n2,float a[2],float RR[1])
{
  //分别代表 ∑Xi, ∑Yi, ∑(XiYi)、∑(Xi*Xi), ∑(Yi*Yi), ∑Xi*∑Yi, (∑X)*(∑X), (∑Y)*(∑Y)
  float SumX=0,SumY=0,SumXY=0,SumXX=0,SumYY=0,SumXAndSumY,SumXAndSumX,
SumYAndSumY;

  if(n1>n2) //如果n1比n2小，则交换二者数值,以保证n1<n2
  {
    int tmp;
    tmp=n1;
    n1=n2;
    n2=tmp;
  }

  for(int i=n1;i<=n2;i++) //此处根据实际情况可修改
  {
    SumX=SumX+x[i];         //SumX=∑xi
    SumY=SumY+y[i];         //SumY=∑yi
    SumXY=SumXY+x[i]*y[i];  //SumXY=∑(XiYi)
    SumXX=SumXX+x[i]*x[i];  //SumXX=∑(Xi*Xi)
    SumYY=SumYY+y[i]*y[i];  //SumYY=∑(Yi*Yi)
  }
  SumXAndSumY=SumX*SumY;    //SumXAndSumY= ∑Xi * ∑Yi
  SumXAndSumX=SumX*SumX;    //SumXAndSumX=(∑X)*(∑X)
  SumYAndSumY=SumY*SumY;    //SumYAndSumY=(∑Y)*(∑Y)

  if((n2-n1+1)*SumXX-SumXAndSumX)
  {
    a[0]=((n2-n1+1)*SumXY-SumXAndSumY)/((n2-n1+1)*SumXX-SumXAndSumX);  //斜率a
    a[1]=(SumY-a[0]*SumX)/(n2-n1+1);  //截距b
  }
  if(((n2-n1+1)*SumXX-SumXAndSumX)*((n2-n1+1)*SumYY-SumYAndSumY))
//计算相关系数R的平方
    RR[0]=((n2-n1+1)*SumXY-SumXAndSumY)*((n2-n1+1)*SumXY-SumXAndSumY)/(((n2-n1
+1)*SumXX-SumXAndSumX)*((n2-n1+1)*SumYY-SumYAndSumY));//R平方
}

#endif



