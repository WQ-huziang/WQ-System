#include <iostream>
#include <cstring>
#include <unordered_map>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "adapter/CTPAdapter.h"
#include "md/CustomCtpMd.h"

using std::endl;
using std::cerr;
using std::cout;

// constructure
CustomCtpMd* CustomCtpMd::CreateCustomCtpMd(
	TThostFtdcInvestorIDType gInvesterID,
	TThostFtdcPasswordType gInvesterPassword,
	TThostFtdcBrokerIDType gBrokerID, 
	char dataDirPath[],
	char gMdFrontAddr[],
	int clientID,
	int serverID)
{
	CustomCtpMd *newCustomCtpMd = new CustomCtpMd(gBrokerID, gInvesterID, gInvesterPassword, dataDirPath, clientID, serverID);
    newCustomCtpMd->nRequestID = 0;
    newCustomCtpMd->mylock = new Semaphore();

	newCustomCtpMd->pMdUserApi = CThostFtdcMdApi::CreateFtdcMdApi(dataDirPath);
	newCustomCtpMd->pMdUserApi->RegisterSpi(newCustomCtpMd);
	newCustomCtpMd->pMdUserApi->RegisterFront(gMdFrontAddr);
	newCustomCtpMd->pMdUserApi->Init();
	newCustomCtpMd->mylock->wait();
	newCustomCtpMd->UserLogin();

	return newCustomCtpMd;
}

// instructure
CustomCtpMd::~CustomCtpMd() {
}

int CustomCtpMd::ProcessInstrumentIDList(char *list[], const int l, const char pInstrumentIDList[]) {
	if (pInstrumentIDList[0] == '\0') {
		return 0;
	}

	// split string
	int sum = 0;
	char str[strlen(pInstrumentIDList) + 1];
	strcpy(str, pInstrumentIDList);
	char *begin = str;
	for (int i = 0; str[i] != '\0'; i++) {
		if (str[i] == ' ') {
			str[i] = '\0';
			char *temp = new char[l];
			strcpy(temp, begin);
			list[sum++] = temp;
			begin = str + i + 1;
		}
	}
	char *temp = new char[l];
	strcpy(temp, begin);
	list[sum++] = temp;
	return sum;
}

void CustomCtpMd::Wait() {
	pMdUserApi->Join();
	pMdUserApi->Release();
}


// login user
void CustomCtpMd::UserLogin() {
	// Login
	CThostFtdcReqUserLoginField loginReq;
	memset(&loginReq, 0, sizeof(loginReq));
	strcpy(loginReq.BrokerID, sBrokerID);
	strcpy(loginReq.UserID, sInvesterID);
	strcpy(loginReq.Password, sInvesterPassword);

	int rt = pMdUserApi->ReqUserLogin(&loginReq, nRequestID);
	if (!rt) {
		fprintf(fout, ">>>>> Send user request success\n");
	}
	else {
		fprintf(ferr, "!!!!! Send user request failed\n");
	}
}

// logout user
void CustomCtpMd::UserLogout() {
	CThostFtdcUserLogoutField *logoutUserInfo = new CThostFtdcUserLogoutField();
	strcpy(logoutUserInfo->BrokerID, this->sBrokerID);
	strcpy(logoutUserInfo->UserID, this->sInvesterID);
	pMdUserApi->ReqUserLogout(logoutUserInfo, nRequestID);
}

void CustomCtpMd::SubscribeInstrument(char **ppInstrumentID, int nCount) {
	for (int i = 0; i < nCount; i++) {
		fprintf(fout, "%s\n", ppInstrumentID[i]);
	}
	pMdUserApi->SubscribeMarketData(ppInstrumentID, nCount);
}



void CustomCtpMd::UnsubscribeInstrument(char **ppInstrumentID, int nCount) {
	pMdUserApi->UnSubscribeMarketData(ppInstrumentID, nCount);
}

// undefined
void CustomCtpMd::QuoteInstrument() {
}

// undefined
void CustomCtpMd::UnQuoteInstrument() {
}



/* ----- MdSpi回调函数 ----- */
// show error message , this is inline function
inline void showErrorMessage(CThostFtdcRspInfoField *pRspInfo) 
{
	cerr << "Error code: " << pRspInfo->ErrorID << endl;
	cerr << "Error message: " << pRspInfo->ErrorMsg << endl;
}

// 连接成功应答
void CustomCtpMd::OnFrontConnected()
{
	fprintf(fout, "===== Connection Success =====\n");
	mylock->unlock();
}

// 断开连接通知
void CustomCtpMd::OnFrontDisconnected(int nReason)
{
	cerr << "===== Connection Break off =====" << endl;
	cerr << "Error code: 0x" << std::hex << nReason << endl;
}

// 心跳超时警告
void CustomCtpMd::OnHeartBeatWarning(int nTimeLapse)
{
	cerr << "===== Heart Beat Warning =====" << endl;
	cerr << "The interval connect time: " << nTimeLapse << endl;
}

// 登录应答
void CustomCtpMd::OnRspUserLogin(
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
	}
	else {
		cerr << "===== Login Failed =====" << endl;
		showErrorMessage(pRspInfo);
	}
}

// 登出应答
void CustomCtpMd::OnRspUserLogout(
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
	} else {
		cerr << "===== Login Failed =====" << endl;
		showErrorMessage(pRspInfo);
	}
}

// 错误通知
void CustomCtpMd::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	bool bResult = pRspInfo && (pRspInfo->ErrorID != 0);
	if (bResult) {
		cerr << "===== Something Error =====" << endl;
		showErrorMessage(pRspInfo);
	}
}

// 订阅行情应答
void CustomCtpMd::OnRspSubMarketData(
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
		sprintf(filePath, "%s%s_market_data.csv", dataDirPath, pSpecificInstrument->InstrumentID);

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
void CustomCtpMd::OnRspUnSubMarketData(
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
void CustomCtpMd::OnRspSubForQuoteRsp(
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
void CustomCtpMd::OnRspUnSubForQuoteRsp(
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
void CustomCtpMd::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)
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
	sprintf(filePath, "%s%s_market_data.csv", dataDirPath, pDepthMarketData->InstrumentID);
	
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

	this->CustomMd::OnGetMarketData(parseFrom(*pDepthMarketData));
}

// 询价详情通知
void CustomCtpMd::OnRtnForQuoteRsp(CThostFtdcForQuoteRspField *pForQuoteRsp)
{
	// 部分询价结果
	cout << "===== Get Quote Data =====" << endl;
	cout << "Trading Day: " << pForQuoteRsp->TradingDay << endl;
	cout << "Exchange ID: " << pForQuoteRsp->ExchangeID << endl;
	cout << "Instrument ID: " << pForQuoteRsp->InstrumentID << endl;
	cout << "Quote system ID: " << pForQuoteRsp->ForQuoteSysID << endl;
}