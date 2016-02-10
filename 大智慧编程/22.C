OUTPUT int GetOneBottomSharp(INPUT const CALCINFO* pCalcInfo,
INPUT unsigned int cur_pos,INPUT unsigned int N1_pos,INPUT unsigned N2_pos,INPUT unsigned int M_sharp)
{
	int ret_flag = 0;
	unsigned int i;
	unsigned int min_pos;/*底分型的位置*/
	float min_count = 0;/*由于序列为股价，因此初始化的最大值为10000是合理的*/

	float	tem_high ;
	float	tem_low;
	unsigned int fengxing_cnt;

	
	/*输入参数有效性验证*/
	if(NULL == pCalcInfo )
	{
		ret_flag = 0;
		return ret_flag;
	}
	if(!((0<=N1_pos) && (N1_pos<N2_pos) && (N2_pos<=cur_pos)
		&& (cur_pos<MAXMATRIX) && (0<M_sharp)  &&  (M_sharp <(N2_pos-N1_pos))))
	{
		ret_flag = 0;
		return ret_flag;
	}

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
	if( !((min_pos - N1_pos)>=M_sharp && ((N2_pos -min_pos)>=M_sharp)) )
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
		}

		/*判断底分型，因此每次都同前期最恶劣的情况对比*/
		if( pCalcInfo->m_pStkData[i].m_fHigh > tem_high) 
		{
			tem_high = pCalcInfo->m_pStkData[i].m_fHigh;
		}
		if( pCalcInfo->m_pStkData[i].m_fLow > tem_low )
		{
			tem_low = pCalcInfo->m_pStkData[i].m_fLow;
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
		tem_high = pCalcInfo->m_pStkData[min_pos].m_fHigh;
		tem_low = pCalcInfo->m_pStkData[min_pos].m_fLow;
		fengxing_cnt = 0;
		/*对左侧运算*/
		for(i =N1_pos  ; i < min_pos ; i ++)
		{
			/*形成一个标准上升序列*/
			if( (pCalcInfo->m_pStkData[i].m_fHigh > tem_high) && (pCalcInfo->m_pStkData[i].m_fLow > tem_low) )
			{
				fengxing_cnt ++;
			}

			/*判断顶分型，因此每次都同前期最恶劣的情况对比*/
			if( pCalcInfo->m_pStkData[i].m_fHigh > tem_high) 
			{
				tem_high = pCalcInfo->m_pStkData[i].m_fHigh;
			}
			if( pCalcInfo->m_pStkData[i].m_fLow > tem_low )
			{
				tem_low = pCalcInfo->m_pStkData[i].m_fLow;
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



