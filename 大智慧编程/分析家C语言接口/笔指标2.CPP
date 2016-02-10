variable: oldbi[51]=0,drawlen=0,drawflag=0;
variable: changdu[11]=0,fudu[11]=0,fudubizhi[11]=0,xielv[11]=0,mianji[11]=0;
M:=5;


var1:="chzhshch@kxian";/*处理K线*/
var2:="chzhshch@bi";/*处理笔关系*/
drawline(var2=1,h,var2=-1,l,0),pointdot,coloryellow;
drawline(var2=-1,l,var2=1,h,0),pointdot,coloryellow;
var3:="chzhshch@duan";/*处理段关系*/
drawtext(var3=1,h,''+h),align2,colormagenta;
drawtext(var3=-1,l,''+l),colorgreen;
drawline(var3=1,h,var3=-1,l,0),colorff6600;
drawline(var3=-1,l,var3=1,h,0),colorff6600;



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




/*将确定的笔值赋值给数组oldbi[]*/
IF(orgin_flag<>0 AND oldbi[1]=0) THEN BEGIN
		oldbi[1] := orgin_flag;

END
IF(di_flag<0  AND oldbi[1]>0) THEN BEGIN
	FOR I=1 TO 49 DO BEGIN
		oldbi[51-I]:=oldbi[50-I];
	END
	oldbi[1]:=di_flag;

END
IF(ding_flag>0 AND oldbi[1]<0 ) THEN BEGIN
	FOR I=1 TO 49 DO BEGIN
		oldbi[51-I]:=oldbi[50-I];
	END
	oldbi[1]:=ding_flag;		

END;




/*上面的代码已经实现了寻找确定的笔，将其结果保存在oldbi[]数组中了，oldbi[1]表示最近一个确定的笔的顶点或底点；正数为顶点，负数为底点*/
/*下面的代码将对这些笔的其他相关属性进行计算，便于后面的笔间比较*/


/*计算出最近几次笔对应的长度、幅度、斜率、面积，只有长度恒为正；其他变量值为正则表示处于上升笔，为负则表示下降笔*/
IF(oldbi[9]<>0) THEN BEGIN

	FOR I=1 TO 6 DO BEGIN
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
		
		IF(oldbi[1]>0) THEN BEGIN	
			fudubizhi[I+1]:=ABS(fudu[I+1]/oldbi[I]);
		END
		ELSE BEGIN
			fudubizhi[I+1]:=ABS(fudu[I+1]/oldbi[I+1]);
		END
	
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
MYTEST:mianji[1];

/*这里的幅度指的是比值,也就是增加或者减小值*/
/*振幅低于10%波动的股票不值得参与*/
FUDU_LEVEL1 := 15/100;
/*振幅高于30%波动的股票可认为处于上升或者下跌过程中,这个过程下跌建议找二买或者下降笔得到确认后再买,上升过程中出现回调且无大的背离可买入*/
FUDU_LEVEL2 := 40/100;


/*一个重要规则：上升笔的确认点必须不能持股，即使此前持有股票（多为没有在上升笔形成阶段找到合适卖点），必须清仓！*/
/*另一个重要规则：下降笔确认点可进一步确认此前下降笔中买入的股票是安全、成功的，这些股票必须在此后上升笔形成中出现背驰卖出！*/


/*理论上只有在上升的一笔得到确认后，且下降的一笔尚未得到确认的时候买股；为最低点买股，必然不会被套当然买股时还需要买足其他的背驰条件，这通常对应于一买或者是二买*/
/*特殊恶劣的情况下可以在下降一笔得到确认的初期买股，这通常对应着暴跌或者是长期阴跌或者是量价极度背驰后买股，也就是*/
/*下降一笔得到确认后的上升一笔，此时照理来说前期已经买入股票了，只要持股即可，如果还有货币可在上升一笔的适当回调中买入，在上升一笔的过程中*/
/*一定要密切关注量价极度背驰的情况，一旦出现量价极度背驰的情况，即使上升的笔尚未被下降的笔打断也要出一定量的股*/
/*同时在笔的上升过程中也可以到下一个级别去做一定仓位的短差，此时短差肯定不会被套的，这是由高一级别的笔的上升性质决定的*/
/*若前几笔的幅度都比较小或者斜率都非常小，那么说明该股的波动是比较小的，小波动的股票必然没有大的利差，且对于高位小幅波动的股票存在暴跌的风险*/
/*低位小幅长期震荡的股票则存在后期突然发力的可能性，对于这种情况，启用bool函数适时跟踪，当发现出现大幅波动后可介入。*/
IF(oldbi[9]<>0) THEN BEGIN


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
	IF(oldbi[1]>0) THEN BEGIN
		/*目前尚处于小幅下降中,谨慎下跌中继和高位背驰下跌,属于缠中说禅小幅盘整状态，不能参与,还有为小幅震荡也不宜参与*/
		IF(fudubizhi[1]<FUDU_LEVEL1) THEN BEGIN			
		
			/*如果前三个笔的幅度都小于Level1,说明是小幅度的震荡,不适宜参与*/
			IF(fudubizhi[2]<FUDU_LEVEL1 AND fudubizhi[3]<FUDU_LEVEL1) THEN BEGIN
				/*SELL if HAVE ANY STOCK AND NOT BUY ANY*/
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
			
			/*这种情况最好不要买股，因为下跌已经形成，如决堤的洪水，不可能说止就能止住的；最好的方法是在确认下跌笔以后利用更低级别寻找买卖点*/
			/*多说一句，低级别的介入最好的时机是在本级别的下降的笔刚被确认阶段杀入；低级别操作最好是在高级别上升笔过程中介入*/
			;
		
		END

	END
	
	
	/*底分型即下降一笔已经形成，正在形成上升一笔过程中；这个过程的中心思想是持有股票，并寻找机会卖股，直到发现出现了某种程度的背驰为止可卖出*/
	/*理论上卖出是在上升一笔中干的活，可能卖出后会出现更高的点，但这个跟你已经没有关系了，不必后悔*/	
	IF(oldbi[1]<0) THEN BEGIN
	/*目前尚处于小幅上升中,谨慎下跌中继和高位背驰下跌,属于缠中说禅小幅盘整状态，不能参与,还有为小幅震荡也不宜参与*/
		IF(fudubizhi[1]<FUDU_LEVEL1) THEN BEGIN			
		
			/*如果前三个笔的幅度都小于Level1,说明是小幅度的震荡,不适宜参与*/
			IF(fudubizhi[2]<FUDU_LEVEL1 AND fudubizhi[3]<FUDU_LEVEL1) THEN BEGIN
				/*SELL if HAVE ANY STOCK AND NOT BUY ANY*/
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
			
			/*这种情况最好不要买股，因为下跌已经形成，如决堤的洪水，不可能说止就能止住的；最好的方法是在确认下跌笔以后利用更低级别寻找买卖点*/
			/*多说一句，低级别的介入最好的时机是在本级别的下降的笔刚被确认阶段杀入；低级别操作最好是在高级别上升笔过程中介入*/
			;
		END	
	
	
	
	;
	END
	

END;
