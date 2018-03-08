#pragma once
// ---- 派生的交易类 ---- //
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
	///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
	void OnFrontConnected();

	///登录请求响应
	void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///错误应答
	void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
	void OnFrontDisconnected(int nReason);

	///心跳超时警告。当长时间未收到报文时，该方法被调用。
	void OnHeartBeatWarning(int nTimeLapse);

	///登出请求响应
	void OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///投资者结算结果确认响应
	//void OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///请求查询合约响应
	//void OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///请求查询资金账户响应
	//void OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///请求查询投资者持仓响应
	//void OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///报单录入请求响应
	void OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///报单操作请求响应
	void OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///报单通知
	void OnRtnOrder(CThostFtdcOrderField *pOrder);

	///成交通知
	void OnRtnTrade(CThostFtdcTradeField *pTrade);
};