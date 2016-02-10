
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
#if 0
	/*考虑到有些k线数据不全导致找不到分型点，过滤不正常的k线*/
    if(1000<N2_pos-N1_pos)
        N1_pos=N2_pos-1000;
#endif

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
	if( !((max_pos - N1_pos)>=M_sharp && ((N2_pos -max_pos)>=M_sharp))) 
	{
		ret_flag = 0;
		return ret_flag;
	}
	
	tem_high = pCalcInfo->m_pStkData[max_pos].m_fHigh;
	tem_low = pCalcInfo->m_pStkData[max_pos].m_fLow;
	fengxing_cnt = 0;
	/*对右侧运算*/
	for(i = max_pos ; i <= N2_pos ;i++)
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
        else if((pCalcInfo->m_pStkData[i].m_fHigh >=tem_high) && (pCalcInfo->m_pStkData[i].m_fLow >= tem_low))
        {
            /*形成了一个标准的上升序列了，不严厉的情况下可以走这个分支*/
            tem_high = pCalcInfo->m_pStkData[i].m_fHigh;
            tem_low = pCalcInfo->m_pStkData[i].m_fLow;
            
        }

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
		for(i =N1_pos  ; i <= max_pos ; i++)
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
            else if((pCalcInfo->m_pStkData[i].m_fHigh <=tem_high) && (pCalcInfo->m_pStkData[i].m_fLow <=tem_low))
            {
                /*形成了一个标准的下降序列了，不严厉的情况下可以走这个分支*/
                tem_high = pCalcInfo->m_pStkData[i].m_fHigh;
                tem_low = pCalcInfo->m_pStkData[i].m_fLow;
                
            }
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



