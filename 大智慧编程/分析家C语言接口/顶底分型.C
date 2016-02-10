










#define MAXMATRIX      20000 /*定义搜索的数组的最大值*/

float  matrix_dingdi[MAXMATRIX];/*定义顶底*/


float  matrix_high[MAXMATRIX];
float  matrix_low[MAXMATRIX];

#define NULL 0

#define INPUT 
#define OUTPUT

OUTPUT int GetOneTopSharp(INPUT const CALCINFO* pCalcInfo,
INPUT unsigned int N1_pos,INPUT unsigned N2_pos,INPUT unsigned int M_sharp)
{
	int ret_flag = 0;
	unsigned int i;
	unsigned int max_pos;/*顶分型的位置*/
	float max_count = 0;/*由于序列为正数，因此初始化的最大值为0是合理的*/

	float	tem_high ;
	float	tem_low;
	unsigned int fengxing_cnt;

	/*输入参数有效性验证*/
	if(NULL == pCalcInfo )
	{
		ret_flag = 0;
		return ret_flag;
	}
	if(!((0<=N1_pos) && (N1_pos<N2_pos) 
		&& (0<=M_sharp)  &&  (M_sharp <=(N2_pos-N1_pos))))
	{
		ret_flag = 0;
		return ret_flag;
	}

	/*考虑到有些k线数据不全导致找不到分型点，过滤不正常的k线*/
    if(1000<N2_pos-N1_pos)
        N1_pos=N2_pos-1000;


	/*第一步：寻找N1_pos到N2_pos期间的最大值，顶分型必以最大值为顶点*/
	for(i = N1_pos;i < N2_pos ; i++)
	{
		if(pCalcInfo->m_pStkData[i].m_fHigh > max_count)
		{
			max_count = pCalcInfo->m_pStkData[i].m_fHigh;
			max_pos = i;
		}
				
	}


	/*第二步：对于存在最大值点时，确认是否为顶分型的最高点，判断方法是看其左右两边是否分别形成了M_sharp长度的标准下降序列*/

	/*首先简要判断是否满足顶分型的初步特征，以减少无谓的计算量*/
	if( !((max_pos - N1_pos)>=M_sharp && ((N2_pos -max_pos)>=M_sharp))) 
	{
		ret_flag = 0;
		return ret_flag;
	}
	
	tem_high = pCalcInfo->m_pStkData[max_pos].m_fHigh;
	tem_low = pCalcInfo->m_pStkData[max_pos].m_fLow;
	fengxing_cnt = 0;
	/*对右侧运算*/
	for(i = max_pos ; i < N2_pos ; i ++)
	{

        /*形成一个标准下降序列*/
        if( (pCalcInfo->m_pStkData[i].m_fHigh < tem_high) && (pCalcInfo->m_pStkData[i].m_fLow < tem_low))
        {
            fengxing_cnt ++;
            tem_high = pCalcInfo->m_pStkData[i].m_fHigh;
            tem_low = pCalcInfo->m_pStkData[i].m_fLow;
        
        }
        else if((pCalcInfo->m_pStkData[i].m_fHigh >= tem_high) && (pCalcInfo->m_pStkData[i].m_fLow <= tem_low))
        {
            /*包含了前一跟K线，有可能会破坏前期形成的序列，为了保持一致性，简单处理，不做修正*/
            tem_high = pCalcInfo->m_pStkData[i].m_fHigh;
            tem_low = pCalcInfo->m_pStkData[i].m_fLow;
        }
#if 1
        else if((pCalcInfo->m_pStkData[i].m_fHigh >tem_high) && (pCalcInfo->m_pStkData[i].m_fLow > tem_low))
        {
            /*形成了一个标准的上升序列了，不严厉的情况下可以走这个分支*/
            tem_high = pCalcInfo->m_pStkData[i].m_fHigh;
            tem_low = pCalcInfo->m_pStkData[i].m_fLow;
            
        }
#endif

        else
        {
            /*被前一跟k线所包含，do nothing*/
            ;
        }

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
		for(i =N1_pos  ; i < max_pos ; i ++)
		{


            /*形成一个标准上升序列*/
            if( (pCalcInfo->m_pStkData[i].m_fHigh > tem_high) && (pCalcInfo->m_pStkData[i].m_fLow > tem_low) )
            {
                fengxing_cnt ++;
                tem_high = pCalcInfo->m_pStkData[i].m_fHigh;
                tem_low = pCalcInfo->m_pStkData[i].m_fLow;
            }
            else if((pCalcInfo->m_pStkData[i].m_fHigh >=tem_high) && (pCalcInfo->m_pStkData[i].m_fLow <=tem_low))
            {
                /*包含了前一跟K线，有可能会破坏前期形成的序列，因此简单处理，不做修正*/
                tem_high = pCalcInfo->m_pStkData[i].m_fHigh;
                tem_low = pCalcInfo->m_pStkData[i].m_fLow;
                
            }
#if 1
            else if((pCalcInfo->m_pStkData[i].m_fHigh <tem_high) && (pCalcInfo->m_pStkData[i].m_fLow <tem_low))
            {
                /*形成了一个标准的下降序列了，不严厉的情况下可以走这个分支*/
                tem_high = pCalcInfo->m_pStkData[i].m_fHigh;
                tem_low = pCalcInfo->m_pStkData[i].m_fLow;
                
            }
#endif
            else
            {
                /*被前一跟k线所包含，do nothing*/
                ;
            }

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
OUTPUT int GetOneBottomSharp(INPUT const CALCINFO* pCalcInfo,
INPUT unsigned int N1_pos,INPUT unsigned N2_pos,INPUT unsigned int M_sharp)
{
	int ret_flag = 0;
	unsigned int i;
	unsigned int min_pos;/*底分型的位置*/
	float min_count = 10000;/*由于序列为股价，因此初始化的最大值为10000是合理的*/

	float	tem_high ;
	float	tem_low;
	unsigned int fengxing_cnt;

	
	/*输入参数有效性验证*/
	if(NULL == pCalcInfo )
	{
		ret_flag = 0;
		return ret_flag;
	}
	if(!((0<=N1_pos) && (N1_pos<N2_pos)
		&& (0<M_sharp)  &&  (M_sharp <=(N2_pos-N1_pos))))
	{
		ret_flag = 0;
		return ret_flag;
	}

	/*考虑到有些k线数据不全导致找不到分型点，过滤不正常的k线*/
    if(1000<N2_pos-N1_pos)
        N1_pos=N2_pos-1000;

	/*第一步：寻找N1_pos到N2_pos期间的最小值，底分型必以最小值为底点*/
	for(i = N1_pos;i < N2_pos ; i++)
	{
		if(pCalcInfo->m_pStkData[i].m_fLow < min_count)
		{
			min_count = pCalcInfo->m_pStkData[i].m_fLow;
			min_pos = i;
		}
				
	}

	/*第二步：对于存在最值点时，确认是否为底分型的最低点，判断方法是看其左右两边是否分别形成了M_sharp长度的标准上升序列*/

	/*首先简要判断是否满足底分型的初步特征，以减少无谓的计算量*/
	if( !((min_pos - N1_pos)>=(M_sharp-1) && (N2_pos -min_pos)>=(M_sharp-1)) )
	{
		ret_flag = 0;
		return ret_flag;
	}
	
	tem_high = pCalcInfo->m_pStkData[min_pos].m_fHigh;
	tem_low = pCalcInfo->m_pStkData[min_pos].m_fLow;
	fengxing_cnt = 0;
	/*对右侧运算*/
	for(i = min_pos ; i < N2_pos ; i ++)
	{
		/*形成一个标准上升序列*/
		if( (pCalcInfo->m_pStkData[i].m_fHigh > tem_high) && (pCalcInfo->m_pStkData[i].m_fLow > tem_low) )
		{
			fengxing_cnt ++;
			tem_high = pCalcInfo->m_pStkData[i].m_fHigh;
			tem_low = pCalcInfo->m_pStkData[i].m_fLow;
		}
        else if((pCalcInfo->m_pStkData[i].m_fHigh >=tem_high) && (pCalcInfo->m_pStkData[i].m_fLow <=tem_low))
        {
            /*包含了前一跟K线，有可能会破坏前期形成的序列，因此简单处理，不做修正*/
            tem_high = pCalcInfo->m_pStkData[i].m_fHigh;
            tem_low = pCalcInfo->m_pStkData[i].m_fLow;
            
        }
#if 1
        else if((pCalcInfo->m_pStkData[i].m_fHigh <tem_high) && (pCalcInfo->m_pStkData[i].m_fLow <tem_low))
        {
            /*形成了一个标准的下降序列了，不严厉的情况下可以走这个分支*/
            tem_high = pCalcInfo->m_pStkData[i].m_fHigh;
            tem_low = pCalcInfo->m_pStkData[i].m_fLow;
            
        }
#endif

        else
        {
            /*被前一跟k线所包含，do nothing*/
            ;
        }

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
		for(i =N1_pos  ; i < min_pos ; i ++)
		{
			/*形成一个标准下降序列*/
			if( (pCalcInfo->m_pStkData[i].m_fHigh < tem_high) && (pCalcInfo->m_pStkData[i].m_fLow < tem_low))
			{
				fengxing_cnt ++;
                tem_high = pCalcInfo->m_pStkData[i].m_fHigh;
                tem_low = pCalcInfo->m_pStkData[i].m_fLow;

			}
            else if((pCalcInfo->m_pStkData[i].m_fHigh >= tem_high) && (pCalcInfo->m_pStkData[i].m_fLow <= tem_low))
            {
                /*包含了前一跟K线，有可能会破坏前期形成的序列，为了保持一致性，简单处理，不做修正*/
                tem_high = pCalcInfo->m_pStkData[i].m_fHigh;
                tem_low = pCalcInfo->m_pStkData[i].m_fLow;
            }
#if 1
            else if((pCalcInfo->m_pStkData[i].m_fHigh >tem_high) && (pCalcInfo->m_pStkData[i].m_fLow > tem_low))
            {
                /*形成了一个标准的上升序列了，不严厉的情况下可以走这个分支*/
                tem_high = pCalcInfo->m_pStkData[i].m_fHigh;
                tem_low = pCalcInfo->m_pStkData[i].m_fLow;
                
            }
#endif

            else
            {
                /*被前一跟k线所包含，do nothing*/
                ;
            }

			/*左边的分型序列已经足够多了，不用再继续进行下去了，节省运算量；同时可确认两边都形成了下降序列，即顶分型成立*/
			if(fengxing_cnt > M_sharp-1)
			{
				ret_flag = min_pos;
			}			
		
		}
	
	}			
				
	return ret_flag;
	
}








///////////////////////////////////////////////////////////////////////////////
//计算 顶分型
//调用方法:
//	"FXJFUNC@MYTEST";

#define TIME 4

__declspec(dllexport) int WINAPI MYTEST(const CALCINFO* pCalcInfo)
{
    int i;
    int pos = 0;
	int top_flag=0;
	int bottom_flag=0;
	int old_top_flag=0;
	int old_bottom_flag=0;

	int cur_top_flag=0;
	int cur_bottom_flag=0;

	for(i=0;i<pCalcInfo->m_nNumData;i++)
	{
        pCalcInfo->m_pResultBuf[i] = 0;

		/*寻找第一个顶或者底，同时出现顶和底的时候，最先出现顶或者底是确定的顶或者底*/
		if(!pos)
		{
			if(i - pos > TIME)
			{
				top_flag = GetOneTopSharp(pCalcInfo,pos,i,TIME);
				bottom_flag = GetOneBottomSharp(pCalcInfo,pos,i,TIME);

                /*开始阶段同时出现顶和底*/
                if(top_flag &&bottom_flag)
                {

                    /*先出现顶后出现底*/
                    if(top_flag >bottom_flag)
                    {
                        pCalcInfo->m_pResultBuf[bottom_flag] = -100;
                        pCalcInfo->m_pResultBuf[top_flag] = 100;
                        old_bottom_flag = bottom_flag;
                        pos = top_flag;
                        bottom_flag = 0;
                        old_top_flag = 0;
                        
                    }

                    /*先出底顶后出现顶*/
                    if(top_flag < bottom_flag)
                    {
                        pCalcInfo->m_pResultBuf[bottom_flag] = -100;
                        pCalcInfo->m_pResultBuf[top_flag] = 100;
                        old_top_flag = top_flag;
                        pos = bottom_flag;
                        top_flag = 0;
                        old_bottom_flag = 0;
                        
                    }


                }
#if 0
				if(top_flag )
				{
					pos = top_flag;
                    bottom_flag = 0;
					pCalcInfo->m_pResultBuf[pos] = 100;
                    old_bottom_flag = 0;
                    old_top_flag = 0;

				}
				if(bottom_flag)
				{
					pos = bottom_flag;
                    top_flag = 0;
                    pCalcInfo->m_pResultBuf[pos] = -100;
                    old_bottom_flag = 0;
                    old_top_flag = 0;

				}
#endif


			}			
		}
		else
		{
			if(i - pos > TIME)
			{
				/*前一次获取到的是顶，那么这次找的是底*/
				if(top_flag)
				{
					/*由于可能出现比前一次的顶更好的顶，因此需要修正一下*/
					cur_top_flag = GetOneTopSharp(pCalcInfo,old_bottom_flag,i,TIME);

                    if(cur_top_flag > top_flag)
                    {
						pCalcInfo->m_pResultBuf[top_flag] = 0;
						pCalcInfo->m_pResultBuf[cur_top_flag] = 100;

                        top_flag = cur_top_flag;
                        pos = cur_top_flag;
                    }

					bottom_flag = GetOneBottomSharp(pCalcInfo,pos,i,TIME);
					if(bottom_flag)
					{
                        old_top_flag = top_flag;
						top_flag = 0;
						pos = bottom_flag;
						pCalcInfo->m_pResultBuf[pos] = -100;
					}
					
				}
				else if(bottom_flag)
				{

					
					cur_bottom_flag = GetOneBottomSharp(pCalcInfo,old_top_flag,i,TIME);

                    if(cur_bottom_flag > bottom_flag)
                    {
						pCalcInfo->m_pResultBuf[bottom_flag] = 0;
						pCalcInfo->m_pResultBuf[cur_bottom_flag] = -100;

                        bottom_flag = cur_bottom_flag;
                        pos = cur_bottom_flag;
                    }

					top_flag = GetOneTopSharp(pCalcInfo,pos,i,TIME);
					if(top_flag)
					{
                        old_bottom_flag=bottom_flag;
						bottom_flag = 0;
						pos = top_flag;
						pCalcInfo->m_pResultBuf[pos] = 100;
                        
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

///////////////////////////////////////////////////////////////////////////////




