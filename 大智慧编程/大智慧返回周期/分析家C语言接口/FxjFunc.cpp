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
//���� N ���ھ���,һ����������,��ʾ��������
//���÷���:
//	"FXJFUNC@MYMAVOL"(N);

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
// ����ֵ��Χ���� fAlpha=0.21 ʱ  0 <= MYRARVAR	"FXJFUNC@MRARVAR"((INDEXH-INDEXL)/REF(INDEXC,1),ALPHA,PERIOD) <= 100  
// ���÷���:
//;

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


//"ZqFunc@MYZHOUQI"
__declspec(dllexport) int WINAPI MYZHOUQI(const CALCINFO* pCalcInfo)
{
	int i;
	/*�޲�������*/
//	if (pCalcInfo->m_pfParam1)										
//		return -1;
	for(i=0;i<pCalcInfo->m_nNumData;i++)	
	{	
		
		pCalcInfo->m_pResultBuf[i] = (float)pCalcInfo->m_dataType;		
	//	pCalcInfo->m_pResultBuf[i] = 2;		
	}
	return 0;
}