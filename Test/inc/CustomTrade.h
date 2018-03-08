#pragma once
// ---- �����Ľ����� ---- //
#include "ThostFtdcTraderApi.h"
#include "CustomUser.h"
#include "CustomSemaphore.h"

class CustomTrade : public CThostFtdcTraderSpi, public CustomUser, public Semaphore
{
private:
	TThostFtdcFrontIDType tradeFrontID;	      // front ID, return by server
	TThostFtdcSessionIDType	sessionID;	      // session ID, return by server
	TThostFtdcOrderRefType orderRef;          // max order reference, return by server

	bool isParallel;                          // parallel or not
	int nRequestID;                           // request ID
	bool isLogin;                             // user has login or not

	CThostFtdcTraderApi *pTradeUserApi;       // Td api

/* ----- Custom Function ----- */
public:
	/// constructure function
	/// dataDirPath must be absolute
	static CustomTrade* CreateCustomTrade(
		TThostFtdcInvestorIDType gInvesterID,
		TThostFtdcPasswordType gInvesterPassword,
		char dataDirPath[] = "/home/huziang/Desktop/api/Test/data/Td",
		bool isParallel = false,
		TThostFtdcBrokerIDType gBrokerID = "9999", 
		char gMdFrontAddr[] = "tcp://180.168.146.187:10001");

	/// constructure function
	CustomTrade(
		TThostFtdcBrokerIDType gBrokerID,
		TThostFtdcInvestorIDType gInvesterID,
		TThostFtdcPasswordType gInvesterPassword,
		char *dataDirPath) 
		: CustomUser(gBrokerID, gInvesterID, gInvesterPassword, dataDirPath) {}

	~CustomTrade();

	void userLogin();

	void userLogout();

	/// insert order
	/// @param direction the insert ways about order
	///            THOST_FTDC_D_Buy : buy
	///            THOST_FTDC_D_Sell : sell
	void orderInsert(
		TThostFtdcInstrumentIDType instrumentID,
		TThostFtdcPriceType price,
		TThostFtdcVolumeType volume,
		TThostFtdcDirectionType direction);

	/// withdraw order
	void orderWithdraw(TThostFtdcInstrumentIDType instrumentID);
	
	/// judge whether the order is my own order or not 
	bool isMyOrder(CThostFtdcOrderField *pOrder);

	/// judge whether the order is trading order or not 
	bool isTradingOrder(CThostFtdcOrderField *pOrder);

/* ----- Overlap the TdSpi ----- */
private:
	/// deal with error response info
	bool isErrorRspInfo(CThostFtdcRspInfoField *pRspInfo);

public:
	///���ͻ����뽻�׺�̨������ͨ������ʱ����δ��¼ǰ�����÷��������á�
	void OnFrontConnected();

	///��¼������Ӧ
	void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///����Ӧ��
	void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///���ͻ����뽻�׺�̨ͨ�����ӶϿ�ʱ���÷��������á���������������API���Զ��������ӣ��ͻ��˿ɲ�������
	void OnFrontDisconnected(int nReason);

	///������ʱ���档����ʱ��δ�յ�����ʱ���÷��������á�
	void OnHeartBeatWarning(int nTimeLapse);

	///�ǳ�������Ӧ
	void OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///Ͷ���߽�����ȷ����Ӧ
	//void OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�����ѯ��Լ��Ӧ
	//void OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�����ѯ�ʽ��˻���Ӧ
	//void OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�����ѯͶ���ֲ߳���Ӧ
	//void OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///����¼��������Ӧ
	void OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///��������������Ӧ
	void OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///����֪ͨ
	void OnRtnOrder(CThostFtdcOrderField *pOrder);

	///�ɽ�֪ͨ
	void OnRtnTrade(CThostFtdcTradeField *pTrade);
};