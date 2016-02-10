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
#pragma pack (1) /*指定按1字节对齐*/
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

typedef struct tagCALCINFO
{
	const DWORD					m_dwSize;				//结构大小
	const DWORD					m_dwVersion;			//调用软件版本(V2.10 : 0x210)
	const DWORD					m_dwSerial;				//调用软件序列号
	const char *const			m_strStkLabel;			//股票代码
	const BOOL					m_bIndex;				//指数

	const int					m_nNumData;				//数据数量(pCalcInfo,pStkDataEx,
																//			    pResultBuf数据数量)
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




typedef struct tagCALCINFO1
{
	 DWORD					m_dwSize;				//结构大小
	 DWORD					m_dwVersion;			//调用软件版本(V2.10 : 0x210)
	 DWORD					m_dwSerial;				//调用软件序列号
	 char *			m_strStkLabel;			//股票代码
	 BOOL					m_bIndex;				//指数
	
	 int					m_nNumData;				//数据数量(pCalcInfo,pStkDataEx,
	//			    pResultBuf数据数量)
	 STKDATA *		m_pStkData;				//常规数据,注意:当m_nNumData==0
	//时可能为 NULL
	 STKDATAEx *	m_pStkDataEx;			//扩展数据,分笔成交买卖盘,
	//注意:可能为 NULL
	
	 int					m_nParam1Start;		//参数1有效位置
	 float *		m_pfParam1;				//调用参数1	
	 float *		m_pfParam2;				//调用参数2
	 float *		m_pfParam3;				//调用参数3
	 float *		m_pfParam4;				//调用参数4
	
	float *				m_pResultBuf;			//结果缓冲区
	 PERIOD_TYPE			m_ePeriodType;			//数据类型
	 float* 		m_pfFinData;			//财务数据
} CALCINFO1;





/*自己定义的宏和结构*/
#define NULL 0
#define INPUT 
#define OUTPUT

/*定义分型的标准序列个数*/
#define TIME 4

#define MAXMATRIX      20000 /*定义搜索的数组的最大值*/
#define MAXIMCAREBI  10


/*定义大周期和中周期，中周期和小周期之间的转换率一般认为相差5-6，比如1分钟和5分钟，5分钟和30分钟*/
#define ZHOUQIZHUANHUANLV    5


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
	float matrix_dingdi_fudubizhi[MAXIMCAREBI]; /*计算所有的顶底连线的幅度比值*/
	float matrix_dingdi_xielv[MAXIMCAREBI]; /*计算所有的顶底连线的斜率*/
	float matrix_dingdi_mianji[MAXIMCAREBI]; /*计算所有的顶底连线的面积*/

	/*
	最近几次笔对应的长度、幅度、斜率、面积，为正则表示处于上升笔的属性，为负则表示下降笔的属性
	需要说明的是比较的重点是确立阻力最小的方向，因此没有必要专门画出中枢来
	*/

	unsigned int current_stat;/*当前状态分为：上涨未背驰，上涨背驰、下跌未背驰、下跌背驰、盘整上涨未背驰、盘整下跌未背驰
	、盘整上涨背驰、盘整下跌背驰*/

	unsigned int beichilidu;/*宏观定义当前节点组成的笔动态的上涨或者下跌的背驰力度，属于综合判断分为
	上涨背驰、上涨未背驰、下跌背驰、下跌未背驰、上涨轻微背驰、下跌轻微背驰、无法判断，背驰还有一个
	成交量的背驰，需要重点考察*/

	unsigned int dianweiyuce;/*预测当前这一笔最可能的结束位置，预测是最没有用最不可靠的东西，但是如果确实这一笔到达该位置附近出现了
	背驰，或者下一个周期在该位置附近出现了卖点，需要重点对待。如果没有按照预测的走，则不用管他
	 */

	unsigned int buy_sell_point;/* 
	定义当前属于哪三类买卖点区间，第一买点、第二买点、第三买点，第一卖点
	、第二买点、第三卖点，持股区间、持币区间，第一类买卖点
	这个是最重要的结论型参数、可据此买卖股票；当然进入买卖点区间后确定更具体的买卖点可以通过
	macd和kdj金叉来辅助判断之，建议多买第二买点，第一买点是作为加底仓用的，安全的第二买点可以加到大仓位
	出现第三买点表明第一第二买点的安全性，可以满仓或者满仓呆涨了。卖点则相反。
	具体的买卖点则最好由小周期的二三买来确定，在小周期中还可以通过买卖点做小仓位的差价；最好有三个
	周期级别，最高周期级别宏观战略上确定可能的高低点，背驰程度等等。如宏观上看出现了下跌且尚未背驰
	那无论如何不能进去，如出现了上涨背驰，则需要密切注意下一个周期级别的卖点了。
	中间的周期是用来确定买卖的主要依据，最小周期则是用来精确确定买入和卖出点位的，通常还可以根据小级别的
	买卖点做一些短差
	*/
	/************************************************************************/
	float zhisundianwei;/*某种意义上说第一买点是不稳定的，也就是说会被突发事件等外在因素所破坏掉，这个时候要触发
		止损，当然理论上这个时候会出现卖点的，但如果是小转大的情况可能来不及卖掉，所以需要一个止损点
		，这个点位可以由三卖确定之，其实在前期有盈利的情况下还有必要设立一个止盈点来动态应付可能出现的风险。
		由于买点是依次出现的，这也保证了在盈利的基础上加码
	 */


} STPrivateData;

#pragma pack () /*默认字节对齐*/
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
__declspec(dllexport) int WINAPI MYTEST(const CALCINFO* pCalcInfo);
__declspec(dllexport) int WINAPI MYTESTI(const CALCINFO* pCalcInfo);
__declspec(dllexport) int WINAPI MYTESTII(const CALCINFO* pCalcInfo);

#ifdef __cplusplus
}
#endif //__cplusplus

#endif //__FXJFUNC_H_INCLUDE





















