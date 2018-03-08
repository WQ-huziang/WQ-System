#include <iostream>
#include <stdio.h>
#include <string>
#include <unordered_map>
#include "ThostFtdcUserApiStruct.h"
#include "ThostFtdcUserApiDataType.h"
#include "CustomMd.h"
#include "CustomTrade.h"

using namespace std;

// Global value
// public value
char dataDicPath[100] = "data/";                                   // data's dictionary path
TThostFtdcBrokerIDType gCompanyID = "9999";                        // company ID
TThostFtdcInvestorIDType gInvesterID = "";                             // user ID
TThostFtdcPasswordType gInvesterPassword = "";                         // user password

char *pInstrumentID[] = {"AP805"};                               // stock ID
int instrumentNum = 1;                                             // number of stock ID

// // Td value
// CThostFtdcTraderApi *g_pTradeUserApi = nullptr;                    // 
// char gTradeFrontAddr[] = "tcp://180.168.146.187:10001";            // 
TThostFtdcInstrumentIDType g_pTradeInstrumentID = "zn1705";
TThostFtdcDirectionType gTradeDirection = THOST_FTDC_D_Sell;
TThostFtdcPriceType gLimitPrice = 22735; 

int main(int argv, char *argc)
{
	// input ID and password
	cout << "Please input your ID: ";
	scanf("%s", gInvesterID);
	cout << "Please input your password: ";
	scanf("%s", gInvesterPassword);

	// Initialize Md
	CustomMd *customMdApi = CustomMd::CreateCustomMd(gInvesterID, gInvesterPassword);
	cout << "Initialize market description..." << endl;
	cout << "Getting the information..." << endl;
	customMdApi->subscribeInstrument(pInstrumentID, instrumentNum);
	


	CustomTrade *customTradeApi = CustomTrade::CreateCustomTrade(gInvesterID, gInvesterPassword);
	cout << "Initialize user trade..." << endl;
	customTradeApi->userLogin();
	customTradeApi->orderInsert(pInstrumentID[0], 10000, 1, THOST_FTDC_D_Buy);

	while (1) {

	}
	return 0;
}