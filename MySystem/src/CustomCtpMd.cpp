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



/* ----- MdSpi�ص����� ----- */
// show error message , this is inline function
inline void showErrorMessage(CThostFtdcRspInfoField *pRspInfo) 
{
	cerr << "Error code: " << pRspInfo->ErrorID << endl;
	cerr << "Error message: " << pRspInfo->ErrorMsg << endl;
}

// ���ӳɹ�Ӧ��
void CustomCtpMd::OnFrontConnected()
{
	fprintf(fout, "===== Connection Success =====\n");
	mylock->unlock();
}

// �Ͽ�����֪ͨ
void CustomCtpMd::OnFrontDisconnected(int nReason)
{
	cerr << "===== Connection Break off =====" << endl;
	cerr << "Error code: 0x" << std::hex << nReason << endl;
}

// ������ʱ����
void CustomCtpMd::OnHeartBeatWarning(int nTimeLapse)
{
	cerr << "===== Heart Beat Warning =====" << endl;
	cerr << "The interval connect time: " << nTimeLapse << endl;
}

// ��¼Ӧ��
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

// �ǳ�Ӧ��
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

// ����֪ͨ
void CustomCtpMd::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	bool bResult = pRspInfo && (pRspInfo->ErrorID != 0);
	if (bResult) {
		cerr << "===== Something Error =====" << endl;
		showErrorMessage(pRspInfo);
	}
}

// ��������Ӧ��
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
		// �����Ҫ�����ļ��������ݿ⣬�����ﴴ����ͷ,��ͬ�ĺ�Լ�����洢
		char filePath[100];
		sprintf(filePath, "%s%s_market_data.csv", dataDirPath, pSpecificInstrument->InstrumentID);

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

// ����ѯ��Ӧ��
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

// ȡ������ѯ��Ӧ��
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
// ��������֪ͨ
void CustomCtpMd::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)
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

// ѯ������֪ͨ
void CustomCtpMd::OnRtnForQuoteRsp(CThostFtdcForQuoteRspField *pForQuoteRsp)
{
	// ����ѯ�۽��
	cout << "===== Get Quote Data =====" << endl;
	cout << "Trading Day: " << pForQuoteRsp->TradingDay << endl;
	cout << "Exchange ID: " << pForQuoteRsp->ExchangeID << endl;
	cout << "Instrument ID: " << pForQuoteRsp->InstrumentID << endl;
	cout << "Quote system ID: " << pForQuoteRsp->ForQuoteSysID << endl;
}