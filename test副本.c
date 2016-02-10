//+------------------------------------------------------------------+
//|                           Designed by xuejia, China               |
//|                   Copyright 2015, xuejia mql4                    |
//|                                 http://blog.sina.com.cn/FXTrade | 
//+------------------------------------------------------------------+ 
#property copyright "Copyright 2015 , HangZhou, China." 
#property link "http://blog.sina.com.cn/FXTrade ,  http://www.mql4.com/users/DxdCn" 
#property version   "0.01"

//定义本EA操作的订单的唯一标识号码
//预定义的常量 Magic用于后面程序里的开仓动作时标记在订单里面的一个符号，用于区别其他EA程序下的单 防止混乱
#define MAGICMA 201508011234
//+------------------------------------------------------------------+
//| 注意没有指标文件那些property                   |
//+------------------------------------------------------------------+

extern int _MagicNumber = MAGICMA;

extern int whichmethod = 1;   //1~4 种下单方式  1 仅开仓, 2 有止损无止赢, 3 有止赢无止损, 4 有止赢也有止损

extern double TakeProfit = 100;   //止赢点数
extern   double StopLoss = 20;    //止损点数

extern int Luft = 20; // 挂单交易放置水平的间距，保证滑点情况下成交

extern double MaximumRisk     = 0.3; //资金控制,控制下单量;-1~1之间数值越大，风险越小，负数卖空，正数买多。

extern double TrailingStop =25;     //跟踪止赢点数设置
extern   int maxOpen = 3;   //最多开仓次数限制 
extern   int maxLots = 5;   //最多单仓持仓量限制 

extern int bb = 0;       //非零就允许跟踪止赢


extern double MATrendPeriod=26;//使用26均线 开仓条件参数  本例子
 

extern double Lots = 0.1;//每单的交易量




//定义资金管理--------------------------------------------------------+
extern string _ZiJinGuanLi_ ="任选一个";
extern bool zijiguanli1= true;//false true、、
extern bool zijiguanli2= false;//false true、、
extern bool zijiguanli3= false;//false true、、

//定义
//返回分析出来的当前做多和做空的风险系数，挂单时要充分考虑该风险系数；通过MA计算得到。
// 分别对5分钟、30分钟、4小时线做分析
double CalculateRisk(void)
{

    //获取移动平均线
    ma=iMA(NULL,0,5,0,MODE_SMA,PRICE_CLOSE,0);

    //获取布林线上轨
    boll_up = iBands(NULL,0,20,1,0,PRICE_CLOSE,MODE_UPER);
    boll_low = iBands(NULL,0,20,1,0,PRICE_CLOSE,MODE_LOWER)

    double strong_weak = (ma-boll_low)/(bool_up-bool_low);

}

//-2为空；-1为弱空；2为多；1为弱多
int g_Strong_Weak_flag =0;

//1表示刚上穿，2表示上穿一定的时间周期，-1表示刚下穿，-2表示下穿一定的时间周期
int g_cross_flag =0;

double CalculateStrong_weak(void)
{

    double strong_weak[50];

    //1表示刚上穿，2表示上穿一定的时间周期，-1表示刚下穿，-2表示下穿一定的时间周期
    int cross_flag =0;
    bars=iBars(NULL,5);

    if (50<=bars) {
        bars= 50;
    }
    else{
        return 0;
    }

    for (i=0;i<bars;i++) {
        //获取移动平均线
        ma=iMA(NULL,0,5,0,MODE_SMA,PRICE_CLOSE,i);

        //获取布林线上轨
        boll_up = iBands(NULL,5,20,1,0,PRICE_CLOSE,MODE_UPER,i);
        boll_low = iBands(NULL,5,20,1,0,PRICE_CLOSE,MODE_LOWER,i);
        strong_weak[i] = (ma-boll_low)/(bool_up-bool_low);

    }
    

    ////////////////////////////////////////////////////////////////// 
    
    if (0 > strong_weak[0]) 
    {
        g_Strong_Weak_flag = -2;

    }
    else if ((0 <= strong_weak[0])&&(0.5 > strong_weak[0])) {

        g_Strong_Weak_flag = -1;

    }
    else if ((0.5 <= strong_weak[0])&&(1 >= strong_weak[0])) {

        g_Strong_Weak_flag = 1;

    }
    else
    {
        g_Strong_Weak_flag = 2;
    }
     
    //////////////////////////////////////////////////////////////////
        
        
        
        
        
        for (i=1;i<bars;i++) {
        //获取移动平均线
            if（0 > strong_weak[0]）
            {
        
                if ((0<=strong_weak[i])&&(1==i)) {
                    cross_flag = -1;
                    break;
                }
                if ((0<=strong_weak[i])&&(1 < i)) {
                    cross_flag = -2;
                    break;
                }            
        
            }
            else if ((0 <= strong_weak[0])&&(0.5 > strong_weak[0])) {
        
                if (1==i) {
        
        
                    if (0>strong_weak[i]) {
                        cross_flag = 1;
                        break;
                    }
                    if (0.5<= strong_weak[i]) {
                        cross_flag = -1;
                        break;
                    } 
        
                }
                else
                {
        
                    if (0>strong_weak[i]) {
                        cross_flag = 2;
                        break;
                    }
                    if (0.5<= strong_weak[i]) {
                        cross_flag = -2;
                        break;
                    } 
                }
            }
            else if ((0.5 <= strong_weak[0])&&(1 >= strong_weak[0])) {
        
        
                if (1==i) {
        
        
                    if (0.5>strong_weak[i]) {
                        cross_flag = 1;
                        break;
                    }
                    if (1< strong_weak[i]) {
                        cross_flag = -1;
                        break;
                    } 
        
                }
                else
                {
        
                    if (0.5>strong_weak[i]) {
                        cross_flag = 2;
                        break;
                    }
                    if (01< strong_weak[i]) {
                        cross_flag = -2;
                        break;
                    } 
                }
            }
            else{
                if ((1>=strong_weak[i])&&(1==i)) {
                    cross_flag = 1;
                    break;
                }
                if ((1>=strong_weak[i])&&(1 < i)) {
                    cross_flag = -2;
                    break;
                } 
            }
        
        }
    }


}

//return值： 
//0表示状态未变或者没有状态
//返回值为 2 表示当前点可以开空单,先平调所有单
//返回值为 1表示当前点可以开多单，先平调所有单
//返回值为 -1表示平调此前开得多单
//返回值为-2表示平调此前开得空单
//返回值为0表示不做任何操作
//返回值为-5表示大周期可能起变化，强制平调所有单

//总结逻辑就是 返回 非0时平调所有单；然后看如果 返回2开空单，返回1开多单。

int CalculateBuy_Sell_Flag(void)
{
    int ret=0;
    //Level3为空或者弱空，且L2为空，L1由弱空进入空的瞬间开空单

    if ((g_Strong_Weak_flag_Three < 0)&&(g_Strong_Weak_flag_Two == -2)
        &&(g_Strong_Weak_flag_One == -2)&&(g_cross_flag_One == -1)) {
        ret = 2;
    }
    //Level3为多或者弱多，且L2为多，L1由弱多进入多的瞬间开多单

    else if ((g_Strong_Weak_flag_Three > 0)&&(g_Strong_Weak_flag_Two == 2)
        &&(g_Strong_Weak_flag_One < -1)&&(g_cross_flag_One == 1)) {
        ret = 1;
    }
    //大趋势为空得情况下，小趋势可能会背驰再背驰，因此相对于开空单，放宽平空单的条件为，L1由弱空进入若多的瞬间
    //这是一个可考虑的条件，也可以采用对等条件：L1由空进入弱空的瞬间，平空单。
   // if ((g_Strong_Weak_flag_One == -1)&&(g_cross_flag_One == 1)) {
    else if ((g_Strong_Weak_flag_One == 1)&&(g_cross_flag_One == 1)) {
        ret = -2;
    }
    //大趋势为多得情况下，小趋势可能会背驰再背驰，因此相对于开多单，放宽平多单的条件为，L1由弱多进入若空的瞬间
    //这是一个可考虑的条件，也可以采用对等条件：L1由空进入弱多的瞬间，平多单。
   // if ((g_Strong_Weak_flag_One == 1)&&(g_cross_flag_One == -1)) {
    else if ((g_Strong_Weak_flag_One == -1)&&(g_cross_flag_One == -1)) {
        ret = -1;
    }
   //L3弱空翻弱多或者弱多翻弱空的瞬间确保平调所有的单
    else if ((g_Strong_Weak_flag_Three == 1)&&(g_cross_flag_Three == 1)) {
        ret = -5;
    }
    else if ((g_Strong_Weak_flag_Three == -1)&&(g_cross_flag_Three == -1)) {
        ret = -5;
    }
   //L2空翻弱空或者多翻若多的瞬间确保平调所有的单
    else if ((g_Strong_Weak_flag_Two == -1)&&(g_cross_flag_Two == 1)) {
        ret = -5;
    }
    else if ((g_Strong_Weak_flag_Two == 1)&&(g_cross_flag_Two == -1)) {
        ret = -5;
    }
    return ret;
}




//获取开单向单的订单仓位信息函数-----------------------------------------------------//

//如你所见，这非常简单: 一共 11 个变量，每个都储存仓位的相关信息
//(ticket #, type, lot size, 等等). 当函数开始运行时，这些变量被归零。作为全局变量这是必需的。
//函数被调用时变量也可以不归零，但我们需要的不是先前的信息，我们需要的是最近的。
//然后所有的仓位会以标准的方式被搜索，一旦获得需要的信号和MagicNumber 值，
//信息将被存储在相应的变量中
//---------------------------------------------------------------------------------//
int _Ticket = 0, _Type = 0; double _Lots = 0.0,

_OpenPrice = 0.0, _StopLoss = 0.0;
double _TakeProfit = 0.0; datetime _OpenTime = -1;

double _Profit = 0.0, _Swap = 0.0;
double _Commission = 0.0; string _Comment = \"\";

datetime _Expiration = -1;

void OneOrderInit( int magic )

{
    int _GetLastError, _OrdersTotal = OrdersTotal();

    _Ticket = 0; _Type = 0; _Lots = 0.0; _OpenPrice = 0.0;

    _StopLoss = 0.0;
    _TakeProfit = 0.0; _OpenTime = -1; _Profit = 0.0;

    _Swap = 0.0;
    _Commission = 0.0; _Comment = \"\"; _Expiration = -1;

    for ( int z = _OrdersTotal - 1; z >= 0; z -- )

    {
        if ( !OrderSelect( z, SELECT_BY_POS ) )

        {
            _GetLastError = GetLastError();
            Print(\"OrderSelect( \", z, \", SELECT_BY_POS ) -错误#\",
            _GetLastError );
            continue;
        }

        if(OrderMagicNumber() == magic && OrderSymbol() ==
        Symbol())

        {
            _Ticket = OrderTicket();
            _Type = OrderType();
            _Lots = NormalizeDouble( OrderLots(), 1 );
            _OpenPrice = NormalizeDouble( OrderOpenPrice(), Digits);
            _StopLoss = NormalizeDouble( OrderStopLoss(), Digits);
            _TakeProfit = NormalizeDouble( OrderTakeProfit(), Digits);
            _OpenTime = OrderOpenTime();
            _Profit = NormalizeDouble( OrderProfit(), 2 );
            _Swap = NormalizeDouble( OrderSwap(), 2 );
            _Commission = NormalizeDouble( OrderCommission(), 2 );
            _Comment = OrderComment();
            _Expiration = OrderExpiration();
            return;
        }

    }
}


//-------------------------------------------------------------------------------//
//获取开双向单的订单仓位信息函数-----------------------------------------------------//


// 在定单特性中的整体变量会被储存:
int _BuyTicket = 0, _SellTicket = 0, _BuyStopTicket = 0;
int _SellStopTicket = 0, _BuyLimitTicket = 0, _SellLimitTicket = 0;
double _BuyLots = 0.0, _SellLots = 0.0, _BuyStopLots = 0.0; 
double _SellStopLots = 0.0, _BuyLimitLots = 0.0, 
_SellLimitLots = 0.0;
double _BuyOpenPrice = 0.0, _SellOpenPrice = 0.0, 
_BuyStopOpenPrice = 0.0;
double _SellStopOpenPrice = 0.0, _BuyLimitOpenPrice = 0.0,
_SellLimitOpenPrice = 0.0;
double _BuyStopLoss = 0.0, _SellStopLoss = 0.0, _BuyStopStopLoss = 0.0;
double _SellStopStopLoss = 0.0, _BuyLimitStopLoss = 0.0, _SellLimitStopLoss = 0.0;
double _BuyTakeProfit = 0.0, _SellTakeProfit = 0.0,
_BuyStopTakeProfit = 0.0;
double _SellStopTakeProfit = 0.0, _BuyLimitTakeProfit = 0.0,
_SellLimitTakeProfit = 0.0;
datetime _BuyOpenTime = -1, _SellOpenTime = -1, 
_BuyStopOpenTime = -1;
datetime _SellStopOpenTime = -1, _BuyLimitOpenTime = -1,
_SellLimitOpenTime = -1;
double _BuyProfit = 0.0, _SellProfit = 0.0, _BuySwap = 0.0, _SellSwap = 0.0;
double _BuyCommission = 0.0, _SellCommission = 0.0;

string _BuyComment = "", _SellComment = "", _BuyStopComment = ""; 
string _SellStopComment = "", _BuyLimitComment = "", 
_SellLimitComment = "";

datetime _BuyStopExpiration = -1, _SellStopExpiration = -1;
datetime _BuyLimitExpiration = -1, _SellLimitExpiration = -1;

void OneTypeOrdersInit( int magic )
{
    // 变量归零:
    _BuyTicket = 0; _SellTicket = 0; _BuyStopTicket = 0;
    _SellStopTicket = 0; _BuyLimitTicket = 0; _SellLimitTicket = 0;

    _BuyLots = 0.0; _SellLots = 0.0; _BuyStopLots = 0.0;
    _SellStopLots = 0.0; _BuyLimitLots = 0.0; _SellLimitLots = 0.0;

    _BuyOpenPrice = 0.0; _SellOpenPrice = 0.0; _BuyStopOpenPrice = 0.0;
    _SellStopOpenPrice = 0.0; _BuyLimitOpenPrice = 0.0; 

    _SellLimitOpenPrice = 0.0;

    _BuyStopLoss = 0.0; _SellStopLoss = 0.0; _BuyStopStopLoss = 0.0;
    _SellStopStopLoss = 0.0; _BuyLimitStopLoss = 0.0; 

    _SellLimitStopLoss = 0.0;

    _BuyTakeProfit = 0.0; _SellTakeProfit = 0.0;
    _BuyStopTakeProfit = 0.0;
    _SellStopTakeProfit = 0.0; _BuyLimitTakeProfit = 0.0; 

    _SellLimitTakeProfit = 0.0;

    _BuyOpenTime = -1; _SellOpenTime = -1; _BuyStopOpenTime = -1;
    _SellStopOpenTime = -1; _BuyLimitOpenTime = -1; 

    _SellLimitOpenTime = -1;

    _BuyProfit = 0.0; _SellProfit = 0.0; _BuySwap = 0.0; 

    _SellSwap = 0.0;
    _BuyCommission = 0.0; _SellCommission = 0.0;

    _BuyComment = ""; _SellComment = ""; _BuyStopComment = "";
    _SellStopComment = ""; _BuyLimitComment = ""; 

    _SellLimitComment = "";

    _BuyStopExpiration = -1; _SellStopExpiration = -1;
    _BuyLimitExpiration = -1; _SellLimitExpiration = -1;

    int _GetLastError = 0, _OrdersTotal = OrdersTotal();
    for ( int z = _OrdersTotal - 1; z >= 0; z -- )
    {
        if ( !OrderSelect( z, SELECT_BY_POS ) )

        {
        _GetLastError = GetLastError();
        Print("OrderSelect(", z, ",SELECT_BY_POS) - Error #", 

        _GetLastError );
        continue;
        }
        if ( OrderMagicNumber() == magic && OrderSymbol() == 

        Symbol() )
        {
            switch ( OrderType() )

            {
                case OP_BUY:
                    _BuyTicket = OrderTicket();
                    _BuyLots = NormalizeDouble( OrderLots(), 1 );
                    _BuyOpenPrice = NormalizeDouble( OrderOpenPrice(),
                    Digits );
                    _BuyStopLoss = NormalizeDouble( OrderStopLoss(),
                    Digits );
                    _BuyTakeProfit = NormalizeDouble( OrderTakeProfit(),
                    Digits );
                    _BuyOpenTime = OrderOpenTime();
                    _BuyProfit = NormalizeDouble( OrderProfit(), 2 );
                    _BuySwap = NormalizeDouble( OrderSwap(), 2 );
                    _BuyCommission = NormalizeDouble( OrderCommission(),
                    2 );
                    _BuyComment = OrderComment();
                    break;
                case OP_SELL:
                    _SellTicket = OrderTicket();
                    _SellLots = NormalizeDouble( OrderLots(), 1 );
                    _SellOpenPrice = NormalizeDouble( OrderOpenPrice(),
                    Digits );
                    _SellStopLoss = NormalizeDouble( OrderStopLoss(),
                    Digits );
                    _SellTakeProfit = NormalizeDouble( OrderTakeProfit(),
                    Digits );
                    _SellOpenTime = OrderOpenTime();
                    _SellProfit = NormalizeDouble( OrderProfit(), 2 );
                    _SellSwap = NormalizeDouble( OrderSwap(), 2 );
                    _SellCommission = NormalizeDouble( OrderCommission(),
                    2 );
                    _SellComment = OrderComment();
                    break;
                case OP_BUYSTOP:
                    _BuyStopTicket = OrderTicket();
                    _BuyStopLots = NormalizeDouble( OrderLots(), 1 );
                    _BuyStopOpenPrice = NormalizeDouble( OrderOpenPrice(),
                    Digits );
                    _BuyStopStopLoss = NormalizeDouble( OrderStopLoss(),
                    Digits );
                    _BuyStopTakeProfit = NormalizeDouble( OrderTakeProfit(),
                    Digits );
                    _BuyStopOpenTime = OrderOpenTime();
                    _BuyStopComment = OrderComment();
                    _BuyStopExpiration = OrderExpiration();
                    break;
                case OP_SELLSTOP:
                    _SellStopTicket = OrderTicket();
                    _SellStopLots = NormalizeDouble( OrderLots(), 1 );
                    _SellStopOpenPrice = NormalizeDouble( OrderOpenPrice(),
                    Digits );
                    _SellStopStopLoss = NormalizeDouble( OrderStopLoss(),
                    Digits );
                    _SellStopTakeProfit = NormalizeDouble( OrderTakeProfit(),
                    Digits );
                    _SellStopOpenTime = OrderOpenTime();
                    _SellStopComment = OrderComment();
                    _SellStopExpiration = OrderExpiration();
                    break;
                case OP_BUYLIMIT:
                    _BuyLimitTicket = OrderTicket();
                    _BuyLimitLots = NormalizeDouble( OrderLots(), 1 );
                    _BuyLimitOpenPrice = NormalizeDouble( OrderOpenPrice(),
                    Digits );
                    _BuyLimitStopLoss = NormalizeDouble( OrderStopLoss(),
                    Digits );
                    _BuyLimitTakeProfit = NormalizeDouble( OrderTakeProfit(),
                    Digits );
                    _BuyLimitOpenTime = OrderOpenTime();
                    _BuyLimitComment = OrderComment();
                    _BuyLimitExpiration = OrderExpiration();
                    break;
                case OP_SELLLIMIT:
                    _SellLimitTicket = OrderTicket();
                    _SellLimitLots = NormalizeDouble( OrderLots(), 1 );
                    _SellLimitOpenPrice = NormalizeDouble( OrderOpenPrice(),
                    Digits );
                    _SellLimitStopLoss = NormalizeDouble( OrderStopLoss(),
                    Digits );
                    _SellLimitTakeProfit = NormalizeDouble( OrderTakeProfit(),
                    Digits );
                    _SellLimitOpenTime = OrderOpenTime();
                    _SellLimitComment = OrderComment();
                    _SellLimitExpiration = OrderExpiration();
                    break;
            }

        }
    }
}

//-----------------------------------------------------------------------//

int i, p2, xxx,p1, res;
double Lots; 
datetime lasttime;       //时间控制, 仅当一个时间周期完成才检查条件
int init()   //初始化
{ 
    Lots = 1;
    lasttime = NULL;

    //设置自动交易周期
    if (Period() != PERIOD_M15) {
        Print("Please select 15 min period.");
        return (0);
    }


return(0); 
}
int deinit() { return(0); } //反初始化
//主程序
int start()
{
CheckForOpen();    //开仓 平仓 条件检查 和操作
if (bb>0)   CTP();   //跟踪止赢
return(0);
}
//+------下面是各子程序--------------------------------------------+
double LotsOptimized()   //确定下单量，开仓调用 资金控制
{
double lot=Lots;
int   orders=HistoryTotal();   // history orders total
int   losses=0;             // number of losses orders without a break
//MarketInfo(Symbol(),MODE_MINLOT);     相关信息
//MarketInfo(Symbol(),MODE_MAXLOT);
//MarketInfo(Symbol(),MODE_LOTSTEP);
lot=NormalizeDouble(MaximumRisk * AccountBalance()/AccountLeverage(),1);     //开仓量计算
if(lot<0.1) lot=0.1;
if(lot>maxLots) lot=maxLots;
return(lot);
}
  
//平仓持有的买单
void CloseBuy()
{
if (OrdersTotal( ) > 0 )   
{
  for(i=OrdersTotal()-1;i>=0;i--)
  {
  if(OrderSelect(i,SELECT_BY_POS,MODE_TRADES)==false)     break;
  if(OrderType()==OP_BUY) 
  {
    OrderClose(OrderTicket(),OrderLots(),Bid,3,White);
    Sleep(5000); 
  }
  }
}
}
//平仓持有的卖单
void CloseSell()
{
if (OrdersTotal( ) > 0 )   
{
  for(i=OrdersTotal()-1;i>=0;i--)
  {
  if(OrderSelect(i,SELECT_BY_POS,MODE_TRADES)==false)     break;
  if(OrderType()==OP_SELL) 
    {
    OrderClose(OrderTicket(),OrderLots(),Ask,3,White);
    Sleep(5000); 
    }
  }
}
}
//判断是否买或卖或平仓
int buyorsell()   //在这个函数计算设置你的交易信号  这里使用MACD 和MA 做例子
{
  double MacdCurrent, MacdPrevious, SignalCurrent;
  double SignalPrevious, MaCurrent, MaPrevious;
  MacdCurrent=iMACD(NULL,0,12,26,9,PRICE_CLOSE,MODE_MAIN,0);
  MacdPrevious=iMACD(NULL,0,12,26,9,PRICE_CLOSE,MODE_MAIN,1);
  SignalCurrent=iMACD(NULL,0,12,26,9,PRICE_CLOSE,MODE_SIGNAL,0);
  SignalPrevious=iMACD(NULL,0,12,26,9,PRICE_CLOSE,MODE_SIGNAL,1);
  MaCurrent=iMA(NULL,0,MATrendPeriod,0,MODE_EMA,PRICE_CLOSE,0);
  MaPrevious=iMA(NULL,0,MATrendPeriod,0,MODE_EMA,PRICE_CLOSE,1);
if(MacdCurrent<0 && MacdCurrent>SignalCurrent && MacdPrevious<SignalPrevious 
    && MaCurrent>MaPrevious)
  return (1); // 买 Ma在上升，Macd在0线上，并且两线上交叉
if(MacdCurrent>0 && MacdCurrent<SignalCurrent && MacdPrevious>SignalPrevious 
    && MaCurrent<MaPrevious)
  return (-1); // 卖
return (0); //不交易
}
int nowbuyorsell = 0;
void CheckForOpen()
{
if (Time[0] == lasttime ) return; //每时间周期检查一次  时间控制
lasttime = Time[0];
nowbuyorsell = buyorsell(); //获取买卖信号
 
if (nowbuyorsell == 1) //买　先结束已卖的
  CloseSell();
if (nowbuyorsell == -1) //卖　先结束已买的
    CloseBuy(); if (TimeDayOfWeek(CurTime()) == 1)
  {
  if (TimeHour(CurTime()) < 3 ) return; //周一早8点前不做 具体决定于你的时区和服务器的时区  时间控制
  }
if (TimeDayOfWeek(CurTime()) == 5)
  {
  if (TimeHour(CurTime()) > 19 ) return; //周五晚11点后不做
  }
 
if (OrdersTotal( ) >= maxOpen) return ;   
//如果已持有开仓次数达到最大，不做
if (nowbuyorsell==0) return;   //不交易
TradeOK();   //去下单交易
}
void TradeOK()   //去下单交易
{
int error ;
if (nowbuyorsell == 1) //买 
  {
    switch (whichmethod)
    {
    case 1:   res=OrderSend(Symbol(),OP_BUY,LotsOptimized(),Ask,3,0,0,"",MAGICMA,0,Blue);break;
    case 2:   res=OrderSend(Symbol(),OP_BUY,LotsOptimized(),Ask,3,Ask-StopLoss*Point,0,"",MAGICMA,0,Blue); break;
    case 3:   res=OrderSend(Symbol(),OP_BUY,LotsOptimized(),Ask,3,0,Ask+TakeProfit*Point,"",MAGICMA,0,Blue);break;
    case 4:   res=OrderSend(Symbol(),OP_BUY,LotsOptimized(),Ask,3,Ask-StopLoss*Point,Ask+TakeProfit*Point,"",MAGICMA,0,Blue);break;
    default : res=OrderSend(Symbol(),OP_BUY,LotsOptimized(),Ask,3,0,0,"",MAGICMA,0,Blue);break;
    }
    if (res <=0)
    {
    error=GetLastError();
    if(error==134)Print("Received 134 Error after OrderSend() !! ");         // not enough money
    if(error==135) RefreshRates();   // prices have changed
    }
    Sleep(5000); 
    return ;   
  }
if (nowbuyorsell == -1) //卖
  {
    switch (whichmethod)
    {
    case 1:   res=OrderSend(Symbol(),OP_SELL,LotsOptimized(),Bid,3,0,0,"",MAGICMA,0,Red); break;
    case 2:   res=OrderSend(Symbol(),OP_SELL,LotsOptimized(),Bid,3,Bid+StopLoss*Point,0,"",MAGICMA,0,Red); break;
    case 3:   res=OrderSend(Symbol(),OP_SELL,LotsOptimized(),Bid,3,0,Bid-TakeProfit*Point,"",MAGICMA,0,Red); break;
    case 4:   res=OrderSend(Symbol(),OP_SELL,LotsOptimized(),Bid,3,Bid+StopLoss*Point,Bid-TakeProfit*Point,"",MAGICMA,0,Red); break;
    default : res=OrderSend(Symbol(),OP_SELL,LotsOptimized(),Bid,3,0,0,"",MAGICMA,0,Red); break;
    }
    if (res <=0)
    {
    error=GetLastError();
    if(error==134) Print("Received 134 Error after OrderSend() !! ");         // not enough money
    if(error==135) RefreshRates();   // prices have changed
    }
    Sleep(5000);
    return ;   
  }
}
void CTP()   //跟踪止赢
{
bool bs = false;
for (int i = 0; i < OrdersTotal(); i++)
{
  if(OrderSelect(i,SELECT_BY_POS,MODE_TRADES)==false)     break;
  if (OrderType() == OP_BUY) 
  {
    if ((Bid - OrderOpenPrice()) > (TrailingStop * MarketInfo(OrderSymbol(), MODE_POINT)))    //开仓价格 当前止损和当前价格比较判断是否要修改跟踪止赢设置
    {
    if (OrderStopLoss() < Bid - TrailingStop * MarketInfo(OrderSymbol(), MODE_POINT)) 
    {
      bs = OrderModify(OrderTicket(), OrderOpenPrice(), Bid - TrailingStop * MarketInfo(OrderSymbol(), MODE_POINT), OrderTakeProfit(),0, Green);
    }
    }
  } 
  else if (OrderType() == OP_SELL) 
  {
    if ((OrderOpenPrice() - Ask) > (TrailingStop * MarketInfo(OrderSymbol(), MODE_POINT)))  //开仓价格 当前止损和当前价格比较判断是否要修改跟踪止赢设置

    {
    if ((OrderStopLoss()) > (Ask + TrailingStop * MarketInfo(OrderSymbol(), MODE_POINT)))
    {     
      bs = OrderModify(OrderTicket(), OrderOpenPrice(),
        Ask + TrailingStop * MarketInfo(OrderSymbol(), MODE_POINT), OrderTakeProfit(),0, Tan);
}
    }
  }
}
}



//////////////////////////////////////////////////////////////////////////////
//自定义函数                   //
////////////////////////////////////////////////////////////////////////////// 


//订单净值转换点数
int iOrderEquitToPoint(int myTicket)
{
  int myPoint=0;
  if (OrderSelect(myTicket,SELECT_BY_TICKET,MODE_TRADES))
     {
        if (OrderType()==OP_BUY)
           {
              myPoint=(Bid-OrderOpenPrice())/Point;
           }
        if (OrderType()==OP_SELL)
           {
              myPoint=(OrderOpenPrice()-Ask)/Point;
           }
     }
  return(myPoint);
}

//金额转换手数
double iFundsToHands(string mySymbol,double myFunds)
{

    double myLots=myFunds/MarketInfo(mySymbol, MODE_MARGINREQUIRED);//换算可开仓手数
    myLots=MathRound(myLots/MarketInfo(mySymbol, MODE_MINLOT))*MarketInfo(Symbol(), MODE_MINLOT);//手数整形
    return(myLots);
}



//平掉所有仓位包括挂单
void CloseAllOrder()
{  
    int tick[200]; 
    PingFlag=0;
    int j=0;
    for(int i=0;i<OrdersTotal();i++)
    {
        OrderSelect(i, SELECT_BY_POS, MODE_TRADES);
        if(OrderSymbol()==Symbol() && OrderMagicNumber()==Magic)
        {
            j=j+1;
            tick[j]=OrderTicket();      
            Print("全部平仓！ :",tick[j]);    
        }
    }
    if (j!=0)
    {
        for(i=1;i<=j;i++)
        {
            OrderSelect(tick, SELECT_BY_TICKET);
            if(OrderType()==OP_BUY)
            {
                if(OrderClose(OrderTicket(),OrderLots(),Bid,滑点可接受范围)==false)
                {
                    PingFlag=1;
                    Print("多头平仓失败"+GetLastError());
                } 
            } 
          
         //OrderSelect(tick, SELECT_BY_TICKET);
         if(OrderType()==OP_BUYSTOP)
          {
           if(OrderDelete(OrderTicket(),CLR_NONE)==false)
           {
            PingFlag=1;
            Print("多头挂单撤销失败"+GetLastError());
           } 
          } 
         //OrderSelect(tick, SELECT_BY_TICKET);
         if(OrderType()==OP_BUYLIMIT)
          {
           if(OrderDelete(OrderTicket(),CLR_NONE)==false)
           {
            PingFlag=1;
            Print("多头挂单撤销失败"+GetLastError());
           } 
          } 
         if(OrderType()==OP_SELL)
          {
           if(OrderClose(OrderTicket(),OrderLots(),Ask,滑点可接受范围)==false)
           {
            PingFlag=1;
            Print("空头平仓失败"+GetLastError());
           } 
          } 
          
          if(OrderType()==OP_SELLSTOP)
          {
           if(OrderDelete(OrderTicket(),CLR_NONE)==false)
           {
            PingFlag=1;
            Print("空头挂单撤销失败"+GetLastError());
           } 
          }     
          if(OrderType()==OP_SELLLIMIT)
          {
           if(OrderDelete(OrderTicket(),CLR_NONE)==false)
           {
            PingFlag=1;
            Print("空头挂单撤销失败"+GetLastError());
           } 
          } 
        }
    }
  if (PingFlag==0) PingStop=1;
}


//批量清仓
void iBatchClose(int myType,double myLots,int myMagicNum)
{
  for (int cnt=0;cnt
     {
        if (OrderSelect(cnt,SELECT_BY_POS,MODE_TRADES) && OrderMagicNumber()==myMagicNum && OrderSymbol()==Symbol())
           {
              iWait(1000);
              if ((myType==OP_BUY || (myType==9 && OrderType()==OP_BUY)) && (myLots==0 || myLots>OrderLots()))
                 {
                    myLots=OrderLots();
                    OrderClose(OrderTicket(),myLots,Bid,0);
                 }
              if ((myType==OP_SELL || (myType==9 && OrderType()==OP_SELL)) && (myLots==0 || myLots>OrderLots()))
                 {
                    myLots=OrderLots();
                    OrderClose(OrderTicket(),myLots,Ask,0);
                 }
              if (myType==OP_BUYLIMIT || myType==OP_SELLLIMIT || myType==OP_BUYSTOP || myType==OP_SELLSTOP
                 || (myType==9 && (OrderType()==OP_BUYLIMIT || OrderType()==OP_SELLLIMIT || OrderType()==OP_BUYSTOP || OrderType()==OP_SELLSTOP)))
                 {
                    OrderDelete(OrderTicket());
                 }
           }
     }
  return(0);
}
 
//开仓量整形
double iLotsFormat(double myLots)
{
  myLots=MathRound(myLots/MarketInfo(Symbol(), MODE_MINLOT))*MarketInfo(Symbol(), MODE_MINLOT);//开仓量整形
  return(myLots);
}


//持仓单净值总计
double iGroupEquity(int myOrderType,int myMagicNum)
{
  double myReturn;
  if (OrdersTotal()==0) return(0);
  for (int cnt=0;cnt
     {
        if (OrderSelect(cnt,SELECT_BY_POS,MODE_TRADES)
            && OrderMagicNumber()==myMagicNum && OrderSymbol()==Symbol() && OrderType()==myOrderType)
           {
              myReturn=myReturn+OrderProfit();
           }
     }
  return(myReturn);
}

//持仓单开仓量总计
double iGroupLots(int myOrderType,int myMagicNum)
{
  double myReturn;
  if (OrdersTotal()==0) return(0);
  for (int cnt=0;cnt
     {
        if (OrderSelect(cnt,SELECT_BY_POS,MODE_TRADES)
            && OrderMagicNumber()==myMagicNum && OrderSymbol()==Symbol() && OrderType()==myOrderType)
           {
              myReturn=myReturn+OrderLots();
           }
     }
  return(myReturn);
}
 


//时间周期转换字符
string iTimeFrameToString(int myTimeFrame)
{
  switch(myTimeFrame)
     {
        case 1:
           return("M1");
           break;
        case 5:
           return("M5");
           break;
        case 15:
           return("M15");
           break;
        case 30:
           return("M30");
           break;
        case 60:
           return("H1");
           break;
        case 240:
           return("H4");
           break;
        case 1440:
           return("D1");
           break;
        case 10080:
           return("W1");
           break;
        case 43200:
           return("MN1");
           break;
     } 
}
 

//持仓单数量统计
int iOrderStatistics(int myOrderType,int myMagicNum)
{
  int myReturn;
  if (OrdersTotal()==0) return(0);
  for (int cnt=0;cnt
     {
        if (OrderSelect(cnt,SELECT_BY_POS,MODE_TRADES)
            && OrderMagicNumber()==myMagicNum && OrderSymbol()==Symbol() && OrderType()==myOrderType)
           {
              myReturn=myReturn+1;
           }
     }
  return(myReturn);
}
 

//计算特定条件的订单
int iOrdersSort(int myOrderType,int myOrderSort,int myMaxMin,int myMagicNum)
{
  int myReturn,myArrayRange,cnt,i,j;
  if (OrdersTotal()<=0) return(myReturn);
  myArrayRange=OrdersTotal();
  //持仓订单基本信息:0订单号,1开仓时间,2订单利润,3订单类型,4开仓量,5开仓价,
  //                 6止损价,7止赢价,8订单特征码,9订单佣金,10掉期,11挂单有效日期
  double myOrdersArray[][12]; //定义订单数组,第1维:订单序号;第2维:订单信息
  ArrayResize(myOrdersArray,myArrayRange); //重新界定订单数组
  ArrayInitialize(myOrdersArray, 0.0); //初始化订单数组
  //订单数组赋值
  for (cnt=0; cnt
     {
        //选中当前货币对相关持仓订单
        if (OrderSelect(cnt, SELECT_BY_POS, MODE_TRADES) && OrderSymbol()==Symbol() && OrderMagicNumber()==myMagicNum)
           {
              myOrdersArray[cnt][0]=OrderTicket();//0订单号
              myOrdersArray[cnt][1]=OrderOpenTime();//1开仓时间
              myOrdersArray[cnt][2]=OrderProfit();//2订单利润
              myOrdersArray[cnt][3]=OrderType();//3订单类型
              myOrdersArray[cnt][4]=OrderLots();//4开仓量
              myOrdersArray[cnt][5]=OrderOpenPrice();//5开仓价
              myOrdersArray[cnt][6]=OrderStopLoss();//6止损价
              myOrdersArray[cnt][7]=OrderTakeProfit();//7止赢价
              myOrdersArray[cnt][8]=OrderMagicNumber();//8订单特征码
              myOrdersArray[cnt][9]=OrderCommission();//9订单佣金
              myOrdersArray[cnt][10]=OrderSwap();//10掉期
              myOrdersArray[cnt][11]=OrderExpiration();//11挂单有效日期
           }
     }
  double myTempArray[12]; //定义临时数组
  //ArrayResize(myTempArray,myArrayRange); //重新界定临时数组
  ArrayInitialize(myTempArray, 0.0); //初始化临时数组
  //开始按条件排序
  //---- 按时间降序排列数组,原始数组重新排序
  if (myOrderSort==0)
     {
        for (i=0;i<=myArrayRange;i++)
           {
              for (j=myArrayRange;j>i;j--)
                 {
                    if (myOrdersArray[j][1]>myOrdersArray[j-1][1])
                       {
                          myTempArray[0]=myOrdersArray[j-1][0];
                          myTempArray[1]=myOrdersArray[j-1][1];
                          myTempArray[2]=myOrdersArray[j-1][2];
                          myTempArray[3]=myOrdersArray[j-1][3];
                          myTempArray[4]=myOrdersArray[j-1][4];
                          myTempArray[5]=myOrdersArray[j-1][5];
                          myTempArray[6]=myOrdersArray[j-1][6];
                          myTempArray[7]=myOrdersArray[j-1][7];
                          myTempArray[8]=myOrdersArray[j-1][8];
                          myTempArray[9]=myOrdersArray[j-1][9];
                          myTempArray[10]=myOrdersArray[j-1][10];
                          myTempArray[11]=myOrdersArray[j-1][11];
                          
                          myOrdersArray[j-1][0]=myOrdersArray[j][0];
                          myOrdersArray[j-1][1]=myOrdersArray[j][1];
                          myOrdersArray[j-1][2]=myOrdersArray[j][2];
                          myOrdersArray[j-1][3]=myOrdersArray[j][3];
                          myOrdersArray[j-1][4]=myOrdersArray[j][4];
                          myOrdersArray[j-1][5]=myOrdersArray[j][5];
                          myOrdersArray[j-1][6]=myOrdersArray[j][6];
                          myOrdersArray[j-1][7]=myOrdersArray[j][7];
                          myOrdersArray[j-1][8]=myOrdersArray[j][8];
                          myOrdersArray[j-1][9]=myOrdersArray[j][9];
                          myOrdersArray[j-1][10]=myOrdersArray[j][10];
                          myOrdersArray[j-1][11]=myOrdersArray[j][11];
                          
                          myOrdersArray[j][0]=myTempArray[0];
                          myOrdersArray[j][1]=myTempArray[1];
                          myOrdersArray[j][2]=myTempArray[2];
                          myOrdersArray[j][3]=myTempArray[3];
                          myOrdersArray[j][4]=myTempArray[4];
                          myOrdersArray[j][5]=myTempArray[5];
                          myOrdersArray[j][6]=myTempArray[6];
                          myOrdersArray[j][7]=myTempArray[7];
                          myOrdersArray[j][8]=myTempArray[8];
                          myOrdersArray[j][9]=myTempArray[9];
                          myOrdersArray[j][10]=myTempArray[10];
                          myOrdersArray[j][11]=myTempArray[11];
                       }
                  }
           }
        if (myMaxMin==0) //x,0,0
           {
              for (cnt=myArrayRange;cnt>=0;cnt--)
                 {
                    if (myOrdersArray[cnt][0]!=0 && myOrderType==NormalizeDouble(myOrdersArray[cnt][3],0)) 
                       {
                          myReturn=NormalizeDouble(myOrdersArray[cnt][0],0);
                          break;
                       }
                 }
           }
        if (myMaxMin==1) //x,0,1
           {
              for (cnt=0;cnt
                 {
                    if (myOrdersArray[cnt][0]!=0 && myOrderType==NormalizeDouble(myOrdersArray[cnt][3],0)) 
                       {
                          myReturn=NormalizeDouble(myOrdersArray[cnt][0],0);
                          break;
                       }
                 }
           }
     }
  //---- 按订单净值降序排列数组,原始数组重新排序
  if (myOrderSort==1 || myOrderSort==2)
     {
        ArrayInitialize(myTempArray, 0.0); //初始化临时数组
        
        for (i=0;i<=myArrayRange;i++)
           {
              for (j=myArrayRange-1;j>i;j--)
                 {
                    if (myOrdersArray[j][2]>myOrdersArray[j-1][2])
                       {
                          myTempArray[0]=myOrdersArray[j-1][0];
                          myTempArray[1]=myOrdersArray[j-1][1];
                          myTempArray[2]=myOrdersArray[j-1][2];
                          myTempArray[3]=myOrdersArray[j-1][3];
                          myTempArray[4]=myOrdersArray[j-1][4];
                          myTempArray[5]=myOrdersArray[j-1][5];
                          myTempArray[6]=myOrdersArray[j-1][6];
                          myTempArray[7]=myOrdersArray[j-1][7];
                          myTempArray[8]=myOrdersArray[j-1][8];
                          myTempArray[9]=myOrdersArray[j-1][9];
                          myTempArray[10]=myOrdersArray[j-1][10];
                          myTempArray[11]=myOrdersArray[j-1][11];
                          
                          myOrdersArray[j-1][0]=myOrdersArray[j][0];
                          myOrdersArray[j-1][1]=myOrdersArray[j][1];
                          myOrdersArray[j-1][2]=myOrdersArray[j][2];
                          myOrdersArray[j-1][3]=myOrdersArray[j][3];
                          myOrdersArray[j-1][4]=myOrdersArray[j][4];
                          myOrdersArray[j-1][5]=myOrdersArray[j][5];
                          myOrdersArray[j-1][6]=myOrdersArray[j][6];
                          myOrdersArray[j-1][7]=myOrdersArray[j][7];
                          myOrdersArray[j-1][8]=myOrdersArray[j][8];
                          myOrdersArray[j-1][9]=myOrdersArray[j][9];
                          myOrdersArray[j-1][10]=myOrdersArray[j][10];
                          myOrdersArray[j-1][11]=myOrdersArray[j][11];
                          
                          myOrdersArray[j][0]=myTempArray[0];
                          myOrdersArray[j][1]=myTempArray[1];
                          myOrdersArray[j][2]=myTempArray[2];
                          myOrdersArray[j][3]=myTempArray[3];
                          myOrdersArray[j][4]=myTempArray[4];
                          myOrdersArray[j][5]=myTempArray[5];
                          myOrdersArray[j][6]=myTempArray[6];
                          myOrdersArray[j][7]=myTempArray[7];
                          myOrdersArray[j][8]=myTempArray[8];
                          myOrdersArray[j][9]=myTempArray[9];
                          myOrdersArray[j][10]=myTempArray[10];
                          myOrdersArray[j][11]=myTempArray[11];
                       }
                  }
           }
        if (myOrderSort==1 && myMaxMin==1) //x,1,1
           {
              for (cnt=myArrayRange;cnt>=0;cnt--)
                 {
                    if (myOrdersArray[cnt][0]!=0 && myOrderType==NormalizeDouble(myOrdersArray[cnt][3],0) && myOrdersArray[cnt][2]>0) 
                       {
                          myReturn=NormalizeDouble(myOrdersArray[cnt][0],0);
                          break;
                       }
                 }
           }
        if (myOrderSort==1 && myMaxMin==0) //x,1,0
           {
              for (cnt=0;cnt
                 {
                    if (myOrdersArray[cnt][0]!=0 && myOrderType==NormalizeDouble(myOrdersArray[cnt][3],0) && myOrdersArray[cnt][2]>0) 
                       {
                          myReturn=NormalizeDouble(myOrdersArray[cnt][0],0);
                          break;
                       }
                 }
           }
        if (myOrderSort==2 && myMaxMin==0) //x,2,1
           {
              for (cnt=myArrayRange;cnt>=0;cnt--)
                 {
                    if (myOrdersArray[cnt][0]!=0 && myOrderType==NormalizeDouble(myOrdersArray[cnt][3],0) && myOrdersArray[cnt][2]<0) 
                       {
                          myReturn=NormalizeDouble(myOrdersArray[cnt][0],0);
                          break;
                       }
                 }
           }
        if (myOrderSort==2 && myMaxMin==1) //x,2,0
           {
              for (cnt=0;cnt
                 {
                    if (myOrdersArray[cnt][0]!=0 && myOrderType==NormalizeDouble(myOrdersArray[cnt][3],0) && myOrdersArray[cnt][2]<0) 
                       {
                          myReturn=NormalizeDouble(myOrdersArray[cnt][0],0);
                          break;
                       }
                 }
           }
     }
  //---- 按开仓价降序排列数组,原始数组重新排序
  if (myOrderSort==3)
     {
        for (i=0;i<=myArrayRange;i++)
           {
              for (j=myArrayRange;j>i;j--)
                 {
                    if (myOrdersArray[j][5]>myOrdersArray[j-1][5])
                       {
                          myTempArray[0]=myOrdersArray[j-1][0];
                          myTempArray[1]=myOrdersArray[j-1][1];
                          myTempArray[2]=myOrdersArray[j-1][2];
                          myTempArray[3]=myOrdersArray[j-1][3];
                          myTempArray[4]=myOrdersArray[j-1][4];
                          myTempArray[5]=myOrdersArray[j-1][5];
                          myTempArray[6]=myOrdersArray[j-1][6];
                          myTempArray[7]=myOrdersArray[j-1][7];
                          myTempArray[8]=myOrdersArray[j-1][8];
                          myTempArray[9]=myOrdersArray[j-1][9];
                          myTempArray[10]=myOrdersArray[j-1][10];
                          myTempArray[11]=myOrdersArray[j-1][11];
                          
                          myOrdersArray[j-1][0]=myOrdersArray[j][0];
                          myOrdersArray[j-1][1]=myOrdersArray[j][1];
                          myOrdersArray[j-1][2]=myOrdersArray[j][2];
                          myOrdersArray[j-1][3]=myOrdersArray[j][3];
                          myOrdersArray[j-1][4]=myOrdersArray[j][4];
                          myOrdersArray[j-1][5]=myOrdersArray[j][5];
                          myOrdersArray[j-1][6]=myOrdersArray[j][6];
                          myOrdersArray[j-1][7]=myOrdersArray[j][7];
                          myOrdersArray[j-1][8]=myOrdersArray[j][8];
                          myOrdersArray[j-1][9]=myOrdersArray[j][9];
                          myOrdersArray[j-1][10]=myOrdersArray[j][10];
                          myOrdersArray[j-1][11]=myOrdersArray[j][11];
                          
                          myOrdersArray[j][0]=myTempArray[0];
                          myOrdersArray[j][1]=myTempArray[1];
                          myOrdersArray[j][2]=myTempArray[2];
                          myOrdersArray[j][3]=myTempArray[3];
                          myOrdersArray[j][4]=myTempArray[4];
                          myOrdersArray[j][5]=myTempArray[5];
                          myOrdersArray[j][6]=myTempArray[6];
                          myOrdersArray[j][7]=myTempArray[7];
                          myOrdersArray[j][8]=myTempArray[8];
                          myOrdersArray[j][9]=myTempArray[9];
                          myOrdersArray[j][10]=myTempArray[10];
                          myOrdersArray[j][11]=myTempArray[11];
                       }
                  }
           }
        if (myMaxMin==1) //x,3,0
           {
              for (cnt=myArrayRange;cnt>=0;cnt--)
                 {
                    if (myOrdersArray[cnt][0]!=0 && myOrderType==NormalizeDouble(myOrdersArray[cnt][3],0) && myOrdersArray[cnt][5]>0) 
                       {
                          myReturn=NormalizeDouble(myOrdersArray[cnt][0],0);
                          break;
                       }
                 }
           }
        if (myMaxMin==0) //x,3,1
           {
              for (cnt=0;cnt
                 {
                    if (myOrdersArray[cnt][0]!=0 && myOrderType==NormalizeDouble(myOrdersArray[cnt][3],0) && myOrdersArray[cnt][5]>0) 
                       {
                          myReturn=NormalizeDouble(myOrdersArray[cnt][0],0);
                          break;
                       }
                 }
           }
     }
  return(myReturn);
}

//有效时间段
bool iValidTime(string myStartTime,string myEndTime,bool myServerTime)
{
  bool myValue=false;
  int myST,myET;
  if (myServerTime==true)
     {
        myST=StrToTime(Year()+"."+Month()+"."+Day()+" "+myStartTime);
        myET=StrToTime(Year()+"."+Month()+"."+Day()+" "+myEndTime);
        
     }
  if (myServerTime==false)
     {
        myST=StrToTime(myStartTime);
        myET=StrToTime(myEndTime);
        
     }
  if (myST>myET) myET=myET+1440*60;
  if (TimeCurrent()>myST && TimeCurrent()
     {
        myValue=true;
     }
  if (TimeLocal()>myST && TimeLocal()
     {
        myValue=true;
     }
  if (myST==myET) myValue=true;
  return(myValue);
}

//交易繁忙，程序等待，更新缓存数据 
void iWait(int myDelayTime) 
{
  while (!IsTradeAllowed() || IsTradeContextBusy()) Sleep(myDelayTime);
  RefreshRates();
  return(0);
}



//显示错误信息
string iGetErrorInfo(int myErrorNum)
   {
      string myLastErrorStr;
      if(myErrorNum>=0)
         {
            switch (myErrorNum)
               {
                  case 0:myLastErrorStr="错误代码:"+0+"-没有错误返回";break;
                  case 1:myLastErrorStr="错误代码:"+1+"-没有错误返回但结果不明";break;
                  case 2:myLastErrorStr="错误代码:"+2+"-一般错误";break;
                  case 3:myLastErrorStr="错误代码:"+3+"-无效交易参量";break;
                  case 4:myLastErrorStr="错误代码:"+4+"-交易服务器繁忙";break;
                  case 5:myLastErrorStr="错误代码:"+5+"-客户终端旧版本";break;
                  case 6:myLastErrorStr="错误代码:"+6+"-没有连接服务器";break;
                  case 7:myLastErrorStr="错误代码:"+7+"-没有权限";break;
                  case 8:myLastErrorStr="错误代码:"+8+"-请求过于频繁";break;
                  case 9:myLastErrorStr="错误代码:"+9+"-交易运行故障";break;
                  case 64:myLastErrorStr="错误代码:"+64+"-账户禁止";break;
                  case 65:myLastErrorStr="错误代码:"+65+"-无效账户";break;
                  case 128:myLastErrorStr="错误代码:"+128+"-交易超时";break;
                  case 129:myLastErrorStr="错误代码:"+129+"-无效价格";break;
                  case 130:myLastErrorStr="错误代码:"+130+"-无效停止";break;
                  case 131:myLastErrorStr="错误代码:"+131+"-无效交易量";break;
                  case 132:myLastErrorStr="错误代码:"+132+"-市场关闭";break;
                  case 133:myLastErrorStr="错误代码:"+133+"-交易被禁止";break;
                  case 134:myLastErrorStr="错误代码:"+134+"-资金不足";break;
                  case 135:myLastErrorStr="错误代码:"+135+"-价格改变";break;
                  case 136:myLastErrorStr="错误代码:"+136+"-开价";break;
                  case 137:myLastErrorStr="错误代码:"+137+"-经纪繁忙";break;
                  case 138:myLastErrorStr="错误代码:"+138+"-重新开价";break;
                  case 139:myLastErrorStr="错误代码:"+139+"-定单被锁定";break;
                  case 140:myLastErrorStr="错误代码:"+140+"-只允许看涨仓位";break;
                  case 141:myLastErrorStr="错误代码:"+141+"-过多请求";break;
                  case 145:myLastErrorStr="错误代码:"+145+"-因为过于接近市场，修改无效";break;
                  case 146:myLastErrorStr="错误代码:"+146+"-交易文本已满";break;
                  case 147:myLastErrorStr="错误代码:"+147+"-时间周期被经纪否定";break;
                  case 148:myLastErrorStr="错误代码:"+148+"-开单和挂单总数已被经纪限定";break;
                  case 149:myLastErrorStr="错误代码:"+149+"-当对冲被拒绝时,打开相对于现有的一个单量";break;
                  case 150:myLastErrorStr="错误代码:"+150+"-把为反FIFO规定的单子平掉";break;
                  case 4000:myLastErrorStr="错误代码:"+4000+"-没有错误";break;
                  case 4001:myLastErrorStr="错误代码:"+4001+"-错误函数指示";break;
                  case 4002:myLastErrorStr="错误代码:"+4002+"-数组索引超出范围";break;
                  case 4003:myLastErrorStr="错误代码:"+4003+"-对于调用堆栈储存器函数没有足够内存";break;
                  case 4004:myLastErrorStr="错误代码:"+4004+"-循环堆栈储存器溢出";break;
                  case 4005:myLastErrorStr="错误代码:"+4005+"-对于堆栈储存器参量没有内存";break;
                  case 4006:myLastErrorStr="错误代码:"+4006+"-对于字行参量没有足够内存";break;
                  case 4007:myLastErrorStr="错误代码:"+4007+"-对于字行没有足够内存";break;
                  case 4008:myLastErrorStr="错误代码:"+4008+"-没有初始字行";break;
                  case 4009:myLastErrorStr="错误代码:"+4009+"-在数组中没有初始字串符";break;
                  case 4010:myLastErrorStr="错误代码:"+4010+"-对于数组没有内存";break;
                  case 4011:myLastErrorStr="错误代码:"+4011+"-字行过长";break;
                  case 4012:myLastErrorStr="错误代码:"+4012+"-余数划分为零";break;
                  case 4013:myLastErrorStr="错误代码:"+4013+"-零划分";break;
                  case 4014:myLastErrorStr="错误代码:"+4014+"-不明命令";break;
                  case 4015:myLastErrorStr="错误代码:"+4015+"-错误转换(没有常规错误)";break;
                  case 4016:myLastErrorStr="错误代码:"+4016+"-没有初始数组";break;
                  case 4017:myLastErrorStr="错误代码:"+4017+"-禁止调用DLL ";break;
                  case 4018:myLastErrorStr="错误代码:"+4018+"-数据库不能下载";break;
                  case 4019:myLastErrorStr="错误代码:"+4019+"-不能调用函数";break;
                  case 4020:myLastErrorStr="错误代码:"+4020+"-禁止调用智能交易函数";break;
                  case 4021:myLastErrorStr="错误代码:"+4021+"-对于来自函数的字行没有足够内存";break;
                  case 4022:myLastErrorStr="错误代码:"+4022+"-系统繁忙 (没有常规错误)";break;
                  case 4050:myLastErrorStr="错误代码:"+4050+"-无效计数参量函数";break;
                  case 4051:myLastErrorStr="错误代码:"+4051+"-无效开仓量";break;
                  case 4052:myLastErrorStr="错误代码:"+4052+"-字行函数内部错误";break;
                  case 4053:myLastErrorStr="错误代码:"+4053+"-一些数组错误";break;
                  case 4054:myLastErrorStr="错误代码:"+4054+"-应用不正确数组";break;
                  case 4055:myLastErrorStr="错误代码:"+4055+"-自定义指标错误";break;
                  case 4056:myLastErrorStr="错误代码:"+4056+"-不协调数组";break;
                  case 4057:myLastErrorStr="错误代码:"+4057+"-整体变量过程错误";break;
                  case 4058:myLastErrorStr="错误代码:"+4058+"-整体变量未找到";break;
                  case 4059:myLastErrorStr="错误代码:"+4059+"-测试模式函数禁止";break;
                  case 4060:myLastErrorStr="错误代码:"+4060+"-没有确认函数";break;
                  case 4061:myLastErrorStr="错误代码:"+4061+"-发送邮件错误";break;
                  case 4062:myLastErrorStr="错误代码:"+4062+"-字行预计参量";break;
                  case 4063:myLastErrorStr="错误代码:"+4063+"-整数预计参量";break;
                  case 4064:myLastErrorStr="错误代码:"+4064+"-双预计参量";break;
                  case 4065:myLastErrorStr="错误代码:"+4065+"-数组作为预计参量";break;
                  case 4066:myLastErrorStr="错误代码:"+4066+"-刷新状态请求历史数据";break;
                  case 4067:myLastErrorStr="错误代码:"+4067+"-交易函数错误";break;
                  case 4099:myLastErrorStr="错误代码:"+4099+"-文件结束";break;
                  case 4100:myLastErrorStr="错误代码:"+4100+"-一些文件错误";break;
                  case 4101:myLastErrorStr="错误代码:"+4101+"-错误文件名称";break;
                  case 4102:myLastErrorStr="错误代码:"+4102+"-打开文件过多";break;
                  case 4103:myLastErrorStr="错误代码:"+4103+"-不能打开文件";break;
                  case 4104:myLastErrorStr="错误代码:"+4104+"-不协调文件";break;
                  case 4105:myLastErrorStr="错误代码:"+4105+"-没有选择定单";break;
                  case 4106:myLastErrorStr="错误代码:"+4106+"-不明货币对";break;
                  case 4107:myLastErrorStr="错误代码:"+4107+"-无效价格";break;
                  case 4108:myLastErrorStr="错误代码:"+4108+"-无效定单编码";break;
                  case 4109:myLastErrorStr="错误代码:"+4109+"-不允许交易";break;
                  case 4110:myLastErrorStr="错误代码:"+4110+"-不允许长期";break;
                  case 4111:myLastErrorStr="错误代码:"+4111+"-不允许短期";break;
                  case 4200:myLastErrorStr="错误代码:"+4200+"-定单已经存在";break;
                  case 4201:myLastErrorStr="错误代码:"+4201+"-不明定单属性";break;
                  case 4202:myLastErrorStr="错误代码:"+4202+"-定单不存在";break;
                  case 4203:myLastErrorStr="错误代码:"+4203+"-不明定单类型";break;
                  case 4204:myLastErrorStr="错误代码:"+4204+"-没有定单名称";break;
                  case 4205:myLastErrorStr="错误代码:"+4205+"-定单坐标错误";break;
                  case 4206:myLastErrorStr="错误代码:"+4206+"-没有指定子窗口";break;
                  case 4207:myLastErrorStr="错误代码:"+4207+"-定单一些函数错误";break;
                  case 4250:myLastErrorStr="错误代码:"+4250+"-错误设定发送通知到队列中";break;
                  case 4251:myLastErrorStr="错误代码:"+4251+"-无效参量- 空字符串传递到SendNotification()函数";break;
                  case 4252:myLastErrorStr="错误代码:"+4252+"-无效设置发送通知(未指定ID或未启用通知)";break;
                  case 4253:myLastErrorStr="错误代码:"+4253+"-通知发送过于频繁";break;
               }
         }
      return(myLastErrorStr);
   }

