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

#ifndef __FXJFUNC_H_INCLUDE
#define __FXJFUNC_H_INCLUDE

/*
///////////////////////////////////////////////////////////////////////////////
��������չ�����淶V3.10
1.���淶�����ڷ�����3.10��׼���רҵ�湫ʽϵͳ.
2.��չ��������ʵ��ϵͳ��������ʵ�ֵ������㷨.
3.��չ������windows 32λ��̬���ӿ�ʵ��,����ʹ��Microsoft Visual C++���.
4.����ʱ�ڹ�ʽ�༭����д"��̬������@��������"(����,����...);����,�������溯��
  ����дΪ"FXJFUNC@MYMAVOL"(5);
5.��̬���ӿ����ƺͺ������ƿ����Լ�����.
6.ʹ��ʱ���Խ���̬�⿽����������Ŀ¼��ʹ��.
*/

#ifdef __cplusplus
extern "C"
{
#endif //__cplusplus
#pragma pack (1) /*ָ����1�ֽڶ���*/
///////////////////////////////////////////////////////////////////////////////
//��������
enum PERIOD_TYPE
{
	TICK_DATA  =  2,					//�ֱʳɽ�
	MIN1_DATA  =  3,					//1������
	MIN5_DATA  =  4,					//5������					
	MIN15_DATA =  5,					//15������
	MIN30_DATA =  6,					//30������
	MIN60_DATA =  7,					//60������
	DAY_DATA   =  8,					//����
	WEEK_DATA  =  9,					//����
	MONTH_DATA = 10,					//����
	MULTI_DATA = 11					//������
};

///////////////////////////////////////////////////////////////////////////////
//��������

typedef struct tagSTKDATA	
{
	time_t	m_time;			//ʱ��,UCT
	float		m_fOpen;			//����
	float		m_fHigh;			//���
	float		m_fLow;			//���
	float		m_fClose;		//����
	float		m_fVolume;		//�ɽ���
	float		m_fAmount;		//�ɽ���
	WORD		m_wAdvance;		//���Ǽ���(��������Ч)
	WORD		m_wDecline;		//�µ�����(��������Ч)
} STKDATA;


///////////////////////////////////////////////////////////////////////////////
//��չ����,���������ֱʳɽ����ݵ�������

typedef union tagSTKDATAEx
{
	struct
	{
		float m_fBuyPrice[3];		//��1--��3��
		float m_fBuyVol[3];			//��1--��3��
		float m_fSellPrice[3];		//��1--��3��	
		float m_fSellVol[3];			//��1--��3��
	};
	float m_fDataEx[12];				//����
} STKDATAEx;

///////////////////////////////////////////////////////////////////////////////
//��������˳��(m_pfFinData����)

//	���	����

// 	0	�ܹɱ�(���),
// 	1	���ҹ�,
// 	2	�����˷��˹�,
// 	3	���˹�,
// 	4	B��,
// 	5	H��,
// 	6	��ͨA��,
// 	7	ְ����,
// 	8	A2ת���,
// 	9	���ʲ�(ǧԪ),
// 	10	�����ʲ�,
// 	11	�̶��ʲ�,
// 	12	�����ʲ�,
// 	13	����Ͷ��,
// 	14	������ծ,
// 	15	���ڸ�ծ,
// 	16	�ʱ�������,
// 	17	ÿ�ɹ�����,
// 	18	�ɶ�Ȩ��,
// 	19	��Ӫ����,
// 	20	��Ӫ����,
// 	21	��������,
// 	22	Ӫҵ����,
// 	23	Ͷ������,
// 	24	��������,
// 	25	Ӫҵ����֧,
// 	26	�����������,
// 	27	�����ܶ�,
// 	28	˰������,
// 	29	������,
// 	30	δ��������,
// 	31	ÿ��δ����,
// 	32	ÿ������,
// 	33	ÿ�ɾ��ʲ�,
// 	34	����ÿ�ɾ���,
// 	35	�ɶ�Ȩ���,
// 	36	����������


///////////////////////////////////////////////////////////////////////////////
//�������ݽṹ

typedef struct tagCALCINFO
{
	const DWORD					m_dwSize;				//�ṹ��С
	const DWORD					m_dwVersion;			//��������汾(V2.10 : 0x210)
	const DWORD					m_dwSerial;				//����������к�
	const char *const			m_strStkLabel;			//��Ʊ����
	const BOOL					m_bIndex;				//ָ��

	const int					m_nNumData;				//��������(pCalcInfo,pStkDataEx,
																//			    pResultBuf��������)
	const STKDATA *const		m_pStkData;				//��������,ע��:��m_nNumData==0
	                                             //ʱ����Ϊ NULL
	const STKDATAEx *const	m_pStkDataEx;			//��չ����,�ֱʳɽ�������,
	                                             //ע��:����Ϊ NULL

	const int					m_nParam1Start;		//����1��Чλ��
	const float *const		m_pfParam1;				//���ò���1	
	const float *const		m_pfParam2;				//���ò���2
	const float *const		m_pfParam3;				//���ò���3
	const float *const		m_pfParam4;				//���ò���4

	float *const				m_pResultBuf;			//���������
	const PERIOD_TYPE			m_ePeriodType;			//��������
	const float* const		m_pfFinData;			//��������
} CALCINFO;




typedef struct tagCALCINFO1
{
	 DWORD					m_dwSize;				//�ṹ��С
	 DWORD					m_dwVersion;			//��������汾(V2.10 : 0x210)
	 DWORD					m_dwSerial;				//����������к�
	 char *			m_strStkLabel;			//��Ʊ����
	 BOOL					m_bIndex;				//ָ��
	
	 int					m_nNumData;				//��������(pCalcInfo,pStkDataEx,
	//			    pResultBuf��������)
	 STKDATA *		m_pStkData;				//��������,ע��:��m_nNumData==0
	//ʱ����Ϊ NULL
	 STKDATAEx *	m_pStkDataEx;			//��չ����,�ֱʳɽ�������,
	//ע��:����Ϊ NULL
	
	 int					m_nParam1Start;		//����1��Чλ��
	 float *		m_pfParam1;				//���ò���1	
	 float *		m_pfParam2;				//���ò���2
	 float *		m_pfParam3;				//���ò���3
	 float *		m_pfParam4;				//���ò���4
	
	float *				m_pResultBuf;			//���������
	 PERIOD_TYPE			m_ePeriodType;			//��������
	 float* 		m_pfFinData;			//��������
} CALCINFO1;





/*�Լ�����ĺ�ͽṹ*/
#define NULL 0
#define INPUT 
#define OUTPUT

/*������͵ı�׼���и���*/
#define TIME 4

#define MAXMATRIX      20000 /*������������������ֵ*/
#define MAXIMCAREBI  10


/*��������ں������ڣ������ں�С����֮���ת����һ����Ϊ���5-6������1���Ӻ�5���ӣ�5���Ӻ�30����*/
#define ZHOUQIZHUANHUANLV    5


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
	float matrix_dingdi_fudubizhi[MAXIMCAREBI]; /*�������еĶ������ߵķ��ȱ�ֵ*/
	float matrix_dingdi_xielv[MAXIMCAREBI]; /*�������еĶ������ߵ�б��*/
	float matrix_dingdi_mianji[MAXIMCAREBI]; /*�������еĶ������ߵ����*/

	/*
	������αʶ�Ӧ�ĳ��ȡ����ȡ�б�ʡ������Ϊ�����ʾ���������ʵ����ԣ�Ϊ�����ʾ�½��ʵ�����
	��Ҫ˵�����ǱȽϵ��ص���ȷ��������С�ķ������û�б�Ҫר�Ż���������
	*/

	unsigned int current_stat;/*��ǰ״̬��Ϊ������δ���ۣ����Ǳ��ۡ��µ�δ���ۡ��µ����ۡ���������δ���ۡ������µ�δ����
	���������Ǳ��ۡ������µ�����*/

	unsigned int beichilidu;/*��۶��嵱ǰ�ڵ���ɵıʶ�̬�����ǻ����µ��ı������ȣ������ۺ��жϷ�Ϊ
	���Ǳ��ۡ�����δ���ۡ��µ����ۡ��µ�δ���ۡ�������΢���ۡ��µ���΢���ۡ��޷��жϣ����ۻ���һ��
	�ɽ����ı��ۣ���Ҫ�ص㿼��*/

	unsigned int dianweiyuce;/*Ԥ�⵱ǰ��һ������ܵĽ���λ�ã�Ԥ������û������ɿ��Ķ������������ȷʵ��һ�ʵ����λ�ø���������
	���ۣ�������һ�������ڸ�λ�ø������������㣬��Ҫ�ص�Դ������û�а���Ԥ����ߣ����ù���
	 */

	unsigned int buy_sell_point;/* 
	���嵱ǰ�������������������䣬��һ��㡢�ڶ���㡢������㣬��һ����
	���ڶ���㡢�������㣬�ֹ����䡢�ֱ����䣬��һ��������
	���������Ҫ�Ľ����Ͳ������ɾݴ�������Ʊ����Ȼ���������������ȷ������������������ͨ��
	macd��kdj����������ж�֮���������ڶ���㣬��һ�������Ϊ�ӵײ��õģ���ȫ�ĵڶ������Լӵ����λ
	���ֵ�����������һ�ڶ����İ�ȫ�ԣ��������ֻ������ִ����ˡ��������෴��
	������������������С���ڵĶ�������ȷ������С�����л�����ͨ����������С��λ�Ĳ�ۣ����������
	���ڼ���������ڼ�����ս����ȷ�����ܵĸߵ͵㣬���۳̶ȵȵȡ������Ͽ��������µ�����δ����
	��������β��ܽ�ȥ������������Ǳ��ۣ�����Ҫ����ע����һ�����ڼ���������ˡ�
	�м������������ȷ����������Ҫ���ݣ���С��������������ȷȷ�������������λ�ģ�ͨ�������Ը���С�����
	��������һЩ�̲�
	*/
	/************************************************************************/
	float zhisundianwei;/*ĳ��������˵��һ����ǲ��ȶ��ģ�Ҳ����˵�ᱻͻ���¼��������������ƻ��������ʱ��Ҫ����
		ֹ�𣬵�Ȼ���������ʱ����������ģ��������Сת����������������������������Ҫһ��ֹ���
		�������λ����������ȷ��֮����ʵ��ǰ����ӯ��������»��б�Ҫ����һ��ֹӯ������̬Ӧ�����ܳ��ֵķ��ա�
		������������γ��ֵģ���Ҳ��֤����ӯ���Ļ����ϼ���
	 */


} STPrivateData;

#pragma pack () /*Ĭ���ֽڶ���*/
///////////////////////////////////////////////////////////////////////////////
/* ����˵��

__declspec(dllexport) int xxxxxxxx(CALCINFO* pCalcInfo);	   ---------- A
__declspec(dllexport) int xxxxxxxxVAR(CALCINDO* pCalcInfo);	---------- B

1. ����������ȫ����д.

2. ��������������A,B������ʽ֮һ����,����ʵ�ʺ����������xxxxxxxx;
	����C++����������� extern "C" {   } ������.

3. ������ʽA ������������������m_pfParam1Ϊ���������ĺ���;
             m_nParam1Start < 0
	    ��ʽB ��������m_pfParam1Ϊ�����������ĺ�����m_pfParam1ָ��һ������������
		       ���������������СΪm_nNumData, m_nParam1Start >= 0
				 ������Ч��ΧΪm_nParam1Start--(m_nNumData-1);
				 m_nParam1Start = 0 ��ʾ�����ӵ�һ����ʼ��Ч��
	�������m_pfParam2--m_pfParam4ֻ��Ϊ��������.
	A B ���ຯ������������ B ��VAR��β.

4. ������������pCalcInfo->m_pResultBuf����.

5. ��������-1��ʾ�����ȫ��������Ч,���򷵻�m_pResultBuf�е�һ����Чֵλ��,
   ��: m_pResultBuf[����ֵ] -- m_pResultBuf[m_nNumData-1] �������Ϊ��Ч��������.

6. ��ʱ����m_pStkData[x]��m_pfParam1[x]��һ�µģ���Ӧ��֤m_pResultBuf[x]��ǰ�ߵ�
   ʱ��һ���ԡ���Ȼ�������m_pResultBuf[x]��ǰ�ߵ�ʱ�䲻һ�£�����Ȩ���㡣

6. �������Ƴ��Ȳ��ܳ���15�ֽ�,��̬���ӿ��ļ������ܳ���9�ֽ�(��������չ��),��̬��
   ���Ʋ��ܽ�SYSTEM,EXPLORER

7. ����ʱ����ѡ��1�ֽڶ���
*/

///////////////////////////////////////////////////////////////////////////////
//ʾ������                                                ע�� XXXXXXXX ���滻

__declspec(dllexport) int WINAPI MYMAVOL(const CALCINFO* pCalcInfo); //-------A
__declspec(dllexport) int WINAPI MRARVAR(const CALCINFO* pCalcInfo); //-------B
__declspec(dllexport) int WINAPI MYTEST(const CALCINFO* pCalcInfo);
__declspec(dllexport) int WINAPI MYTESTI(const CALCINFO* pCalcInfo);
__declspec(dllexport) int WINAPI MYTESTII(const CALCINFO* pCalcInfo);

#ifdef __cplusplus
}
#endif //__cplusplus

#endif //__FXJFUNC_H_INCLUDE





















