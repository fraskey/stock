/* ============================BeginningOfHeader===============================
/ 名称: FxjFuncSample
/ 功能:
/
/ 版本: 1.0
/ 作者: Dexin Hui
/ 电子邮件：Dexin.Hui@gmail.com
/ 日期: 2006-9-28
/ 依附文件：
/ 所需模块: 
/ 测试环境: WinXP_SP2 VC++6.0SP6 FXJ2006 V6.0.00.2200
/ 备注: 本文件基于fxj_c++FromHuiTianQi.zip改编  欢迎纠错
/ 用法:
/ 修改:
/
/------------------------------------------------------------------------------- 
/ 声明:本代码按现状("AS IS")提供，没有任何明确或隐含的担保，使用者自己须承担使用 
/ 本代码的风险。采用知识共享组织署名 2.5 中国大陆许可协议授予使用者使用或复制本
/ 代码的权限，可以将其用于任何用途，只要在所有副本中包含以上说明及本声明。
/ 知识共享组织许可协议 http://creativecommons.org/licenses/by/2.5/cn/
/===============================EndingOFHeader=================================*/
// FxjFunc.cpp : Defines the entry point for the DLL application.
//


#include "stdafx.h"
#include "FxjFunc.h"

BOOL APIENTRY DllMain( HANDLE /*hModule*/, 
                       DWORD  ul_reason_for_call, 
                       LPVOID /*lpReserved*/ )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
		default:
			return FALSE;
    }
    return TRUE;
}
///////////////////////////////////////////////////////////////////////////////
//计算 N 周期均量,一个常数参数,表示计算周期
//调用方法:
//	"FXJFUNC@MYMAVOL"(N);

__declspec(dllexport) int WINAPI MYMAVOL(const CALCINFO* pCalcInfo)
{
	float fTotal;
	int nPeriod,i,j;
	if (pCalcInfo->m_pfParam1==NULL ||							//参数1有效?
		pCalcInfo->m_nParam1Start>=0 ||							//参数1为常数?
		pCalcInfo->m_pfParam2)										//仅有一个参数?
	   return -1;
	nPeriod = (int)*pCalcInfo->m_pfParam1;						//参数1
	if (nPeriod < 2)													//参数1合法?
	   return -1;
	for(i=nPeriod-1;i<pCalcInfo->m_nNumData;i++)		//计算nPeriod周期的均线,数据
	{																//从nPeriod-1开始有效
		fTotal = 0.0f;
		for(j=0;j<nPeriod;j++)										//累加
			fTotal += pCalcInfo->m_pStkData[i-j].m_fVolume;
		pCalcInfo->m_pResultBuf[i] = fTotal/nPeriod;			//平均
	}
	return nPeriod-1;
}

///////////////////////////////////////////////////////////////////////////////
// 计算相对活跃率 3个参数 (Relative Active Rate)
// 参数1：为序列数据 (INDEXH-INDEXL)/REF(INDEXC,1) 
//        数值范围分析 ：1: >= 0 
//                       2：涨跌停10% 后 < 0.21
// 参数2：fAlpha  为常量数据 阻尼系数        取值范围：1--100
// 参数3：nPeriod 为常量数据 移动平均周期    取值范围：2--210
// 算法：
//            (HIGH-LOWER)/REF(CLOSE,1)
//     (-------------------------------------- +...)/nPeriod
//              m_pfParam1 + fAlpha
//
// 返回值范围：当 fAlpha=0.21 时  0 <= MYRARVAR	"FXJFUNC@MRARVAR"((INDEXH-INDEXL)/REF(INDEXC,1),ALPHA,PERIOD) <= 100  
// 调用方法:
//;

__declspec(dllexport) int WINAPI MRARVAR(const CALCINFO* pCalcInfo)
{
	float fTotal,fTemp;
	int nFirst,i,j;
	if (pCalcInfo->m_pfParam1==NULL ||							//参数 1 无效?
		 pCalcInfo->m_pfParam2==NULL ||							//参数 2 无效?
		 pCalcInfo->m_pfParam3==NULL ||							//参数 3 无效?
		 pCalcInfo->m_nParam1Start<0 ||							//参数 1 为常数数?
		 pCalcInfo->m_pfParam4)										//参数 4 有效?
		return -1;
	nFirst = pCalcInfo->m_nParam1Start;							//有效值
	const int nPeriod = (int)*pCalcInfo->m_pfParam3;		//参数2
	const float fAlpha = *pCalcInfo->m_pfParam2/10000.0f;
	if (nPeriod < 2)
		return -1;
	for(i=nFirst+(nPeriod-1)+1;i<pCalcInfo->m_nNumData;i++)
	{
		fTotal = 0.0f;
		for(j=0;j<nPeriod;j++)										//累加
		{
			fTemp = pCalcInfo->m_pStkData[i-j].m_fHigh - 
								pCalcInfo->m_pStkData[i-j].m_fLow;
			if (pCalcInfo->m_pStkData[(i-j)-1].m_fClose>0.01f)
				fTemp = fTemp / pCalcInfo->m_pStkData[(i-j)-1].m_fClose;
			else
				fTemp = 0.01f;
			fTotal += fTemp/(pCalcInfo->m_pfParam1[i-j] + fAlpha);
			
		}
		pCalcInfo->m_pResultBuf[i] = fTotal/(*pCalcInfo->m_pfParam3);	//平均
	}
	return nFirst+(nPeriod-1)+1;
}


//"ZqFunc@MYZHOUQI"
__declspec(dllexport) int WINAPI MYZHOUQI(const CALCINFO* pCalcInfo)
{
	int i;
	/*无参数输入*/
//	if (pCalcInfo->m_pfParam1)										
//		return -1;
	for(i=0;i<pCalcInfo->m_nNumData;i++)	
	{	
		
		pCalcInfo->m_pResultBuf[i] = (float)pCalcInfo->m_dataType;		
	//	pCalcInfo->m_pResultBuf[i] = 2;		
	}
	return 0;
}