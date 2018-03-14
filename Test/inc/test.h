#pragma once

#include <iostream>
#include <cstdio>
using std::cout;
using std::endl;

void test1(const WZMarketDataField &ori) {
	cout << "SHOW DATA:" << endl;
	fprintf(stdout, "%s\n"
				 "%s\n"
				 "%s\n"
				 "%s\n"
				 "%.4lf\n"
				 "%.4lf\n"
				 "%.4lf\n"
				 "%.4lf\n"
				 "%.4lf\n"
				 "%.4lf\n"
				 "%.4lf\b"
				 "%d\n"
				 "%.4lf\n"
				 "%.4lf\n"
				 "%.4lf\n"
				 "%.4lf\n"
				 "%.4lf\n"
				 "%.4lf\n"
				 "%.4lf\n"
				 "%s\n"
				 "%d\n"
				 "%.4lf\n"
				 "%d\n"
				 "%.4lf\n"
				 "%d\n\n",
				 ori.TradingDay,
				 ori.InstrumentID,
				 ori.ExchangeID,
				 ori.ExchangeInstID,
				 ori.LastPrice,
				 ori.PreSettlementPrice,
				 ori.PreClosePrice,
				 ori.PreOpenInterest,
				 ori.OpenPrice,
				 ori.HighestPrice,
				 ori.LowestPrice,
				 ori.Volume,
				 ori.Turnover,
				 ori.OpenInterest,
				 ori.ClosePrice,
				 ori.SettlementPrice,
				 ori.UpperLimitPrice,
				 ori.LowerLimitPrice,
				 ori.PreDelta,
				 ori.UpdateTime,
				 ori.UpdateMillisec,
				 ori.BidPrice1,
				 ori.BidVolume1,
				 ori.AskPrice1,
				 ori.AskVolume1);
}