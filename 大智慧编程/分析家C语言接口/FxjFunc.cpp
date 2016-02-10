/* ============================BeginningOfHeader===============================
/ ����: FxjFuncSample
/ ����:
/
/ �汾: 1.0
/ ����: Dexin Hui
/ �����ʼ���Dexin.Hui@gmail.com
/ ����: 2006-9-28
/ �����ļ���
/ ����ģ��: 
/ ���Ի���: WinXP_SP2 VC++6.0SP6 FXJ2006 V6.0.00.2200
/ ��ע: ���ļ�����fxj_c++FromHuiTianQi.zip�ı�  ��ӭ����
/ �÷�:
/ �޸�:
/
/------------------------------------------------------------------------------- 
/ ����:�����밴��״("AS IS")�ṩ��û���κ���ȷ�������ĵ�����ʹ�����Լ���е�ʹ�� 
/ ������ķ��ա�����֪ʶ������֯���� 2.5 �й���½���Э������ʹ����ʹ�û��Ʊ�
/ �����Ȩ�ޣ����Խ��������κ���;��ֻҪ�����и����а�������˵������������
/ ֪ʶ������֯���Э�� http://creativecommons.org/licenses/by/2.5/cn/
/===============================EndingOFHeader=================================*/
// FxjFunc.cpp : Defines the entry point for the DLL application.
//
#pragma pack (1) /*ָ����1�ֽڶ���*/

#include "stdafx.h"
#include "FxjFunc.h"
#include <stdlib.h>         
#include <malloc.h>


float  *gmatrix_dingdi_level1=NULL;/*���嵱ǰ���ڵĶ���*/
float  *gmatrix_dingdi_level2=NULL;/*������һ�����ڵĶ���*/
float  *gmatrix_dingdi_level3=NULL;/*��������һ�����ڶ���*/

CALCINFO* gst_pCalcInfo_level1 = NULL;/*���屾���ڵĽṹ*/
CALCINFO* gst_pCalcInfo_level2 = NULL;/*���������ڵĽṹ*/
CALCINFO* gst_pCalcInfo_level3 = NULL;/*�����������ڵĽṹ*/

STPrivateData gst_MYPrivData1;/*���屾�������õ�����������*/
STPrivateData gst_MYPrivData2;/*�����ϼ������õ�����������*/
STPrivateData gst_MYPrivData3;/*�������������õ�����������*/



/*���õ��ⲿ��������*/
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
		/*ÿ�ε��ø��ļ�ʱ������,��Ҫ���ڴ�ȳ�ʼ������*/
		case DLL_PROCESS_ATTACH:
        {
			/*ĳ���ڵĽṹm_pStkData�ṹ������������ϸ��������о��󳤶�һ��*/
            gst_MYPrivData1.m_pStkData_len = MAXMATRIX;
            gst_MYPrivData2.m_pStkData_len = (MAXMATRIX/ZHOUQIZHUANHUANLV+4);
            gst_MYPrivData3.m_pStkData_len = (gst_MYPrivData2.m_pStkData_len/ZHOUQIZHUANHUANLV+4);

            /*�Ӷ���������һ���ֿռ���������ױ�Ǵ洢������*/
            if(NULL == gmatrix_dingdi_level1 )
                gmatrix_dingdi_level1 = (float*) malloc((sizeof(float))*gst_MYPrivData1.m_pStkData_len);
            if(NULL == gmatrix_dingdi_level2 )
                gmatrix_dingdi_level2 = (float*) malloc((sizeof(float))*gst_MYPrivData2.m_pStkData_len);
            if(NULL == gmatrix_dingdi_level3 )
                gmatrix_dingdi_level3 = (float*) malloc((sizeof(float))*gst_MYPrivData3.m_pStkData_len);

            /*�Ӷ������һ���ֿռ���߼������ڲ����洢����HIGH,LOW,VOL,�ȵȣ�ʵ�ֿ����ڷ��������ǵ�����Ч�ʵ�����
                ��˳�ʼ����ʱ��ͷ���һ���Ƚϴ�Ŀռ䣬�����Ҫ����ռ��ʱ�����з���*/
            /*gst_MYPrivData1ָ�����ϵͳ����������С���ڳ�ʼ���ݣ���˲���Ҫ��������ռ���*/
            gst_MYPrivData2.pCalcInfo_level->m_pStkData = (STKDATA *)malloc((sizeof(STKDATA))*gst_MYPrivData2.m_pStkData_len);
            gst_MYPrivData3.pCalcInfo_level->m_pStkData = (STKDATA *)malloc((sizeof(STKDATA))*gst_MYPrivData3.m_pStkData_len);            
            break;
        }
		
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
        {
            /*�ͷŵ�Ӷ�������Ŀռ�*/
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
//���� N ���ھ���,һ����������,��ʾ��������
//���÷���:
//	"FXJFUNC@MYMAVOL"(N);
/*����һ�����Ժ���*/
__declspec(dllexport) int WINAPI MYMAVOL(const CALCINFO* pCalcInfo)
{
	float fTotal;
	int nPeriod,i,j;
	if (pCalcInfo->m_pfParam1==NULL ||							//����1��Ч?
		pCalcInfo->m_nParam1Start>=0 ||							//����1Ϊ����?
		pCalcInfo->m_pfParam2)										//����һ������?
	   return -1;
	nPeriod = (int)*pCalcInfo->m_pfParam1;						//����1
	if (nPeriod < 2)													//����1�Ϸ�?
	   return -1;
	for(i=nPeriod-1;i<pCalcInfo->m_nNumData;i++)		//����nPeriod���ڵľ���,����
	{																//��nPeriod-1��ʼ��Ч
		fTotal = 0.0f;
		for(j=0;j<nPeriod;j++)										//�ۼ�
			fTotal += pCalcInfo->m_pStkData[i-j].m_fVolume;
		pCalcInfo->m_pResultBuf[i] = fTotal/nPeriod;			//ƽ��
	}
	return nPeriod-1;
}

///////////////////////////////////////////////////////////////////////////////
// ������Ի�Ծ�� 3������ (Relative Active Rate)
// ����1��Ϊ�������� (INDEXH-INDEXL)/REF(INDEXC,1) 
//        ��ֵ��Χ���� ��1: >= 0 
//                       2���ǵ�ͣ10% �� < 0.21
// ����2��fAlpha  Ϊ�������� ����ϵ��        ȡֵ��Χ��1--100
// ����3��nPeriod Ϊ�������� �ƶ�ƽ������    ȡֵ��Χ��2--210
// �㷨��
//            (HIGH-LOWER)/REF(CLOSE,1)
//     (-------------------------------------- +...)/nPeriod
//              m_pfParam1 + fAlpha
//
// ����ֵ��Χ���� fAlpha=0.21 ʱ  0 <= MYRARVAR <= 100  
// ���÷���:
//	"FXJFUNC@MRARVAR"((INDEXH-INDEXL)/REF(INDEXC,1),ALPHA,PERIOD);
/*����һ�����Ժ���*/

__declspec(dllexport) int WINAPI MRARVAR(const CALCINFO* pCalcInfo)
{
	float fTotal,fTemp;
	int nFirst,i,j;
	if (pCalcInfo->m_pfParam1==NULL ||							//���� 1 ��Ч?
		 pCalcInfo->m_pfParam2==NULL ||							//���� 2 ��Ч?
		 pCalcInfo->m_pfParam3==NULL ||							//���� 3 ��Ч?
		 pCalcInfo->m_nParam1Start<0 ||							//���� 1 Ϊ������?
		 pCalcInfo->m_pfParam4)										//���� 4 ��Ч?
		return -1;
	nFirst = pCalcInfo->m_nParam1Start;							//��Чֵ
	const int nPeriod = (int)*pCalcInfo->m_pfParam3;		//����2
	const float fAlpha = *pCalcInfo->m_pfParam2/10000.0f;
	if (nPeriod < 2)
		return -1;
	for(i=nFirst+(nPeriod-1)+1;i<pCalcInfo->m_nNumData;i++)
	{
		fTotal = 0.0f;
		for(j=0;j<nPeriod;j++)										//�ۼ�
		{
			fTemp = pCalcInfo->m_pStkData[i-j].m_fHigh - 
								pCalcInfo->m_pStkData[i-j].m_fLow;
			if (pCalcInfo->m_pStkData[(i-j)-1].m_fClose>0.01f)
				fTemp = fTemp / pCalcInfo->m_pStkData[(i-j)-1].m_fClose;
			else
				fTemp = 0.01f;
			fTotal += fTemp/(pCalcInfo->m_pfParam1[i-j] + fAlpha);
			
		}
		pCalcInfo->m_pResultBuf[i] = fTotal/(*pCalcInfo->m_pfParam3);	//ƽ��
	}
	return nFirst+(nPeriod-1)+1;
}







///////////////////////////////////////////////////////////////////////////////
//���� ������
//���÷���:
//	"FXJFUNC@MYTEST";��ǰ���ڼ���
//
///////////////////////////////////////////////////////////////////////////////
extern "C" __declspec(dllexport) int WINAPI MYTEST(const CALCINFO* pCalcInfo)
{
    int i;
    /*����߽��飬�����˴�ǰ����Ķ�̬�ڴ��С�ˣ�����ͷŵ���������һ����ڴ�*/
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
//���� ������
//���÷���:
//	"FXJFUNC@MYTEST";��һ�����ڼ���
//
///////////////////////////////////////////////////////////////////////////////

extern "C" __declspec(dllexport) int WINAPI MYTESTI(const CALCINFO* pCalcInfo)
{
    int i;
    /*����߽��飬�����˴�ǰ����Ķ�̬�ڴ��С�ˣ�����ͷŵ���������һ����ڴ�*/
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

   /*�ú�������С���ڵ���ֵ���������������ֵ���������1���ӵĸߵ͵�����5���ӵĸߵ͵�*/
    ConstrPrivData(&gst_MYPrivData2,&gst_MYPrivData1,ZHOUQIZHUANHUANLV);

	/*������һ���ڶ��׷�������*/
    CalcuDingDiFenXing((CALCINFO*)(gst_MYPrivData2.pCalcInfo_level),gmatrix_dingdi_level2,TIME);

	/*��������*/
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
//���� ������
//���÷���:
//	"FXJFUNC@MYTEST";���������ڼ���
//
///////////////////////////////////////////////////////////////////////////////

extern "C" __declspec(dllexport) int WINAPI MYTESTII(const CALCINFO* pCalcInfo)
{
    int i;
    /*����߽��飬�����˴�ǰ����Ķ�̬�ڴ��С�ˣ�����ͷŵ���������һ����ڴ�*/
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


#pragma pack () /*Ĭ���ֽڶ���*/









