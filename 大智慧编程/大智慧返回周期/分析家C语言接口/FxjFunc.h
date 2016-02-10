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


#ifndef __FXJFUNC_H_INCLUDE
#define __FXJFUNC_H_INCLUDE

#pragma pack (1) /*指定按1字节对齐*/
/*
///////////////////////////////////////////////////////////////////////////////
分析家扩展函数规范V3.10
1.本规范适用于分析家3.10标准版和专业版公式系统.
2.扩展函数用于实现系统函数不能实现的特殊算法.
3.扩展函数用windows 32位动态连接库实现,建议使用Microsoft Visual C++编程.
4.调用时在公式编辑器中写"动态库名称@函数名称"(参数,参数...);即可,例如下面函数
  可以写为"FXJFUNC@MYMAVOL"(5);
5.动态连接库名称和函数名称可以自己定义.
6.使用时可以将动态库拷贝到分析家目录下使用.
*/

#ifdef __cplusplus
extern "C"
{
#endif //__cplusplus

///////////////////////////////////////////////////////////////////////////////
//分析周期
enum PERIOD_TYPE
{
	TICK_DATA  =  2,					//分笔成交
	MIN1_DATA  =  3,					//1分钟线
	MIN5_DATA  =  4,					//5分钟线					
	MIN15_DATA =  5,					//15分钟线
	MIN30_DATA =  6,					//30分钟线
	MIN60_DATA =  7,					//60分钟线
	DAY_DATA   =  8,					//日线
	WEEK_DATA  =  9,					//周线
	MONTH_DATA = 10,					//月线
	MULTI_DATA = 11					//多日线
};

///////////////////////////////////////////////////////////////////////////////
//基本数据

typedef struct tagSTKDATA	
{
	time_t	m_time;			//时间,UCT
	float		m_fOpen;			//开盘
	float		m_fHigh;			//最高
	float		m_fLow;			//最低
	float		m_fClose;		//收盘
	float		m_fVolume;		//成交量
	float		m_fAmount;		//成交额
	WORD		m_wAdvance;		//上涨家数(仅大盘有效)
	WORD		m_wDecline;		//下跌家数(仅大盘有效)
} STKDATA;


///////////////////////////////////////////////////////////////////////////////
//扩展数据,用于描述分笔成交数据的买卖盘

typedef union tagSTKDATAEx
{
	struct
	{
		float m_fBuyPrice[3];		//买1--买3价
		float m_fBuyVol[3];			//买1--买3量
		float m_fSellPrice[3];		//卖1--卖3价	
		float m_fSellVol[3];			//卖1--卖3量
	};
	float m_fDataEx[12];				//保留
} STKDATAEx;

///////////////////////////////////////////////////////////////////////////////
//财务数据顺序(m_pfFinData内容)

//	序号	内容

// 	0	总股本(万股),
// 	1	国家股,
// 	2	发起人法人股,
// 	3	法人股,
// 	4	B股,
// 	5	H股,
// 	6	流通A股,
// 	7	职工股,
// 	8	A2转配股,
// 	9	总资产(千元),
// 	10	流动资产,
// 	11	固定资产,
// 	12	无形资产,
// 	13	长期投资,
// 	14	流动负债,
// 	15	长期负债,
// 	16	资本公积金,
// 	17	每股公积金,
// 	18	股东权益,
// 	19	主营收入,
// 	20	主营利润,
// 	21	其他利润,
// 	22	营业利润,
// 	23	投资收益,
// 	24	补贴收入,
// 	25	营业外收支,
// 	26	上年损益调整,
// 	27	利润总额,
// 	28	税后利润,
// 	29	净利润,
// 	30	未分配利润,
// 	31	每股未分配,
// 	32	每股收益,
// 	33	每股净资产,
// 	34	调整每股净资,
// 	35	股东权益比,
// 	36	净资收益率


///////////////////////////////////////////////////////////////////////////////
//函数数据结构
/*
typedef struct tagCALCINFO
{
	const DWORD					m_dwSize;				//结构大小
	const DWORD					m_dwVersion;			//调用软件版本(V2.10 : 0x210)
	const DWORD					m_dwSerial;				//调用软件序列号
	const char *const			m_strStkLabel;			//股票代码
	const BOOL					m_bIndex;				//指数
	const int					m_nNumData;				//数据数量(pCalcInfo,pStkDataEx,																//			    pResultBuf数据数量)
	const STKDATA *const		m_pStkData;				//常规数据,注意:当m_nNumData==0
	                                             //时可能为 NULL
	const STKDATAEx *const	m_pStkDataEx;			//扩展数据,分笔成交买卖盘,
	                                             //注意:可能为 NULL
	const int					m_nParam1Start;		//参数1有效位置
	const float *const		m_pfParam1;				//调用参数1	
	const float *const		m_pfParam2;				//调用参数2
	const float *const		m_pfParam3;				//调用参数3
	const float *const		m_pfParam4;				//调用参数4

	float *const				m_pResultBuf;			//结果缓冲区
	const PERIOD_TYPE			m_ePeriodType;			//数据类型
	const float* const		m_pfFinData;			//财务数据
} CALCINFO;

*/
//函数数据结构
typedef struct tagCALCINFO
{
    const DWORD            m_dwSize;                
    const DWORD            m_dwVersion;            
    const DWORD            m_dwSerial;              
    const char*            m_strStkLabel;            
    const BOOL            m_bIndex;              
    const int            m_nNumData;               
    const STKDATA*        m_pStkData;                
    const STKDATAEx*    m_pDataEx;                
    const int            m_nParam1Start;     
	
    const float*        m_pfParam1;               
    const float*        m_pfParam2;                
    const float*        m_pfParam3;                
    const float*        m_pfParam4;                
	
    float*                m_pResultBuf;           
    const PERIOD_TYPE        m_dataType;                
    const float*        m_pfFinData;            
} CALCINFO;

#pragma pack()//使用默认的字节对齐

///////////////////////////////////////////////////////////////////////////////
/* 函数说明

__declspec(dllexport) int xxxxxxxx(CALCINFO* pCalcInfo);	   ---------- A
__declspec(dllexport) int xxxxxxxxVAR(CALCINDO* pCalcInfo);	---------- B

1. 函数名称需全部大写.

2. 函数必须以上述A,B两种形式之一声明,请用实际函数名称替代xxxxxxxx;
	对于C++程序还需包括在 extern "C" {   } 括号中.

3. 上述形式A 用于声明不带参数或m_pfParam1为常数参数的函数;
             m_nParam1Start < 0
	    形式B 用于声明m_pfParam1为序列数参数的函数，m_pfParam1指向一个浮点型数组
		       序列数参数数组大小为m_nNumData, m_nParam1Start >= 0
				 数据有效范围为m_nParam1Start--(m_nNumData-1);
				 m_nParam1Start = 0 表示参数从第一个开始有效。
	其余参数m_pfParam2--m_pfParam4只能为常数参数.
	A B 两类函数的区别在于 B 以VAR结尾.

4. 函数计算结果由pCalcInfo->m_pResultBuf带回.

5. 函数返回-1表示错误或全部数据无效,否则返回m_pResultBuf中第一个有效值位置,
   即: m_pResultBuf[返回值] -- m_pResultBuf[m_nNumData-1] 间的数据为有效返回数据.

6. 在时间上m_pStkData[x]与m_pfParam1[x]是一致的，你应保证m_pResultBuf[x]与前者的
   时间一致性。当然你可以让m_pResultBuf[x]与前者的时间不一致，决定权在你。

6. 函数名称长度不能超过15字节,动态连接库文件名不能超过9字节(不包括扩展名),动态库
   名称不能叫SYSTEM,EXPLORER

7. 编译时请请选择1字节对齐
*/

///////////////////////////////////////////////////////////////////////////////
//示例函数                                                注意 XXXXXXXX 的替换

__declspec(dllexport) int WINAPI MYMAVOL(const CALCINFO* pCalcInfo); //-------A
__declspec(dllexport) int WINAPI MRARVAR(const CALCINFO* pCalcInfo); //-------B
__declspec(dllexport) int WINAPI MYZHOUQI(const CALCINFO* pCalcInfo);//-------C
#ifdef __cplusplus
}
#endif //__cplusplus

#endif //__FXJFUNC_H_INCLUDE
