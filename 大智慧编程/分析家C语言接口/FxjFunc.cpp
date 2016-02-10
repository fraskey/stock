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
#pragma pack (1) /*指定按1字节对齐*/

#include "stdafx.h"
#include "FxjFunc.h"
#include <stdlib.h>         
#include <malloc.h>


float  *gmatrix_dingdi_level1=NULL;/*定义当前周期的顶底*/
float  *gmatrix_dingdi_level2=NULL;/*定义上一个周期的顶底*/
float  *gmatrix_dingdi_level3=NULL;/*定义上上一个周期顶底*/

CALCINFO* gst_pCalcInfo_level1 = NULL;/*定义本周期的结构*/
CALCINFO* gst_pCalcInfo_level2 = NULL;/*定义上周期的结构*/
CALCINFO* gst_pCalcInfo_level3 = NULL;/*定义上上周期的结构*/

STPrivateData gst_MYPrivData1;/*定义本级周期用到的所有数据*/
STPrivateData gst_MYPrivData2;/*定义上级周期用到的所有数据*/
STPrivateData gst_MYPrivData3;/*定义上上周期用到的所有数据*/



/*引用的外部函数集合*/
extern OUTPUT CalcuDingDiFenXing(INPUT const CALCINFO* pCalcInfo,float* 
								 matrix_dingdi,unsigned int fengxin_time);


extern OUTPUT  int  ConstrPrivData(OUTPUT STPrivateData* HLevel_PriData, INPUT STPrivateData* 
Original_PriData,INPUT unsigned int zqzhuanhuanlv);




extern "C" BOOL APIENTRY DllMain( HANDLE /*hModule*/, 
                       DWORD  ul_reason_for_call, 
                       LPVOID /*lpReserved*/ )
{
    switch (ul_reason_for_call)
	{
		/*每次调用该文件时调用它,主要做内存等初始化工作*/
		case DLL_PROCESS_ATTACH:
        {
			/*某周期的结构m_pStkData结构体个数，理论上跟顶底序列矩阵长度一致*/
            gst_MYPrivData1.m_pStkData_len = MAXMATRIX;
            gst_MYPrivData2.m_pStkData_len = (MAXMATRIX/ZHOUQIZHUANHUANLV+4);
            gst_MYPrivData3.m_pStkData_len = (gst_MYPrivData2.m_pStkData_len/ZHOUQIZHUANHUANLV+4);

            /*从堆里面分配出一部分空间出来给顶底标记存储变量用*/
            if(NULL == gmatrix_dingdi_level1 )
                gmatrix_dingdi_level1 = (float*) malloc((sizeof(float))*gst_MYPrivData1.m_pStkData_len);
            if(NULL == gmatrix_dingdi_level2 )
                gmatrix_dingdi_level2 = (float*) malloc((sizeof(float))*gst_MYPrivData2.m_pStkData_len);
            if(NULL == gmatrix_dingdi_level3 )
                gmatrix_dingdi_level3 = (float*) malloc((sizeof(float))*gst_MYPrivData3.m_pStkData_len);

            /*从堆里分配一部分空间给高级别周期参数存储，如HIGH,LOW,VOL,等等，实现跨周期分析，考虑到利用效率的问题
                因此初始化的时候就分配一个比较大的空间，如果需要更大空间的时候再行分配*/
            /*gst_MYPrivData1指向的是系统传进来的最小周期初始数据，因此不需要单独分配空间了*/
            gst_MYPrivData2.pCalcInfo_level->m_pStkData = (STKDATA *)malloc((sizeof(STKDATA))*gst_MYPrivData2.m_pStkData_len);
            gst_MYPrivData3.pCalcInfo_level->m_pStkData = (STKDATA *)malloc((sizeof(STKDATA))*gst_MYPrivData3.m_pStkData_len);            
            break;
        }
		
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
        {
            /*释放点从堆里申请的空间*/
            if(NULL != gmatrix_dingdi_level1)
            {
                free(gmatrix_dingdi_level1);
                gmatrix_dingdi_level1 = NULL;
            }
            if(NULL != gmatrix_dingdi_level2)
            {
                free(gmatrix_dingdi_level2);
                gmatrix_dingdi_level2 = NULL;
            }
            if(NULL != gmatrix_dingdi_level3)
            {
                free(gmatrix_dingdi_level3);
                gmatrix_dingdi_level3 = NULL;
            }

            if(NULL != gst_MYPrivData2.pCalcInfo_level->m_pStkData)
            {
                free(gst_MYPrivData2.pCalcInfo_level->m_pStkData);
                gst_MYPrivData2.pCalcInfo_level->m_pStkData = NULL;
            }

            if(NULL != gst_MYPrivData3.pCalcInfo_level->m_pStkData)
            {
                free(gst_MYPrivData3.pCalcInfo_level->m_pStkData);
                gst_MYPrivData3.pCalcInfo_level->m_pStkData = NULL;
            }

            break;
        }           

		default:
			return FALSE;
    }
    return TRUE;
}
///////////////////////////////////////////////////////////////////////////////
//计算 N 周期均量,一个常数参数,表示计算周期
//调用方法:
//	"FXJFUNC@MYMAVOL"(N);
/*这是一个测试函数*/
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
// 返回值范围：当 fAlpha=0.21 时  0 <= MYRARVAR <= 100  
// 调用方法:
//	"FXJFUNC@MRARVAR"((INDEXH-INDEXL)/REF(INDEXC,1),ALPHA,PERIOD);
/*这是一个测试函数*/

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







///////////////////////////////////////////////////////////////////////////////
//计算 顶分型
//调用方法:
//	"FXJFUNC@MYTEST";当前周期级别
//
///////////////////////////////////////////////////////////////////////////////
extern "C" __declspec(dllexport) int WINAPI MYTEST(const CALCINFO* pCalcInfo)
{
    int i;
    /*数组边界检查，超过了此前申请的动态内存大小了，因此释放掉重新申请一块大内存*/
    if(pCalcInfo->m_nNumData >= (MAXMATRIX-1))
	{
		if(NULL != gmatrix_dingdi_level1)
		{
			free(gmatrix_dingdi_level1);
		}
		gmatrix_dingdi_level1 = (float*) malloc((sizeof(float))*(pCalcInfo->m_nNumData + 1));
		if(NULL == gmatrix_dingdi_level1)
			return -1;
	}
	else
	{
		if(NULL == gmatrix_dingdi_level1)
		{
			gmatrix_dingdi_level1 = (float*) malloc((sizeof(float))*MAXMATRIX);
			if(NULL == gmatrix_dingdi_level1)
				return -1;
		}
		
	}

	gst_MYPrivData1.pCalcInfo_level = (CALCINFO1*)pCalcInfo;

	gst_MYPrivData1.matrix_dingdi_xulie = gmatrix_dingdi_level1;

    CalcuDingDiFenXing((CALCINFO*)(gst_MYPrivData1.pCalcInfo_level),gst_MYPrivData1.matrix_dingdi_xulie,TIME);

	for(i=0;i<pCalcInfo->m_nNumData;i++)
	{
        pCalcInfo->m_pResultBuf[i] =  gst_MYPrivData1.matrix_dingdi_xulie[i];

	}
	return 0;
}


///////////////////////////////////////////////////////////////////////////////
//计算 顶分型
//调用方法:
//	"FXJFUNC@MYTEST";高一个周期级别
//
///////////////////////////////////////////////////////////////////////////////

extern "C" __declspec(dllexport) int WINAPI MYTESTI(const CALCINFO* pCalcInfo)
{
    int i;
    /*数组边界检查，超过了此前申请的动态内存大小了，因此释放掉重新申请一块大内存*/
    if(pCalcInfo->m_nNumData>= (MAXMATRIX-1))
	{
		if(NULL != gmatrix_dingdi_level2)
		{
			free(gmatrix_dingdi_level2);
		}
		gmatrix_dingdi_level2 = (float*) malloc((sizeof(float))*(pCalcInfo->m_nNumData + 1));
		if(NULL == gmatrix_dingdi_level2)
			return -1;
	}
	else
	{
		if(NULL == gmatrix_dingdi_level2)
		{
			gmatrix_dingdi_level2 = (float*) malloc((sizeof(float))*MAXMATRIX);
			if(NULL == gmatrix_dingdi_level2)
				return -1;
		}
		
	}


	gst_MYPrivData1.pCalcInfo_level = (CALCINFO1*)pCalcInfo;

   /*该函数根据小周期的数值计算出来大周期数值，比如根据1分钟的高低点计算出5分钟的高低点*/
    ConstrPrivData(&gst_MYPrivData2,&gst_MYPrivData1,ZHOUQIZHUANHUANLV);

	/*计算上一周期顶底分型序列*/
    CalcuDingDiFenXing((CALCINFO*)(gst_MYPrivData2.pCalcInfo_level),gmatrix_dingdi_level2,TIME);

	/*粗略修正*/
	for(i=0;i<pCalcInfo->m_nNumData;i++)
	{
		pCalcInfo->m_pResultBuf[i] = 0;
		if(gmatrix_dingdi_level2[i]/ZHOUQIZHUANHUANLV != 0)
		{
    		pCalcInfo->m_pResultBuf[(i/ZHOUQIZHUANHUANLV)*ZHOUQIZHUANHUANLV] =  gmatrix_dingdi_level2[i];
		}
	}
	return 0;
}


#if 0
///////////////////////////////////////////////////////////////////////////////
//计算 顶分型
//调用方法:
//	"FXJFUNC@MYTEST";高两个周期级别
//
///////////////////////////////////////////////////////////////////////////////

extern "C" __declspec(dllexport) int WINAPI MYTESTII(const CALCINFO* pCalcInfo)
{
    int i;
    /*数组边界检查，超过了此前申请的动态内存大小了，因此释放掉重新申请一块大内存*/
    if(pCalcInfo->m_nNumData>= (MAXMATRIX-1))
	{
		if(NULL != gmatrix_dingdi_level3)
		{
			free(gmatrix_dingdi_level3);
		}
		gmatrix_dingdi_level3 = (float*) malloc((sizeof(float))*(pCalcInfo->m_nNumData + 1));
		if(NULL == gmatrix_dingdi_level3)
			return -1;
	}
	else
	{
		if(NULL == gmatrix_dingdi_level3)
		{
			gmatrix_dingdi_level3 = (float*) malloc((sizeof(float))*MAXMATRIX);
			if(NULL == gmatrix_dingdi_level3)
				return -1;
		}
		
	}

    CalcuDingDiFenXing(pCalcInfo,gmatrix_dingdi_level3,TIME*5*5);

	for(i=0;i<pCalcInfo->m_nNumData;i++)
	{
        pCalcInfo->m_pResultBuf[i] =  gmatrix_dingdi_level3[i];

	}
	return 0;
}
#endif


#pragma pack () /*默认字节对齐*/









