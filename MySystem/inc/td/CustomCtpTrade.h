#pragma once

// ---- �����Ľ����� ---- //
#include "api/ctp/ThostFtdcTraderApi.h"
#include "util/CustomUser.h"
#include "util/CustomSemaphore.h"
#include "myapi/CustomTrade.h"

class CustomCtpTrade : public CThostFtdcTraderSpi, public CustomUser, public CustomTrade
{
private:
	TThostFtdcFrontIDType tradeFrontID;	      // front ID, return by server
	TThostFtdcSessionIDType	sessionID;	      // session ID, return by server
	TThostFtdcOrderRefType orderRef;          // max order reference, return by server

	Semaphore *mylock;
	int nRequestID;                           // request ID

	CThostFtdcTraderApi *pTradeUserApi;       // Td api

/* ----- Custom Function ----- */
public:
	/// constructure function
	/// dataDirPath must be absolute
	static CustomCtpTrade* CreateCustomCtpTrade(
		TThostFtdcBrokerIDType gBrokerID,
		TThostFtdcInvestorIDType gInvesterID,
		TThostFtdcPasswordType gInvesterPassword,
		char dataDirPath[],
		char gMdFrontAddr[],
		int clientID,
		int serverID);

	/// constructure function
	CustomCtpTrade(
		TThostFtdcBrokerIDType gBrokerID,
		TThostFtdcInvestorIDType gInvesterID,
		TThostFtdcPasswordType gInvesterPassword,
		char *dataDirPath,
		int clientID,
		int serverID)
		: CustomUser(gBrokerID, gInvesterID, gInvesterPassword, dataDirPath), CustomTrade(clientID, serverID) {}

	~CustomCtpTrade();

	void UserLogin();

	void UserLogout();

	/// insert order
	void OrderInsert(WZInputOrderField &order);

	/// withdraw order
	void OrderWithdraw(WZInputOrderField &order);
	
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