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
extern TThostFtdcBrokerIDType gBrokerID;         // ģ�⾭���̴���
extern TThostFtdcInvestorIDType gInvesterID;     // Ͷ�����˻���
extern TThostFtdcPasswordType gInvesterPassword; // Ͷ��������

extern CThostFtdcMdApi *gp_MdUserApi;            // ����ָ��
extern char gMdFrontAddr[50];                    // ģ������ǰ�õ�ַ
extern char *gp_InstrumentID[];                  // �����Լ�����б��С��ϡ���֣��������ѡһ��
extern int instrumentNum;                        // �����Լ��������

// extern std::unordered_map<std::string, TickToKlineHelper> g_KlineHash; // k�ߴ洢��

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



/* ----- MdSpi�ص����� ----- */
// show error message , this is inline function
inline void showErrorMessage(CThostFtdcRspInfoField *pRspInfo) 
{
	cerr << "Error code: " << pRspInfo->ErrorID << endl;
	cerr << "Error message: " << pRspInfo->ErrorMsg << endl;
}

// ���ӳɹ�Ӧ��
void CustomMd::OnFrontConnected()
{
	cout << "===== Connection Success =====" << endl;

	// ��ʼ��¼
	CThostFtdcReqUserLoginField loginReq;
	memset(&loginReq, 0, sizeof(loginReq));
	strcpy(loginReq.BrokerID, gBrokerID);
	strcpy(loginReq.UserID, gInvesterID);
	strcpy(loginReq.Password, gInvesterPassword);
	static int requestID = 0; // ������

	int rt = gp_MdUserApi->ReqUserLogin(&loginReq, requestID);
	if (!rt)
		cout << ">>>>> Send user request success" << endl;
	else
		cerr << "!!!!! Send user request failed" << endl;
}

// �Ͽ�����֪ͨ
void CustomMd::OnFrontDisconnected(int nReason)
{
	cerr << "===== Connection Break off =====" << endl;
	cerr << "Error code: 0x" << std::hex << nReason << endl;
}

// ������ʱ����
void CustomMd::OnHeartBeatWarning(int nTimeLapse)
{
	cerr << "===== Heart Beat Warning =====" << endl;
	cerr << "The interval connect time�� " << nTimeLapse << endl;
}

// ��¼Ӧ��
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
		
		// ��ʼ��������
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

// �ǳ�Ӧ��
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

// ����֪ͨ
void CustomMd::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	bool bResult = pRspInfo && (pRspInfo->ErrorID != 0);
	if (bResult) {
		cerr << "===== Something Error =====" << endl;
		showErrorMessage(pRspInfo);
	}
}

// ��������Ӧ��
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
		// �����Ҫ�����ļ��������ݿ⣬�����ﴴ����ͷ,��ͬ�ĺ�Լ�����洢
		char filePath[100];
		strncpy(filePath, dataDicPath, 100);
		sprintf(filePath, "%s_market_data.csv", pSpecificInstrument->InstrumentID);
		if ( (access(filePath, W_OK)) == -1 ) 
		{
			FILE* fd = fopen(filePath, "w");
			fprintf(fd, "��Լ����,"
				        "����ʱ��,"
				        "���¼�,"
				        "�ɽ���,"
				        "���һ,"
				        "����һ,"
				        "����һ,"
				        "����һ,"
				        "�ֲ���,"
				        "������\n");
			fclose(fd);
		}
	}
	else {
		cerr << "===== Subscribe Market Data Failed =====" << endl;
		showErrorMessage(pRspInfo);
	}
}

// ȡ����������Ӧ��
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

// ����ѯ��Ӧ��
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

// ȡ������ѯ��Ӧ��
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
// ��������֪ͨ
void CustomMd::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)
{
	// ��ӡ���飬�ֶν϶࣬��ȡ����
	cout << "===== Get Depth Market Data =====" << endl;
	cout << "Trading Day: " << pDepthMarketData->TradingDay << endl;
	cout << "Exchange ID: " << pDepthMarketData->ExchangeID << endl;
	cout << "Instrument ID: " << pDepthMarketData->InstrumentID << endl;
	cout << "Last Price: " << pDepthMarketData->LastPrice << endl;
	cout << "Volume: " << pDepthMarketData->Volume << endl;

	// ���ֻ��ȡĳһ����Լ���飬������tick�ش����ļ������ݿ�
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

// ѯ������֪ͨ
void CustomMd::OnRtnForQuoteRsp(CThostFtdcForQuoteRspField *pForQuoteRsp)
{
	// ����ѯ�۽��
	cout << "===== Get Quote Data =====" << endl;
	cout << "Trading Day: " << pForQuoteRsp->TradingDay << endl;
	cout << "Exchange ID: " << pForQuoteRsp->ExchangeID << endl;
	cout << "Instrument ID: " << pForQuoteRsp->InstrumentID << endl;
	cout << "Quote system ID: " << pForQuoteRsp->ForQuoteSysID << endl;
}