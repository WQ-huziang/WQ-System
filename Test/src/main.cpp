#include <iostream>
#include <stdio.h>
#include <string>
#include <unordered_map>
#include "ThostFtdcUserApiStruct.h"
#include "ThostFtdcUserApiDataType.h"
#include "CustomMd.h"
// #include "CustomTradeSpi.h"
// #include "TickToKlineHelper.h"

using namespace std;

// Global value
// public value
char dataDicPath[100] = "data/";                                   // data's dictionary path
TThostFtdcBrokerIDType gCompanyID = "9999";                        // company ID
TThostFtdcInvestorIDType gInvesterID = "";                             // user ID
TThostFtdcPasswordType gInvesterPassword = "";                         // user password

// MD value
CThostFtdcMdApi *gp_MdUserApi = nullptr;                           // Md api's pointer
char gp_MdFrontAddr[] = "tcp://180.168.146.187:10011";             // Md front address
char *gp_InstrumentID[] = {"AP805"};                               // stock ID
int instrumentNum = 1;                                             // number of stock ID
// unordered_map<string, TickToKlineHelper> g_KlineHash;              // 

// // Td value
// CThostFtdcTraderApi *g_pTradeUserApi = nullptr;                    // 
// char gTradeFrontAddr[] = "tcp://180.168.146.187:10001";            // 
// TThostFtdcInstrumentIDType g_pTradeInstrumentID = "zn1705";        // 
// TThostFtdcDirectionType gTradeDirection = THOST_FTDC_D_Sell;       // 
// TThostFtdcPriceType gLimitPrice = 22735;                           // 

int main(int argv, char *argc)
{
	// input ID and password
	cout << "Please input your ID: ";
	scanf("%s", gInvesterID);
	cout << "Please input your password: ";
	scanf("%s", gInvesterPassword);

	// Initialize Md
	CustomMd *customMdApi = CustomMd::CreateCustomMd(gInvesterID, gInvesterPassword);
	customMdApi->subscribeInstrument(gp_InstrumentID, instrumentNum);
	
	cout << "Initialize market description..." << endl;
	cout << "Getting the information..." << endl;

	while (1) {

	}
	// gp_MdUserApi = CThostFtdcMdApi::CreateFtdcMdApi();
	// CThostFtdcMdSpi *pMdUserSpi = new CustomMdSpi;       // ´´½¨ÐÐÇé»Øµ÷ÊµÀý
	// g_pMdUserApi->RegisterSpi(pMdUserSpi);               // ×¢²áÊÂ¼þÀà
	// g_pMdUserApi->RegisterFront(gMdFrontAddr);           // ÉèÖÃÐÐÇéÇ°ÖÃµØÖ·
	// g_pMdUserApi->Init();                                // Á¬½ÓÔËÐÐ
	


	// ³õÊ¼»¯½»Ò×Ïß³Ì
	// cout << "³õÊ¼»¯½»Ò×..." << endl;
	// g_pTradeUserApi = CThostFtdcTraderApi::CreateFtdcTraderApi(); // ´´½¨½»Ò×ÊµÀý
	// //CThostFtdcTraderSpi *pTradeSpi = new CustomTradeSpi;
	// CustomTradeSpi *pTradeSpi = new CustomTradeSpi;               // ´´½¨½»Ò×»Øµ÷ÊµÀý
	// g_pTradeUserApi->RegisterSpi(pTradeSpi);                      // ×¢²áÊÂ¼þÀà
	// g_pTradeUserApi->SubscribePublicTopic(THOST_TERT_RESTART);    // ¶©ÔÄ¹«¹²Á÷
	// g_pTradeUserApi->SubscribePrivateTopic(THOST_TERT_RESTART);   // ¶©ÔÄË½ÓÐÁ÷
	// g_pTradeUserApi->RegisterFront(gTradeFrontAddr);              // ÉèÖÃ½»Ò×Ç°ÖÃµØÖ·
	// g_pTradeUserApi->Init();                                      // Á¬½ÓÔËÐÐ
		

	// µÈµ½Ïß³ÌÍË³ö
	// g_pMdUserApi->Join();
	// delete pMdUserSpi;
	// g_pMdUserApi->Release();

	// g_pTradeUserApi->Join();
	// delete pTradeSpi;
	// g_pTradeUserApi->Release();

	// ×ª»»±¾µØkÏßÊý¾Ý
	//TickToKlineHelper tickToKlineHelper;
	//tickToKlineHelper.KLineFromLocalData("market_data.csv", "K_line_data.csv");
	
	getchar();
	return 0;
}