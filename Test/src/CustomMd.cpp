#include <iostream>
#include <cstring>
#include <unordered_map>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "CustomMd.h"
#include "TickToKlineHelper.h"

using std::cout;
using std::endl;
using std::cerr;

// global value from main.cpp
extern char dataDicPath[100];                    // data's dictionary path
extern TThostFtdcBrokerIDType gBrokerID;         // 模拟经纪商代码
extern TThostFtdcInvestorIDType gInvesterID;     // 投资者账户名
extern TThostFtdcPasswordType gInvesterPassword; // 投资者密码

extern CThostFtdcMdApi *gp_MdUserApi;            // 行情指针
extern char gMdFrontAddr[50];                    // 模拟行情前置地址
extern char *gp_InstrumentID[];                  // 行情合约代码列表，中、上、大、郑交易所各选一种
extern int instrumentNum;                        // 行情合约订阅数量

// extern std::unordered_map<std::string, TickToKlineHelper> g_KlineHash; // k线存储表

// structure
CustomMd::CustomMd(
	TThostFtdcInvestorIDType gInvesterID,
	TThostFtdcPasswordType gInvesterPassword,
	TThostFtdcBrokerIDType gBrokerID, 
	char gMdFrontAddr[],
	char dataDicPath[])
{
	strcpy(this->sBrokerID, gBrokerID);
	strcpy(this->sInvesterID, gInvesterID);
	strcpy(this->sInvesterPassword, gInvesterPassword);
	vSubInstrumentID = vector<char*>();
	vQuoteInstrumentID = vector<char*>();

	pMdUserApi = CThostFtdcMdApi::CreateFtdcMdApi(dataDicPath);
	pMdUserApi->RegisterSpi(this);
	pMdUserApi->RegisterFront(gMdFrontAddr);
	pMdUserApi->Init();
}

// instructure
CustomMd::~CustomMd() {
	// clear vector
	for (char *str : vSubInstrumentID) {
		delete[] str;
	}
	for (char *str : vQuoteInstrumentID) {
		delete[] str;
	}
	vSubInstrumentID.clear();
	vQuoteInstrumentID.clear();

	pMdUserApi->Release();
}

void userLogin() {

}

void userLogout() {
	
}

void subscribeInstrument() {

}

void unsubscribeInstrument() {

}

void quoteInstrument() {

}

void unQuoteInstrument() {

}



/* ----- MdSpi回调函数 ----- */
// show error message , this is inline function
inline void showErrorMessage(CThostFtdcRspInfoField *pRspInfo) 
{
	cerr << "Error code: " << pRspInfo->ErrorID << endl;
	cerr << "Error message: " << pRspInfo->ErrorMsg << endl;
}

// 连接成功应答
void CustomMd::OnFrontConnected()
{
	cout << "===== Connection Success =====" << endl;

	// 开始登录
	CThostFtdcReqUserLoginField loginReq;
	memset(&loginReq, 0, sizeof(loginReq));
	strcpy(loginReq.BrokerID, gBrokerID);
	strcpy(loginReq.UserID, gInvesterID);
	strcpy(loginReq.Password, gInvesterPassword);
	static int requestID = 0; // 请求编号

	int rt = gp_MdUserApi->ReqUserLogin(&loginReq, requestID);
	if (!rt)
		cout << ">>>>> Send user request success" << endl;
	else
		cerr << "!!!!! Send user request failed" << endl;
}

// 断开连接通知
void CustomMd::OnFrontDisconnected(int nReason)
{
	cerr << "===== Connection Break off =====" << endl;
	cerr << "Error code: 0x" << std::hex << nReason << endl;
}

// 心跳超时警告
void CustomMd::OnHeartBeatWarning(int nTimeLapse)
{
	cerr << "===== Heart Beat Warning =====" << endl;
	cerr << "The interval connect time： " << nTimeLapse << endl;
}

// 登录应答
void CustomMd::OnRspUserLogin(
	CThostFtdcRspUserLoginField *pRspUserLogin, 
	CThostFtdcRspInfoField *pRspInfo, 
	int nRequestID, 
	bool bIsLast)
{
	bool bResult = pRspInfo && (pRspInfo->ErrorID != 0);
	if (!bResult)
	{
		cout << "===== User Login Success =====" << endl;
		cout << "===== User Information as Follow =====" << endl;
		cout << "Tradeding day: " << pRspUserLogin->TradingDay << endl;
		cout << "Login time: " << pRspUserLogin->LoginTime << endl;
		cout << "Broker ID: " << pRspUserLogin->BrokerID << endl;
		cout << "User ID: " << pRspUserLogin->UserID << endl;
		
		// 开始订阅行情
		int rt = gp_MdUserApi->SubscribeMarketData(gp_InstrumentID, instrumentNum);
		if (!rt)
			cout << ">>>>> Send subscribed market data request success" << endl;
		else
			cerr << "!!!!! Send subscribed market data request failed" << endl;
	}
	else {
		cerr << "===== Login Failed =====" << endl;
		showErrorMessage(pRspInfo);
	}
}

// 登出应答
void CustomMd::OnRspUserLogout(
	CThostFtdcUserLogoutField *pUserLogout,
	CThostFtdcRspInfoField *pRspInfo, 
	int nRequestID, 
	bool bIsLast)
{
	bool bResult = pRspInfo && (pRspInfo->ErrorID != 0);
	if (!bResult)
	{
		cout << "===== User Logout Success =====" << endl;
		cout << "===== User Information as Follow =====" << endl;
		cout << "Broker ID: " << pUserLogout->BrokerID << endl;
		cout << "User ID: " << pUserLogout->UserID << endl;
	}
	else {
		cerr << "===== Login Failed =====" << endl;
		showErrorMessage(pRspInfo);
	}
}

// 错误通知
void CustomMd::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	bool bResult = pRspInfo && (pRspInfo->ErrorID != 0);
	if (bResult) {
		cerr << "===== Something Error =====" << endl;
		showErrorMessage(pRspInfo);
	}
}

// 订阅行情应答
void CustomMd::OnRspSubMarketData(
	CThostFtdcSpecificInstrumentField *pSpecificInstrument, 
	CThostFtdcRspInfoField *pRspInfo, 
	int nRequestID, 
	bool bIsLast)
{
	bool bResult = pRspInfo && (pRspInfo->ErrorID != 0);
	if (!bResult)
	{
		cout << "===== Subscribe Market Data Success =====" << endl;
		cout << "Intrument ID: " << pSpecificInstrument->InstrumentID << endl;
		// 如果需要存入文件或者数据库，在这里创建表头,不同的合约单独存储
		char filePath[100];
		strncpy(filePath, dataDicPath, 100);
		sprintf(filePath, "%s_market_data.csv", pSpecificInstrument->InstrumentID);
		if ( (access(filePath, W_OK)) == -1 ) 
		{
			FILE* fd = fopen(filePath, "w");
			fprintf(fd, "合约代码,"
				        "更新时间,"
				        "最新价,"
				        "成交量,"
				        "买价一,"
				        "买量一,"
				        "卖价一,"
				        "卖量一,"
				        "持仓量,"
				        "换手率\n");
			fclose(fd);
		}
	}
	else {
		cerr << "===== Subscribe Market Data Failed =====" << endl;
		showErrorMessage(pRspInfo);
	}
}

// 取消订阅行情应答
void CustomMd::OnRspUnSubMarketData(
	CThostFtdcSpecificInstrumentField *pSpecificInstrument, 
	CThostFtdcRspInfoField *pRspInfo,
	int nRequestID, 
	bool bIsLast)
{
	bool bResult = pRspInfo && (pRspInfo->ErrorID != 0);
	if (!bResult)
	{
		cout << "===== Unsubscribe Market Data Success =====" << endl;
		cout << "Intrument ID: " << pSpecificInstrument->InstrumentID << endl;
	}
	else {
		cerr << "===== Unsubscribe Market Data Failed =====" << endl;
		showErrorMessage(pRspInfo);
	}
}

// 订阅询价应答
void CustomMd::OnRspSubForQuoteRsp(
	CThostFtdcSpecificInstrumentField *pSpecificInstrument,
	CThostFtdcRspInfoField *pRspInfo,
	int nRequestID,
	bool bIsLast)
{
	bool bResult = pRspInfo && (pRspInfo->ErrorID != 0);
	if (!bResult)
	{
		cout << "===== Subscribe Quote Success =====" << endl;
		cout << "Intrument ID: " << pSpecificInstrument->InstrumentID << endl;
	}
	else {
		cerr << "===== Subscribe Quote Failed =====" << endl;
		showErrorMessage(pRspInfo);
	}
}

// 取消订阅询价应答
void CustomMd::OnRspUnSubForQuoteRsp(
	CThostFtdcSpecificInstrumentField *pSpecificInstrument, 
	CThostFtdcRspInfoField *pRspInfo, 
	int nRequestID, 
	bool bIsLast)
{
	bool bResult = pRspInfo && (pRspInfo->ErrorID != 0);
	if (!bResult)
	{
		cout << "===== Unsubscribe Quote Success =====" << endl;
		cout << "Intrument ID: " << pSpecificInstrument->InstrumentID << endl;
	}
	else {
		cerr << "===== Unsubscribe Quote Failed =====" << endl;
		showErrorMessage(pRspInfo);
	}
}

/*
 i
 m
 p
 o
 r
 t
 a
 n
 t
*/
// 行情详情通知
void CustomMd::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)
{
	// 打印行情，字段较多，截取部分
	cout << "===== Get Depth Market Data =====" << endl;
	cout << "Trading Day: " << pDepthMarketData->TradingDay << endl;
	cout << "Exchange ID: " << pDepthMarketData->ExchangeID << endl;
	cout << "Instrument ID: " << pDepthMarketData->InstrumentID << endl;
	cout << "Last Price: " << pDepthMarketData->LastPrice << endl;
	cout << "Volume: " << pDepthMarketData->Volume << endl;

	// 如果只获取某一个合约行情，可以逐tick地存入文件或数据库
	char filePath[100];
	strncpy(filePath, dataDicPath, 100);
	sprintf(filePath, "%s_market_data.csv", pDepthMarketData->InstrumentID);

	if ( (access(filePath, W_OK)) == -1 ){
		cerr << "!!!!! File " << filePath << " not exist!" << endl;
	} else {
		FILE* fd = fopen(filePath, "a");
		fprintf(fd, "%s,"
			        "%s.%d,"
			        "%lf,"
			        "%d,"
			        "%lf,"
			        "%d,"
			        "%lf,"
			        "%d,"
			        "%lf,"
			        "%lf\n",
			        pDepthMarketData->InstrumentID,
			        pDepthMarketData->UpdateTime, pDepthMarketData->UpdateMillisec,
			        pDepthMarketData->LastPrice,
			        pDepthMarketData->Volume,
			        pDepthMarketData->BidPrice1,
			        pDepthMarketData->BidVolume1,
			        pDepthMarketData->AskPrice1,
			        pDepthMarketData->AskVolume1,
			        pDepthMarketData->OpenInterest,
			        pDepthMarketData->Turnover
			        );
		fclose(fd);
	} 
}

// 询价详情通知
void CustomMd::OnRtnForQuoteRsp(CThostFtdcForQuoteRspField *pForQuoteRsp)
{
	// 部分询价结果
	cout << "===== Get Quote Data =====" << endl;
	cout << "Trading Day: " << pForQuoteRsp->TradingDay << endl;
	cout << "Exchange ID: " << pForQuoteRsp->ExchangeID << endl;
	cout << "Instrument ID: " << pForQuoteRsp->InstrumentID << endl;
	cout << "Quote system ID: " << pForQuoteRsp->ForQuoteSysID << endl;
}