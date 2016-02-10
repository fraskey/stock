variable: oldbi[51]=0,drawlen=0,drawflag=0;
variable: changdu[11]=0,fudu[11]=0,fudubizhi[11]=0,xielv[11]=0,mianji[11]=0;


/*资金管理：这是一个不被重视但是有极其重要的课题，好的资金管理能有效地规避风险，或者增加利润*/
/*资金管理包括合理的分配资金买股,通常情况下投入股市的资金量跟当时股市的风险成反比*/
/*具体包括三个方面:1.根据风险评估决定买入买入股票多少;2.买入股票后根据风险和承受能力决定止损和抛售股票,买入股票的资金要定义安全级别*/
/*3卖出股票要根据风险的大小决定抛售数量\定义预期值达到预期值可多抛售\定义止损值,达到止损值必须抛售*/
/*股市风险包括已经发生的危险和预测可能发生的风险程度:已经发生的风险包括:个股处于暴跌状态\个股下跌加速等等*/
/*可能发生的风险包括:近期高位放巨量\量价极度背离\大盘处于暴跌状态\政策环境恶化\货币政策收紧\基本面恶化\外围环境恶化\大级别处于下降通道小级别买股*/
/*在往下的一笔上买股,尚未被底分型确认阶段,存在底分型构筑失败的可能性:欲增加成功率只能尽可能多的分析各种非价格背驰,并分析这些背驰是否正反映到价格上来*/

/*当向下一笔买入股票,底分型尚未形成时,危险指数为1,此时通过止损价格来止损;当底分型形成以后危险指数为0,测试通过上升笔背驰或者定分型来卖股*/
/*在买点买入股票的价格*/
variable: mairujiage[5]=0;
/*在买点买入的股票数*/
variable: mairuliang[5]=0;
/*没有人能保证买入股票以后必然会上涨，因此必要的止损是必不可少的，关于止损点确定前面已经有所描述*/
variable: zhisunjiage[5]=0;
/*通过定义预期价格，在买入股票后根据背驰确定一个可以达到的点，到达该点以后即使未出现背驰也要抛掉一部分股票，因为目的已经达到，不能太贪*/
variable: yuqijiage[5]=1000;
/*通过危险指数定义当前买入的股票安全级别：目前定义两个级别，不安全0；安全1；*/
variable: weixianzhishu[5]=0;
/*定义买入时的背驰状况,便于在卖出点参考,决定卖出量*/
variable: mairuhuanjin[5]=0;

mlittle:=20/100;
mmiddle:=50/100;
mlarge:=80/100;
mfull:=100/100;


/*这里的幅度指的是比值,也就是增加或者减小值*/
/*振幅低于10%波动的股票不值得参与*/
FUDU_LEVEL1 := 10/100;
/*振幅高于30%波动的股票可认为处于上升或者下跌过程中,这个过程下跌建议找二买或者下降笔得到确认后再买,上升过程中出现回调且无大的背离可买入*/
FUDU_LEVEL2 := 30/100;


/*定义5个标准序列才能构成一笔*/
M:=5;


/*新周期了，要将旧笔数组的相对位移加一*/
FOR I=1 TO 50 DO BEGIN
	IF(oldbi[I] > 0) THEN BEGIN
		oldbi[I]:=oldbi[I]+1;
	END
	ELSE IF(oldbi[I] < 0) THEN BEGIN
		oldbi[I]:=oldbi[I]-1;
	END
	ELSE BEGIN
		;
	END
END;


orgin_flag:= 0;
di_flag:= 0;
ding_flag:=0;

/*确定最原始周期的第一个确认笔*/
IF(oldbi[1]=0 AND BARPOS>10) THEN BEGIN
	N1:=0;
	N2:=BARPOS;
	/*第一步：寻找N1到N2期间的最大值和最小值，顶分和底分型必以最大和最小值为顶底点*/
	min_count:=100000;
	max_count:=0;
	min_pos:=N1;
	max_pos:=N1;
	FOR I=N1 TO N2 DO BEGIN
		IF(REF(HIGH,I)>max_count) THEN BEGIN
			max_count:=REF(HIGH,I);
			max_pos:=I;
		END
	
		IF(REF(LOW,I)<min_count) THEN BEGIN
			min_count:=REF(LOW,I);
			min_pos:=I;
		END	
	
	END
	
	/*第二步：对于存在最大值点时，确认是否为顶分型的最高点，判断方法是看其左右两边是否分别形成了M1长度的标准下降序列*/
	ding_flg_ok:=0;
	IF((max_pos-N1)>=(M-1) AND (N2-max_pos)>=(M-1)) THEN BEGIN
		/*对左侧运算*/
		tem_high:=REF(HIGH,max_pos);
		tem_low:=REF(LOW,max_pos);
		fengxing_cnt:=0;
		FOR I=max_pos TO N2 DO BEGIN
			IF(REF(HIGH,I)<tem_high AND REF(LOW,I)<tem_low) THEN BEGIN
				fengxing_cnt:=fengxing_cnt+1;
			END
			tem_high:= MIN(tem_high,REF(HIGH,I));		
			tem_low:= MIN(tem_low,REF(LOW,I));
			/*序列已经形成，没有必要再找下去*/
			IF(fengxing_cnt>=M-1) THEN BEGIN		
				BREAK;
			END	
		END
		/*左侧序列形成的情况下，可对右侧进行计算*/		
		IF(fengxing_cnt>=M-1) THEN BEGIN	
			/*对右侧运算*/
			tem_high:=REF(HIGH,max_pos);
			tem_low:=REF(LOW,max_pos);
			fengxing_cnt:=0;
			
			FOR I=N1 TO max_pos DO BEGIN
				IF(REF(HIGH,I+(max_pos-N1))<tem_high AND REF(LOW,I+(max_pos-N1))<tem_low) THEN BEGIN
					fengxing_cnt:=fengxing_cnt+1;
				END
				tem_high:= MIN(tem_high,REF(HIGH,I+(max_pos-N1)));		
				tem_low:= MIN(tem_low,REF(LOW,I+(max_pos-N1)));	
			
				/*序列已经形成，没有必要再找下去*/
				IF(fengxing_cnt>=M-1) THEN BEGIN		
					BREAK;
				END		
			END
	
			/*确认该点为顶分型*/
			IF(fengxing_cnt>=M-1) THEN BEGIN		
				ding_flg_ok:=1;
			END	
		END
	
	END
	
	/*第三步：对于存在最小值点时，确认该点是否为底分型的最低点，判断方法是看其左右两边是否分别形成了M1长度的标准上升序列*/
	di_flg_ok:=0;
	IF((min_pos-N1)>=(M-1) AND (N2-min_pos)>=(M-1)) THEN BEGIN
		/*对左侧运算*/
		tem_high:=REF(HIGH,min_pos);
		tem_low:=REF(LOW,min_pos);
		fengxing_cnt:=0;
		FOR I=min_pos TO N2 DO BEGIN
			IF(REF(HIGH,I)>tem_high AND REF(LOW,I)>tem_low) THEN BEGIN
				fengxing_cnt:=fengxing_cnt+1;
			END
			tem_high:= MAX(tem_high,REF(HIGH,I));		
			tem_low:= MAX(tem_low,REF(LOW,I));
			/*序列已经形成，没有必要再找下去*/
			IF(fengxing_cnt>=M-1) THEN BEGIN		
				BREAK;
			END	
		END
		
		/*左侧序列形成的情况下，可对右侧进行计算*/		
		IF(fengxing_cnt>=M-1) THEN BEGIN	
			/*对右侧运算*/
			tem_high:=REF(HIGH,min_pos);
			tem_low:=REF(LOW,min_pos);
			fengxing_cnt:=0;
			
			FOR I=N1 TO min_pos DO BEGIN
				IF(REF(HIGH,I+(min_pos-N1))>tem_high AND REF(LOW,I+(min_pos-N1))>tem_low) THEN BEGIN
					fengxing_cnt:=fengxing_cnt+1;
				END
				tem_high:= MAX(tem_high,REF(HIGH,I+(min_pos-N1)));		
				tem_low:= MAX(tem_low,REF(LOW,I+(min_pos-N1)));	
				
				/*序列已经形成，没有必要再找下去*/
				IF(fengxing_cnt>=M-1) THEN BEGIN		
					BREAK;
				END		
			END
	
			/*确认该点为底分型*/
			IF(fengxing_cnt>=M-1) THEN BEGIN		
				di_flg_ok:=1;
			END	
		END
	END
	
	/*第四步：返回是否存在顶分型和底分型以及其值的大小，当顶分和底分型都存在时返回离当前周期最近的那个*/
	tem_flag:=0;
	IF(ding_flg_ok=1 AND di_flg_ok=0) THEN BEGIN
		tem_flag := max_pos;
	END
	IF(ding_flg_ok=0 AND di_flg_ok=1) THEN BEGIN
		tem_flag := min_pos * (-1);
	END
	
	IF(ding_flg_ok=1 AND di_flg_ok=1) THEN BEGIN
		
		IF(max_pos < min_pos) THEN BEGIN
			tem_flag := max_pos;
		END
		ELSE  BEGIN
			tem_flag := min_pos * (-1);
		END
	END
	orgin_flag:=tem_flag;
	
END


/*此前为确认顶分型，确认顶分型点到当前周期是否存在底分型确认点*/
IF(oldbi[1]>0) THEN BEGIN

	N1:=0;
	N2:=oldbi[1];
	
	/*第一步：寻找N1到N2期间的最小值，底分型必以最小值为底点*/
	min_count:=100000;
	min_pos:=N1;
	FOR I=N1 TO N2 DO BEGIN
		IF(REF(LOW,I)<min_count) THEN BEGIN
			min_count:=REF(LOW,I);
			min_pos:=I;
		END	
	END
	/*欲形成新的底分型，则最近的一次最高值一定为最高，否则不可能增加上升序列数；加上这个条件可大大减少计算量*/
	IF(LOW <= MAX(REF(LOW,1),REF(LOW,2))  OR HIGH <= MAX(REF(HIGH ,1),REF(HIGH ,2))) THEN BEGIN
		min_pos:=N1;
		;
	END

	/*第二步：对于存在最小值点时，确认是否为底分型的最低点，判断方法是只看其右两边是否分别形成了M1长度的标准上升序列*/
	di_flg_r_ok:=0;
	IF((min_pos-N1)>=(M-1)) THEN BEGIN
		/*对右侧运算*/
		tem_high:=REF(HIGH,min_pos);
		tem_low:=REF(LOW,min_pos);
		fengxing_cnt:=0;
		
		FOR I=N1 TO min_pos DO BEGIN
			IF(REF(HIGH,I+(min_pos-N1))>tem_high AND REF(LOW,I+(min_pos-N1))>tem_low) THEN BEGIN
				fengxing_cnt:=fengxing_cnt+1;
			END
			tem_high:= MAX(tem_high,REF(HIGH,I+(min_pos-N1)));		
			tem_low:= MAX(tem_low,REF(LOW,I+(min_pos-N1)));	
			
			/*序列已经形成，没有必要再找下去*/
			IF(fengxing_cnt>=M-1) THEN BEGIN		
				BREAK;
			END		
		END
		/*确认该点为底分型*/
		IF(fengxing_cnt>=M-1) THEN BEGIN		
			di_flg_r_ok:=1;
		END	
	END
	/*第三步：返回是否存在顶分型值的大小*/
	tem_flag:=0;
	IF(di_flg_r_ok=1) THEN BEGIN
		tem_flag := min_pos * (-1);
	END
	/*返回值flag为0时表示N1到N2周期间不存在底分型，-K表示底分型，K值为底分型到当前周期的距离绝对值；*/
	di_flag:=tem_flag;			
END


/*此前为确认底分型，确认底分型点到当前周期是否存在顶分型确认点*/
IF(oldbi[1]<0) THEN BEGIN
	/*ding_flag:= "DINGQR.flag"(0,ttt_var,5);*/
	N1:=0;
	N2:=ABS(oldbi[1]*(-1));
	/*第一步：寻找N1到N2期间的最大值，顶分型必以最大值为顶底点*/
	max_count:=0;
	max_pos:=N1;
	FOR I=N1 TO N2 DO BEGIN
		IF(REF(HIGH,I)>max_count) THEN BEGIN
			max_count:=REF(HIGH,I);
			max_pos:=I;
		END
	END
	/*欲形成新的顶分型，则最近的一次最低值一定为最低，否则不可能增加下降序列数；加上这个条件可大大减少计算量*/
	IF(LOW >= MIN(REF(LOW,1),REF(LOW,2))  OR HIGH >= MIN(REF(HIGH ,1),REF(HIGH ,2))) THEN BEGIN
		max_pos:=N1;
		;
	END
	/*第二步：对于存在最大值点时，确认是否为顶分型的最高点，判断方法是只看其右两边是否分别形成了M1长度的标准下降序列*/
	ding_flg_r_ok:=0;
	IF((max_pos-N1)>=(M-1)) THEN BEGIN
		/*对右侧运算*/
		tem_high:=REF(HIGH,max_pos);
		tem_low:=REF(LOW,max_pos);
		fengxing_cnt:=0;
		FOR I=N1 TO max_pos DO BEGIN
			IF(REF(HIGH,I+(max_pos-N1))<tem_high AND REF(LOW,I+(max_pos-N1))<tem_low) THEN BEGIN
				fengxing_cnt:=fengxing_cnt+1;
			END
			tem_high:= MIN(tem_high,REF(HIGH,I+(max_pos-N1)));		
			tem_low:= MIN(tem_low,REF(LOW,I+(max_pos-N1)));	
			/*序列已经形成，没有必要再找下去*/
			IF(fengxing_cnt>=M-1) THEN BEGIN		
				BREAK;
			END		
		END
		/*确认该点为顶分型*/
		IF(fengxing_cnt>=M-1) THEN BEGIN		
			ding_flg_r_ok:=1;
		END	
	END
	/*第三步：返回是否存在顶分型值的大小*/
	tem_flag:=0;
	IF(ding_flg_r_ok=1) THEN BEGIN
		tem_flag := max_pos;
	END
	
	/*返回值flag为0时表示N1到N2周期间不存在顶分型，+K表示顶分型，K值为顶分型到当前周期的距离；*/
	ding_flag:=tem_flag;	
END


/*当前周期为生成确认的顶底笔点的周期点，该变量后面会买卖股票会用到*/
bi_ding_queren:=0;
bi_di_queren:=0;
/*将确定的笔值赋值给数组oldbi[]*/
IF(orgin_flag<>0 AND oldbi[1]=0) THEN BEGIN
		oldbi[1] := orgin_flag;

END
IF(di_flag<0  AND oldbi[1]>0) THEN BEGIN
	FOR I=1 TO 49 DO BEGIN
		oldbi[51-I]:=oldbi[50-I];
	END
	oldbi[1]:=di_flag;
	bi_di_queren:=1;

END
IF(ding_flag>0 AND oldbi[1]<0 ) THEN BEGIN
	FOR I=1 TO 49 DO BEGIN
		oldbi[51-I]:=oldbi[50-I];
	END
	oldbi[1]:=ding_flag;		
	bi_ding_queren:=1;
END




/*上面的代码已经实现了寻找确定的笔，将其结果保存在oldbi[]数组中了，oldbi[1]表示最近一个确定的笔的顶点或底点；正数为顶点，负数为底点*/
/*下面的代码将对这些笔的其他相关属性进行计算，便于后面的笔间比较*/

/*计算出最近几次笔对应的长度、幅度、斜率、面积，只有长度恒为正；其他变量值为正则表示处于上升笔，为负则表示下降笔*/
IF(oldbi[9]<>0) THEN BEGIN

	FOR I=1 TO 8 DO BEGIN
		changdu[I+1]:= ABS(oldbi[I+1])-ABS(oldbi[I]);

		fudu[I+1]:=REF(CLOSE,ABS(oldbi[I]))- REF(CLOSE,ABS(oldbi[I+1]));
		/*大智慧处理slope + ref + MA 函数有问题，需要中间变量，不知何因，此斜率为相对斜率*/
		tem:=slope(close,abs(changdu[I+1]))/ma(close,abs(changdu[I+1]));
		tem1:= abs(oldbi[i]);
		xielv[I+1]:= 100*ref(tem,tem1);
		
		/*面积计算的方法很多，这里采用收盘价减去线段起点收盘价格的和再除以这段时间收盘均价的方式，除以均价是屏蔽绝对价格的高低对面积的影响*/

		tem1:= abs(oldbi[i]);
		tem2:= abs(oldbi[I+1]);
		msum:=0;
		msum1:=0;
		FOR J=tem1 to tem2 DO BEGIN
			msum:= msum + (ref(close,J)-ref(close,tem2));
			msum1:= msum1 + ref(close,J);
		END
		mianji[I+1]:=abs(changdu[I+1])*msum/msum1;
		fudubizhi[I+1]:=ABS(fudu[I+1]/MIN(REF(CLOSE,tem1),REF(CLOSE,tem2)));
	END

	changdu[1]:= ABS(oldbi[1]);
	fudu[1]:=CLOSE- REF(CLOSE,changdu[1]);
	xielv[1]:= slope(close,changdu[1])/MA(close,changdu[1]);
	mianji[1]:= abs(changdu[1])*(MA(close,abs(changdu[1]))-REF(close,abs(changdu[1])))/MA(close,abs(changdu[1]));	
	IF(oldbi[1]>0) THEN BEGIN	
		fudubizhi[1]:=ABS(fudu[1]/close);
	END
	ELSE BEGIN
		fudubizhi[1]:=ABS(fudu[1]/oldbi[1]);
	END
END
MYTEST:=fudubizhi[2];




/*买卖重要规则0：股票要低点买，高点卖；股票只是赌博的工具而已，工具！*/
/*买卖重要规则1：上升笔的确认点必须不能持股，即使此前持有股票（多为没有在上升笔形成阶段找到合适卖点），必须清仓！*/
/*买卖重要规则2：下降笔确认点可进一步确认此前下降笔中买入的股票是安全、成功的，这些股票必须在此后上升笔形成中出现背驰卖出！*/
/*买卖重要规则3：大盘暴跌情况下上升一笔出现背驰必须抛出所有股票！*/
/*买卖重要规则4：中幅大幅上升的笔上升中出现的天量的量价背离，必须抛出所有股票！*/
/*买卖重要规则5：止损位必须抛出此前买入的股票，因为股票的买入都是在下降一笔中买入的，没有人能保证背驰后必然会变成上升一笔，因此必须服从止损位！*/
								/*实际操作中可适用低级别的三卖来保证买卖的合理性，但这个在本级别上是看不到的！*/


buy_flag:=0;
/*理论上只有在上升的一笔得到确认后，且下降的一笔尚未得到确认的时候买股；为最低点买股，必然不会被套当然买股时还需要买足其他的背驰条件，这通常对应于一买或者是二买*/
/*特殊恶劣的情况下可以在下降一笔得到确认的初期买股，这通常对应着暴跌或者是长期阴跌或者是量价极度背驰后买股，也就是*/
/*下降一笔得到确认后的上升一笔，此时照理来说前期已经买入股票了，只要持股即可，如果还有货币可在上升一笔的适当回调中买入，在上升一笔的过程中*/
/*一定要密切关注量价极度背驰的情况，一旦出现量价极度背驰的情况，即使上升的笔尚未被下降的笔打断也要出一定量的股*/
/*同时在笔的上升过程中也可以到下一个级别去做一定仓位的短差，此时短差肯定不会被套的，这是由高一级别的笔的上升性质决定的*/
/*若前几笔的幅度都比较小或者斜率都非常小，那么说明该股的波动是比较小的，小波动的股票必然没有大的利差，且对于高位小幅波动的股票存在暴跌的风险*/
/*低位小幅长期震荡的股票则存在后期突然发力的可能性，对于这种情况，启用bool函数适时跟踪，当发现出现大幅波动后可介入。*/
IF(oldbi[9]<>0) THEN BEGIN

	/*当股价低于买入时设置的止损价格时必须抛出,当然止损价格的设定有多种方法,一种就是直接设置为买入价格的90%等,还有一种可设置为*/
	/*近期最低点95%,第二种方法的好处是确认底分型失败后即行推出,更加灵活*/
	/*当然在实际走势中是不需要设置止损的,可以通过低一级别的二买和三买来寻找买入点,通过该级别的三卖来保证卖出点,如果本级别的向上*/
	/*一笔构造成功,则表明该买入点的小级别转大级别成功,此处是不得已而为之*/
	/*但是当股票已经脱离危险区时，也就是说危险指数为1时，则不需要通过止损价格来卖了，可以通过笔卖之*/
	IF(CLOSE < zhisunjiage[1] AND weixianzhishu[1]=0) THEN BEGIN
		/*SELL(mairuliang[1]);*/
		zhisunjiage[1]:=0;
		mairuliang[1]:=0;
		mairujiage[1]:=0;
		yuqijiage[1]:=1000;
	END 
	
	/*理论上卖点是由背驰点以及笔终结点来确定,但是为了增加资金的安全性,可以在理论确保能到达的点先卖出一部分,摊低成本;从而保持一个好的心态*/
	IF(CLOSE > yuqijiage[1]) THEN BEGIN
		/*SELL(mairuliang[1]);*/
		zhisunjiage[1]:=0;
		mairuliang[1]:=0;
		mairujiage[1]:=0;		
		yuqijiage[1]:=1000;
		weixianzhishu[1]:=0;
	END 
	
	/*还有一个买点就是线段背驰卖点*/
	/*另外一个比较好的方法是看下一级别是否形成了三卖,如果形成了三卖,那么可以先抛一部分,接着观察后面是形成该级别的趋势还是更大级别的中枢扩展*/
	/*如果趋势,则全抛之,如果盘整,则通过盘整背驰在低点再买回来*/

	/*顶确认点，这个点要做的第一件唯一一件也是最重要一件事情就是不顾一切抛掉手中所有股票！*/
	IF(	bi_ding_queren=1) THEN BEGIN
		/*坚决抛掉手中所有股票！*/
		IF(mairuliang[1] > 0) THEN BEGIN
			/*SELL(mairuliang[1]);*/
			zhisunjiage[1]:=0;
			mairuliang[1]:=0;
			mairujiage[1]:=0;		
			yuqijiage[1]:=1000;
			weixianzhishu[1]:=0;
		END
		;
	END
	
	
	/*底点确认，这个点出现是确认前期下降一笔中买入股票是安全的、正确的；这也是买入股票以后所期待的第一件事情*/
	/*这个点还可以做另外一件事情，就是前面的笔是大幅下跌的一笔，该笔在下跌过程中是没有判断买点的，其出现的买点为第一类买点*/
	/*将该买点定义为下跌确认点主要是规避大幅下跌笔中大量的多头陷阱，当然需要说明的是这个并不是最好的买入方法*/
	/*最好的买入方法是在大幅下跌笔确认的时候在低级别上去找，类2、3买，保证买股是在下降一笔上，但这个事情在本级别上做不到，因此近似处理了*/
	ELSE IF(	bi_di_queren=1 ) THEN BEGIN
		IF(zhisunjiage[1]>0) THEN BEGIN
			weixianzhishu[1]:=1;
		END	
		IF(oldbi[1]<0) THEN BEGIN
			IF(fudubizhi[2]>FUDU_LEVEL2) THEN BEGIN	
				/*BUY(mlittle);*/
				buy_flag:=1;
				weixianzhishu[1]:=1;
				zhisunjiage[1]:=close*90/100;
				mairuliang[1]:=mlittle;
				mairujiage[1]:=close;		
				yuqijiage[1]:=1000;	
			END
		END
	
	END



	/*顶分型即上升一笔已经形成，正在形成下降一笔过程中；这个过程的中心思想是持币，并寻找机会买股，直到发现出现了某种程度的背驰为止可买股*/
	/*理论上寻机买股是在下降一笔中干的活，由于下降一笔并未结束，买入以后还是有可能出现更低的点，有两种方法规避：*/
	/*1、设置止损位，例如买入后如果发现跌了超过10%，那么赶紧抛掉，寻找下一个买点介入；这是机械命令，不能因为损失了就不愿止损，这是命令！这种做法的缺点是内涵不够丰富*/
	/*2、通过低级别的方式来观察买卖点，例如买入后低级别发现了三卖可抛出，因为低级别三卖后可能会出现本级别的下跌，进而使该级别买点构造失败，即行抛出*/		
	
	/*当前处于下降阶段，对于下降阶段有以下几个处理分类：1、强势下降，且下降程度非常大，这个时候需要跟前一次下降比较背驰力度*/
	/*如若坏背驰则必须要等到下降的一笔形成后再少量介入，此阶段通常为长期熊市*/
	/*2、下降不多，且下降程度比上一次上升线段少很多，此时大可以在出现下降背驰后买入，此多为牛市阶段回调*/
	/*3、下降的线段跟上次上升的线段相差无几，且有一定的降幅，此时多为平衡市的低点，可在MACD/KDJ低点介入，高点的目标价12可设置为MA30/60日，以及省次上升的高点*/
	/*4、上升和下降的线段的长度或者斜率都非常的小，这个时候可以认为震荡幅度太小，从而没有必要介入，这种情况下可通过bool方法来观测其异动，*/
	/*5、低位长期小幅盘整后很有可能出现大的上升，高位长期小幅盘整后很可能会暴跌*/	
	ELSE IF(oldbi[1]>0) THEN BEGIN
		/*目前尚处于小幅下降中,谨慎下跌中继和高位背驰下跌,属于缠中说禅小幅盘整状态，不能参与,还有为小幅震荡也不宜参与*/
		IF(fudubizhi[1]<FUDU_LEVEL1) THEN BEGIN			
		
			/*如果前三个笔的幅度都小于Level1,说明是小幅度的震荡,不适宜参与*/
			IF(fudubizhi[2]<FUDU_LEVEL1 AND fudubizhi[3]<FUDU_LEVEL1) THEN BEGIN
				/*盘整阶段，如果有股票的话坚决抛掉手中所有股票！因为盘整何时结束是没有人能知道的*/
				IF(fudubizhi[4]<FUDU_LEVEL1) THEN BEGIN
					IF(mairuliang[1] > 0) THEN BEGIN
						/*SELL(mairuliang[1]);*/
						zhisunjiage[1]:=0;
						mairuliang[1]:=0;
						mairujiage[1]:=0;		
						yuqijiage[1]:=1000;
						weixianzhishu[1]:=0;
					END
				END
				/*前期处于上涨阶段，现在回调了两次，如果处于比较理想的位置，还是可以介入的，毕竟存在上涨中继的可能性*/
				ELSE BEGIN
					;
				END

				;
			END					
			/*这种情况是前期中幅下跌后小幅回升后再小幅下跌并出现了背驰，中幅下跌说明此前跌幅并不是很大，如果各种形势没有明显转好的话，此处的背驰买入很可能是个多头陷阱，也就是说此乃下跌中继*/
			/*需要结合多种条件判断，比如下跌之前是否一直为高位小幅震荡即庄家出货，是否出现过极其明显的量价背离现象，下跌幅度是否很大，且后期时间上无明显背驰，总体来说该买点处于类2买位置，安全性比较高，需警惕下跌中继*/
			ELSE IF(fudubizhi[2]<FUDU_LEVEL1 AND (fudubizhi[3]>=FUDU_LEVEL1 AND fudubizhi[3]<=FUDU_LEVEL2)) THEN BEGIN
				/*分情况可以买入*/
				;
			END
			/*这种情况是前期大幅下跌后小幅回升后再小幅下跌并出现了背驰，中幅下跌说明此前跌势已经很大，理论上至少存在反弹需求，这种出现背驰点的买入一般情况下相对比较安全*/
			/*需要结合多种条件判断，比如下跌之前是否一直为高位小幅震荡即庄家出货，是否出现过极其明显的量价背离现象，下跌幅度是否很大，且后期时间上无明显背驰，总体来说该买点处于类2买位置，安全性比较高，需警惕下跌中继*/
			ELSE IF(fudubizhi[2]<FUDU_LEVEL1 AND  fudubizhi[3]>FUDU_LEVEL2) THEN BEGIN
				/*分情况可以买入*/
				;
			END
			
	
			/*这种情况是前期小幅下跌后中幅幅回升后再小幅下跌并出现了背驰，这种情况通常为上升中继，可买入*/
			ELSE IF((fudubizhi[2]<=FUDU_LEVEL2 AND fudubizhi[2]>=FUDU_LEVEL1) AND fudubizhi[3]<FUDU_LEVEL1 ) THEN BEGIN
				/*可以买入*/
				;
			END	
			/*这种情况是前期中幅下跌后中幅幅回升后再小幅下跌并出现了背驰，通常情况下要跌到上次下跌的位置才比较合理，但是如果出现了背驰，也是可以考虑买入的*/
			/*下跌到的位置如果高于中幅下跌前的高点，则可认为是类三买;否则需要参考fudubizhi[4]的值：大幅上升可买入，小幅上升有风险可少买；中幅上升表示中幅震荡，如果最近一次上升斜率更大，可考虑买入，注意这几种情况只能保证上升到前次高点*/
			/*如果还达不到前期的高点那么，就是转换成所谓的类二卖了，这个点位买股还是要明查的，陷阱比较多*/
			ELSE IF((fudubizhi[2]<=FUDU_LEVEL2 AND fudubizhi[2]>=FUDU_LEVEL1) AND (fudubizhi[3]>=FUDU_LEVEL1 AND fudubizhi[3]<=FUDU_LEVEL2)) THEN BEGIN
				/*分情况可以买入*/
				;
			END
			/*这种情况是前期大幅下跌后中幅回升后再小幅下跌并出现了背驰，这种情况存在比较大的概率由反弹变为反转，买入的风险不是很大，但后面也存在演化为盘整的可能，那就是后期的事了*/
			ELSE IF((fudubizhi[2]<FUDU_LEVEL2 AND fudubizhi[2]>=FUDU_LEVEL1) AND fudubizhi[3]>FUDU_LEVEL2) THEN BEGIN
				/*可以买入*/
				;
			END			
			
			
			/*这种情况是前期大幅下上升后再小幅下跌并出现了背驰，这个点位再买就有追高嫌疑了，风险还是比较大的，如果近期出现了成交量放出近一阶段新高，则坚决不能买！*/
			/*未出现明显背驰的情况下也是可以介入的，因为这也有可能是牛市中继*/
			ELSE IF(fudubizhi[2]>FUDU_LEVEL2) THEN BEGIN
				/*可以买入*/
				;
			END			
	
			ELSE BEGIN
				;
			END
		END
		
		
		/*当前正处于中幅下跌状态，并出现了背驰；如果当前是平衡市这个点位买股是比较好的，因为已经跌出了一定幅度，是中短线波段的好机会*/
		/*但是要注意这也可能是暴跌阶段的一个多头陷阱：从以下两个方面规避：1、前期出现了大的量价背离，2、大盘是否已经进入了暴跌阶段；还要看其他消息面*/
		ELSE IF((fudubizhi[1]<FUDU_LEVEL2) AND (fudubizhi[1]>FUDU_LEVEL1)) THEN BEGIN
	
			/*如果前两个笔的幅度都小于Level1,最近出现中幅下跌并背驰；这种情况下要看下跌前的盘整前或者盘整阶段是否一直处于背离；是否一直处于高价位*/
			/*当然还有一种可能是空头陷阱，如果是这种情况，后面通常会有比较可观的上涨，因为此前的盘整通常是庄家吸货，中幅下跌通常是庄家上涨前下跌试盘行为*/
			IF(fudubizhi[2]<FUDU_LEVEL1 AND fudubizhi[3]<FUDU_LEVEL1) THEN BEGIN
				/*SELL if HAVE ANY STOCK AND NOT BUY ANY*/
				;
			END					
			/*这种情况是前期中幅下跌后小幅回升后再中幅下跌并出现了背驰，这个时候需要比较前两次下跌笔是否出现背驰，如果出现背驰则回升力度大概率大于上次上升力度*/
			/*如果未出现背驰或者加速下跌了，这个可能为下跌中继，建议不要买入，风险比较大*/
			ELSE IF(fudubizhi[2]<FUDU_LEVEL1 AND (fudubizhi[3]>=FUDU_LEVEL1 AND fudubizhi[3]<=FUDU_LEVEL2)) THEN BEGIN
				/*分情况可以买入*/
				;
			END
			/*这种情况是前期大幅下跌后小幅回升后再中幅下跌并出现了背驰，中幅下跌说明此前跌势已经很大，理论上至少存在反弹需求，这种出现背驰点的买入一般情况下相对比较安全*/
			/*需要结合多种条件判断，比如下跌之前是否一直为高位小幅震荡即庄家出货，是否出现过极其明显的量价背离现象，下跌幅度是否很大，且后期时间上无明显背驰，总体来说该买点处于类2买位置，安全性比较高，需警惕下跌中继*/
			ELSE IF(fudubizhi[2]<FUDU_LEVEL1 AND  fudubizhi[3]>FUDU_LEVEL2) THEN BEGIN
				/*分情况可以买入*/
				;
			END
			
	
			/*这种情况是前期小幅下跌后中幅幅回升后再中幅下跌并出现了背驰，这种情况通常为上升中继，可买入*/
			/*但需要关心中幅上升到最高点是否出现了异常的量价背离，如果出现了异常的量价背离，最好少量买甚至不买，存在下跌中继的风险*/
			ELSE IF((fudubizhi[2]<=FUDU_LEVEL2 AND fudubizhi[2]>=FUDU_LEVEL1) AND fudubizhi[3]<FUDU_LEVEL1 ) THEN BEGIN
				/*分情况可以买入*/
				;
			END	
			/*这种情况是前期中幅下跌后中幅幅回升后再中幅下跌并出现了背驰，通常情况下考虑盘整背驰的方式决定是否买入*/
			/*对于盘整背驰还需要考察背驰段的天数以及斜率，据此可考察买入后能否快速上涨以及上涨幅度；当然这是更高的追求了*/
			/*如果下跌最低点不破前期低点，那么理论上大概率上涨突破前期的高点，这是人性的特点反映*/
			ELSE IF((fudubizhi[2]<=FUDU_LEVEL2 AND fudubizhi[2]>=FUDU_LEVEL1) AND (fudubizhi[3]>=FUDU_LEVEL1 AND fudubizhi[3]<=FUDU_LEVEL2)) THEN BEGIN
				/*分情况可以买入*/
				;
			END
			/*这种情况是前期大幅下跌后中幅幅回升后再中幅下跌并出现了背驰，通常情况下考虑盘整背驰的方式决定是否买入*/
			/*对于盘整背驰还需要考察背驰段的天数以及斜率，据此可考察买入后能否快速上涨以及上涨幅度；当然这是更高的追求了*/
			/*如果下跌最低点不破前期低点，那么理论上大概率上涨突破前期的高点，这是人性的特点反映*/			
			ELSE IF((fudubizhi[2]<FUDU_LEVEL2 AND fudubizhi[2]>=FUDU_LEVEL1) AND fudubizhi[3]>FUDU_LEVEL2) THEN BEGIN
				/*可以买入*/
				;
			END			
			
			
			/*这种情况是前期大幅下上升后再小幅下跌并出现了背驰，这个点位再买就有追高嫌疑了，风险还是比较大的，如果近期出现了成交量放出近一阶段新高，则坚决不能买！*/
			/*未出现明显背驰的情况下也是可以介入的，因为这也有可能是牛市中继*/
			ELSE IF(fudubizhi[2]>FUDU_LEVEL2) THEN BEGIN
				/*可以买入*/
				;
			END				
			ELSE BEGIN
				;
			END				
	
		END	
			
		
		/*当前正在形成的一笔一值处于大幅度下降中,这个时候通常是熊市,最好等确定出现下跌的一笔后或者二买时参与,出现背驰的时候只能极少量介入*/
		ELSE IF(fudubizhi[1]>FUDU_LEVEL2) THEN 	BEGIN				
			;
			/*这种情况最好不要买股，因为下跌已经形成，如决堤的洪水，不可能说止就能止住的；最好的方法是在确认下跌笔以后利用更低级别寻找买卖点*/
			/*多说一句，低级别的介入最好的时机是在本级别的下降的笔刚被确认阶段杀入；低级别操作最好是在高级别上升笔过程中介入*/

		END

	END
	
	
	/*底分型即下降一笔已经形成，正在形成上升一笔过程中；这个过程的中心思想是持有股票，并寻找机会卖股，直到发现出现了某种程度的背驰为止可卖出*/
	/*理论上卖出是在上升一笔中干的活，可能卖出后会出现更高的点，但这个跟你已经没有关系了，不必后悔*/	
	ELSE IF(oldbi[1]<0) THEN BEGIN
	
		/*目前尚处于小幅上升中,并形成了背驰，理论上来说，这个时候离买入的时间并不是太长，因此出现特定的小级别背驰没有必要卖出，否则此前干吗买入呢？*/
		/*当然何时卖出有两个点：1、止损点；2、上升一笔确定点；因此这个时候最好还是不要随便卖出！*/
		/*当然如果大盘已经进入了暴跌阶段，还是要抛出的，毕竟倾巢之下焉有完卵，且大盘的暴跌一定为助长个股的暴跌的！*/
		/*该上升点如果已经高于前期的高点了，出现背驰的情况下抛掉一些也是可以的*/
		/*这种情况就是买入后向上的笔是出来了,但是刚出来不久就存在背驰了,这种情况建议使用低级别的买卖点判断*/
		IF(fudubizhi[1]<FUDU_LEVEL1) THEN BEGIN	
			;
		END
		
		
		/*当前正处于中幅上升状态，并出现了背驰；出现背驰是一定要卖出的，只是说根据前期背驰恶劣程度决定卖出多少*/
		/*当然如果大盘已经进入了暴跌阶段，还是要抛出的，毕竟倾巢之下焉有完卵，且大盘的暴跌一定会助长个股的暴跌的！*/
		/*该上升点如果已经高于前期的高点了，出现背驰的情况下抛掉一些也是可以的，剩下的可在完成上升一笔确认的时候抛出*/
		/*如果上升过程中出现了异常的的量价背离，则可选择大量卖出，毕竟没有上升过程中没有卖错的；即使后面可能会涨的更高，天量量价背驰最可怕背驰*/	
		/*如果上升点位尚未达到前期高点，那要根据前期买入的背驰程度决定卖出多少，因此前期买入的时候最好纪录一下当时的背驰状况*/
		ELSE IF((fudubizhi[1]<FUDU_LEVEL2) AND (fudubizhi[1]>FUDU_LEVEL1)) THEN BEGIN
			;
		END	
			
		/*当前正处于大幅上升状态，并出现了背驰；出现背驰是一定要卖出的，毕竟此时已经收获良多，落袋为安。同时根据前期背驰恶劣程度决定卖出多少*/
		/*当然如果大盘已经进入了暴跌阶段，还是要抛出的，毕竟倾巢之下焉有完卵，且大盘的暴跌一定会助长个股的暴跌的！*/
		/*该上升点如果已经高于前期的高点了，出现背驰的情况下多抛掉一些也是可以的，剩下的可在完成上升一笔确认的时候抛出*/
		/*如果上升过程中出现了异常的的量价背离，则可选择完全卖出，毕竟没有上升过程中没有卖错的；即使后面可能会涨的更高，天量量价背驰最可怕背驰*/	
		/*在其他出现背驰的情况，还是要卖出一些的，可以少量卖出*/	
		ELSE IF(fudubizhi[1]>FUDU_LEVEL2) THEN BEGIN	
			;
			/*这种情况最好不要买股，因为下跌已经形成，如决堤的洪水，不可能说止就能止住的；最好的方法是在确认下跌笔以后利用更低级别寻找买卖点*/
			/*多说一句，低级别的介入最好的时机是在本级别的下降的笔刚被确认阶段杀入；低级别操作最好是在高级别上升笔过程中介入*/
		END	
		ELSE BEGIN
			;
		END
		
	END
	

END

aaaaa:buy_flag;