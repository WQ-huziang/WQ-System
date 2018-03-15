#include <iostream>
#include <time.h>
#include <thread>
#include <chrono>
#include <cstring>
#include "td/CustomCtpTrade.h"
#include "adapter/CTPAdapter.h"

using std::cout;
using std::cerr;
using std::endl;

// constructure
CustomCtpTrade* CustomCtpTrade::CreateCustomCtpTrade(
	TThostFtdcInvestorIDType sInvesterID,
	TThostFtdcPasswordType gInvesterPassword,
	TThostFtdcBrokerIDType sBrokerID,
	char dataDirPath[],
	char gTradeFrontAddr[],
	int clientID,
	int serverID)
{
	CustomCtpTrade *newCustomCtpTrade = new CustomCtpTrade(sBrokerID, sInvesterID, gInvesterPassword,
													dataDirPath, clientID, serverID);
    newCustomCtpTrade->nRequestID = 0;
    newCustomCtpTrade->mylock = new Semaphore(true);

	newCustomCtpTrade->pTradeUserApi = CThostFtdcTraderApi::CreateFtdcTraderApi(dataDirPath);
	newCustomCtpTrade->pTradeUserApi->RegisterSpi(newCustomCtpTrade);
	newCustomCtpTrade->pTradeUserApi->RegisterFront(gTradeFrontAddr);
	newCustomCtpTrade->pTradeUserApi->Init();
	newCustomCtpTrade->mylock->wait();

	return newCustomCtpTrade;
}

CustomCtpTrade::~CustomCtpTrade() {
	pTradeUserApi->Join();
	pTradeUserApi->Release();
}

void CustomCtpTrade::UserLogin() {
	// Login
	CThostFtdcReqUserLoginField loginReq;
	memset(&loginReq, 0, sizeof(loginReq));
	strcpy(loginReq.BrokerID, sBrokerID);
	strcpy(loginReq.UserID, sInvesterID);
	strcpy(loginReq.Password, sInvesterPassword);
	int rt = pTradeUserApi->ReqUserLogin(&loginReq, nRequestID);
	if (!rt) {
		fprintf(fout, ">>>>> Send user login request success\n");
	}
	else {
		fprintf(ferr, "!!!!! Send user login request failed\n");
	}
}

void CustomCtpTrade::UserLogout() {
	// Logout
	CThostFtdcUserLogoutField logoutReq;
	memset(&logoutReq, 0, sizeof(logoutReq));
	strcpy(logoutReq.BrokerID, sBrokerID);
	strcpy(logoutReq.UserID, sInvesterID);
	int rt = pTradeUserApi->ReqUserLogout(&logoutReq, nRequestID);
	if (!rt) {
		cout << ">>>>> Send user logout request success" << endl;
	} else {
		cerr << "!!!!! Send user logout request success" << endl;
	}
}

void CustomCtpTrade::SettlementConfirm() {
	CThostFtdcSettlementInfoConfirmField settlementInfoConfirm;
	memset(&settlementInfoConfirm, 0, sizeof(settlementInfoConfirm));
	strcpy(settlementInfoConfirm.BrokerID, sBrokerID);
	strcpy(settlementInfoConfirm.InvestorID, sInvesterID);
	int ret = pTradeUserApi->ReqSettlementInfoConfirm(&settlementInfoConfirm, nRequestID);
	if (!ret) {
		fprintf(fout, ">>>>> Send settle confirm request success\n");	
	} else {
		fprintf(ferr, "!!!!! Send settle confirm request success\n");
	}
}

void CustomCtpTrade::OrderInsert(WZInputOrderField &order) 
{
	cout << order.InstrumentID << endl;
	CThostFtdcInputOrderField orderInsertReq = parseTo(order);
	///报单引用
	strcpy(orderInsertReq.OrderRef, orderRef);

	int rt = pTradeUserApi->ReqOrderInsert(&orderInsertReq, ++nRequestID);
	if (!rt) {
		cout << ">>>>> Send order insert request success" << endl;
	}
	else {
		cerr << "!!!!! Send order insert request failed" << endl;
	}
}

void CustomCtpTrade::OrderWithdraw(WZInputOrderField &order) {
	CThostFtdcInputOrderActionField orderActionReq = {};
	///报单引用
	strcpy(orderActionReq.OrderRef, orderRef);
	///前置编号
	orderActionReq.FrontID = tradeFrontID;
	///会话编号
	orderActionReq.SessionID = sessionID;

	int rt = pTradeUserApi->ReqOrderAction(&orderActionReq, ++nRequestID);
	if (!rt) {
		cout << ">>>>> Send order withdraw request success" << endl;
	}
	else {
		cerr << "!!!!! Send order withdraw request failed" << endl;
	}
}


/* ----- TradeSpi回调函数 ----- */
inline void showErrorMessage(CThostFtdcRspInfoField *pRspInfo) 
{
	cerr << "Error code: " << pRspInfo->ErrorID << endl;
	cerr << "Error message: " << pRspInfo->ErrorMsg << endl;
}

bool CustomCtpTrade::isMyOrder(CThostFtdcOrderField *pOrder)
{
	return ((pOrder->FrontID == tradeFrontID) &&
		(pOrder->SessionID == sessionID) &&
		(strcmp(pOrder->OrderRef, orderRef) == 0));
}

bool CustomCtpTrade::isTradingOrder(CThostFtdcOrderField *pOrder)
{
	return ((pOrder->OrderStatus != THOST_FTDC_OST_PartTradedNotQueueing) &&
		(pOrder->OrderStatus != THOST_FTDC_OST_Canceled) &&
		(pOrder->OrderStatus != THOST_FTDC_OST_AllTraded));
}

void CustomCtpTrade::OnFrontConnected()
{
	fprintf(fout, "===== Connection Success =====\n");
	UserLogin();

}

void CustomCtpTrade::OnRspUserLogin(
	CThostFtdcRspUserLoginField *pRspUserLogin,
	CThostFtdcRspInfoField *pRspInfo,
	int nRequestID,
	bool bIsLast)
{
	bool bResult = pRspInfo && (pRspInfo->ErrorID != 0);
	if (!bResult) {
		fprintf(fout, "===== Login Success =====\n");
		fprintf(fout, "===== User Information as Follow =====\n");
		cout << "Tradeding day: " << pRspUserLogin->TradingDay << endl;
		cout << "Login time: " << pRspUserLogin->LoginTime << endl;
		cout << "Broker ID: " << pRspUserLogin->BrokerID << endl;
		cout << "User ID: " << pRspUserLogin->UserID << endl;
		// 保存会话参数
		tradeFrontID = pRspUserLogin->FrontID;
		sessionID = pRspUserLogin->SessionID;
		strcpy(orderRef, pRspUserLogin->MaxOrderRef);

		SettlementConfirm();
	} else {
		fprintf(ferr, "===== Login failed =====\n");
		showErrorMessage(pRspInfo);
	}
}

void CustomCtpTrade::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	bool bResult = pRspInfo && (pRspInfo->ErrorID != 0);
	if (bResult) {
		fprintf(ferr, "===== Something Error =====\n");
		showErrorMessage(pRspInfo);
	}
}

void CustomCtpTrade::OnFrontDisconnected(int nReason)
{
	cerr << "===== Connection Break off =====" << endl;
	cerr << "Error code: 0x" << std::hex << nReason << endl;
}

void CustomCtpTrade::OnHeartBeatWarning(int nTimeLapse)
{
	cerr << "===== Heart Beat Warning =====" << endl;
	cerr << "The interval connect time: " << nTimeLapse << endl;
}

void CustomCtpTrade::OnRspUserLogout(
	CThostFtdcUserLogoutField *pUserLogout,
	CThostFtdcRspInfoField *pRspInfo,
	int nRequestID,
	bool bIsLast)
{
	bool bResult = pRspInfo && (pRspInfo->ErrorID != 0);
	if (!bResult) {
		cout << "===== User Logout Success =====" << endl;
		cout << "===== User Information as Follow =====" << endl;
		cout << "Broker ID: " << pUserLogout->BrokerID << endl;
		cout << "User ID: " << pUserLogout->UserID << endl;
	} else {
		fprintf(ferr, "===== Logout Failed =====\n");
		showErrorMessage(pRspInfo);
	}
}


void CustomCtpTrade::OnRspOrderInsert(
	CThostFtdcInputOrderField *pInputOrder, 
	CThostFtdcRspInfoField *pRspInfo,
	int nRequestID,
	bool bIsLast)
{
	bool bResult = pRspInfo && (pRspInfo->ErrorID != 0);
	if (!bResult) {
		cout << "===== Order Insert Success =====" << endl;
		cout << "Instrument ID: " << pInputOrder->InstrumentID << endl;
		cout << "Price: " << pInputOrder->LimitPrice << endl;
		cout << "Volumn: " << pInputOrder->VolumeTotalOriginal << endl;
		cout << "Buy or sell: " << pInputOrder->Direction << endl;
	} else {
		fprintf(ferr, "===== Order Insert Failed =====\n");
		showErrorMessage(pRspInfo);
	}
}

void CustomCtpTrade::OnRspOrderAction(
	CThostFtdcInputOrderActionField *pInputOrderAction,
	CThostFtdcRspInfoField *pRspInfo,
	int nRequestID,
	bool bIsLast)
{
	bool bResult = pRspInfo && (pRspInfo->ErrorID != 0);
	if (!bResult) {
		cout << "===== Order Action Success =====" << endl;
		cout << "Instrument ID: " << pInputOrderAction->InstrumentID << endl;
		cout << "Operator flag: " << pInputOrderAction->ActionFlag;
	} else {
		fprintf(ferr, "===== Order Action Failed =====\n");
		showErrorMessage(pRspInfo);
	}
}

void CustomCtpTrade::OnRtnOrder(CThostFtdcOrderField *pOrder)
{
	fprintf(fout, "===== Get Order Response =====\n");

	if (isMyOrder(pOrder)) {
		if (isTradingOrder(pOrder)) {
			fprintf(fout, ">>>>> Your order is trading!\n");
		} else if (pOrder->OrderStatus == THOST_FTDC_OST_Canceled) {
			fprintf(fout, ">>>>> Your order is withdraw successfully!\n");
		}
	}
}

void CustomCtpTrade::OnRtnTrade(CThostFtdcTradeField *pTrade)
{
	cout << "===== Order Trade Successfully =====" << endl;
	cout << "Trade time: " << pTrade->TradeTime << endl;
	cout << "Instrument ID: " << pTrade->InstrumentID << endl;
	cout << "Price: " << pTrade->Price << endl;
	cout << "Volumn: " << pTrade->Volume << endl;
	cout << "Open or close: " << pTrade->Direction << endl;
}

void CustomCtpTrade::OnRspSettlementInfoConfirm(
	CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm,
	CThostFtdcRspInfoField *pRspInfo,
	int nRequestID,
	bool bIsLast)
{
	bool bResult = pRspInfo && (pRspInfo->ErrorID != 0);
	if (!bResult) {
		cout << "===== Settlement Confirm Success =====" << endl;
		cout << "Confirm Date: " << pSettlementInfoConfirm->ConfirmDate << endl;
		cout << "Confirm Time: " << pSettlementInfoConfirm->ConfirmTime << endl;
		mylock->unlock();
	} else {
		fprintf(ferr, "===== Settlement Confirm Failed =====\n");
		showErrorMessage(pRspInfo);
	}
}












// void CustomCtpTradeSpi::reqSettlementInfoConfirm()
// {
// 	CThostFtdcSettlementInfoConfirmField settlementConfirmReq;
// 	memset(&settlementConfirmReq, 0, sizeof(settlementConfirmReq));
// 	strcpy(settlementConfirmReq.BrokerID, sBrokerID);
// 	strcpy(settlementConfirmReq.InvestorID, sInvesterID);
// 	static int requestID = 0; // 请求编号
// 	int rt = g_pTradeUserApi->ReqSettlementInfoConfirm(&settlementConfirmReq, requestID);
// 	if (!rt)
// 		cout << ">>>>>>发送投资者结算结果确认请求成功" << endl;
// 	else
// 		cerr << "--->>>发送投资者结算结果确认请求失败" << endl;
// }

// void CustomCtpTradeSpi::reqQueryInstrument()
// {
// 	CThostFtdcQryInstrumentField instrumentReq;
// 	memset(&instrumentReq, 0, sizeof(instrumentReq));
// 	strcpy(instrumentReq.InstrumentID, g_pTradeInstrumentID);
// 	static int requestID = 0; // 请求编号
// 	int rt = g_pTradeUserApi->ReqQryInstrument(&instrumentReq, requestID);
// 	if (!rt)
// 		cout << ">>>>>>发送合约查询请求成功" << endl;
// 	else
// 		cerr << "--->>>发送合约查询请求失败" << endl;
// }

// void CustomCtpTradeSpi::reqQueryTradingAccount()
// {
// 	CThostFtdcQryTradingAccountField tradingAccountReq;
// 	memset(&tradingAccountReq, 0, sizeof(tradingAccountReq));
// 	strcpy(tradingAccountReq.BrokerID, sBrokerID);
// 	strcpy(tradingAccountReq.InvestorID, sInvesterID);
// 	static int requestID = 0; // 请求编号
// 	std::this_thread::sleep_for(std::chrono::milliseconds(700)); // 有时候需要停顿一会才能查询成功
// 	int rt = g_pTradeUserApi->ReqQryTradingAccount(&tradingAccountReq, requestID);
// 	if (!rt)
// 		cout << ">>>>>>发送投资者资金账户查询请求成功" << endl;
// 	else
// 		cerr << "--->>>发送投资者资金账户查询请求失败" << endl;
// }

// void CustomCtpTradeSpi::reqQueryInvestorPosition()
// {
// 	CThostFtdcQryInvestorPositionField postionReq;
// 	memset(&postionReq, 0, sizeof(postionReq));
// 	strcpy(postionReq.BrokerID, sBrokerID);
// 	strcpy(postionReq.InvestorID, sInvesterID);
// 	strcpy(postionReq.InstrumentID, g_pTradeInstrumentID);
// 	static int requestID = 0; // 请求编号
// 	std::this_thread::sleep_for(std::chrono::milliseconds(700)); // 有时候需要停顿一会才能查询成功
// 	int rt = g_pTradeUserApi->ReqQryInvestorPosition(&postionReq, requestID);
// 	if (!rt)
// 		cout << ">>>>>>发送投资者持仓查询请求成功" << endl;
// 	else
// 		cerr << "--->>>发送投资者持仓查询请求失败" << endl;
// }

// void CustomCtpTradeSpi::reqOrderInsert()
// {
// 	CThostFtdcInputOrderField orderInsertReq;
// 	memset(&orderInsertReq, 0, sizeof(orderInsertReq));
// 	///经纪公司代码
// 	strcpy(orderInsertReq.BrokerID, sBrokerID);
// 	///投资者代码
// 	strcpy(orderInsertReq.InvestorID, sInvesterID);
// 	///合约代码
// 	strcpy(orderInsertReq.InstrumentID, g_pTradeInstrumentID);
// 	///报单引用
// 	strcpy(orderInsertReq.OrderRef, order_ref);
// 	///报单价格条件: 限价
// 	orderInsertReq.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
// 	///买卖方向: 
// 	orderInsertReq.Direction = gTradeDirection;
// 	///组合开平标志: 开仓
// 	orderInsertReq.CombOffsetFlag[0] = THOST_FTDC_OF_Open;
// 	///组合投机套保标志
// 	orderInsertReq.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;
// 	///价格
// 	orderInsertReq.LimitPrice = gLimitPrice;
// 	///数量：1
// 	orderInsertReq.VolumeTotalOriginal = 1;
// 	///有效期类型: 当日有效
// 	orderInsertReq.TimeCondition = THOST_FTDC_TC_GFD;
// 	///成交量类型: 任何数量
// 	orderInsertReq.VolumeCondition = THOST_FTDC_VC_AV;
// 	///最小成交量: 1
// 	orderInsertReq.MinVolume = 1;
// 	///触发条件: 立即
// 	orderInsertReq.ContingentCondition = THOST_FTDC_CC_Immediately;
// 	///强平原因: 非强平
// 	orderInsertReq.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
// 	///自动挂起标志: 否
// 	orderInsertReq.IsAutoSuspend = 0;
// 	///用户强评标志: 否
// 	orderInsertReq.UserForceClose = 0;

// 	static int requestID = 0; // 请求编号
// 	int rt = g_pTradeUserApi->ReqOrderInsert(&orderInsertReq, ++requestID);
// 	if (!rt)
// 		cout << ">>>>>>发送报单录入请求成功" << endl;
// 	else
// 		cerr << "--->>>发送报单录入请求失败" << endl;
// }

// void CustomCtpTradeSpi::reqOrderInsert(
// 	TThostFtdcInstrumentIDType instrumentID,
// 	TThostFtdcPriceType price,
// 	TThostFtdcVolumeType volume,
// 	TThostFtdcDirectionType direction)
// {
// 	CThostFtdcInputOrderField orderInsertReq;
// 	memset(&orderInsertReq, 0, sizeof(orderInsertReq));
// 	///经纪公司代码
// 	strcpy(orderInsertReq.BrokerID, sBrokerID);
// 	///投资者代码
// 	strcpy(orderInsertReq.InvestorID, sInvesterID);
// 	///合约代码
// 	strcpy(orderInsertReq.InstrumentID, instrumentID);
// 	///报单引用
// 	strcpy(orderInsertReq.OrderRef, order_ref);
// 	///报单价格条件: 限价
// 	orderInsertReq.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
// 	///买卖方向: 
// 	orderInsertReq.Direction = direction;
// 	///组合开平标志: 开仓
// 	orderInsertReq.CombOffsetFlag[0] = THOST_FTDC_OF_Open;
// 	///组合投机套保标志
// 	orderInsertReq.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;
// 	///价格
// 	orderInsertReq.LimitPrice = price;
// 	///数量：1
// 	orderInsertReq.VolumeTotalOriginal = volume;
// 	///有效期类型: 当日有效
// 	orderInsertReq.TimeCondition = THOST_FTDC_TC_GFD;
// 	///成交量类型: 任何数量
// 	orderInsertReq.VolumeCondition = THOST_FTDC_VC_AV;
// 	///最小成交量: 1
// 	orderInsertReq.MinVolume = 1;
// 	///触发条件: 立即
// 	orderInsertReq.ContingentCondition = THOST_FTDC_CC_Immediately;
// 	///强平原因: 非强平
// 	orderInsertReq.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
// 	///自动挂起标志: 否
// 	orderInsertReq.IsAutoSuspend = 0;
// 	///用户强评标志: 否
// 	orderInsertReq.UserForceClose = 0;

// 	static int requestID = 0; // 请求编号
// 	int rt = g_pTradeUserApi->ReqOrderInsert(&orderInsertReq, ++requestID);
// 	if (!rt)
// 		cout << ">>>>>>发送报单录入请求成功" << endl;
// 	else
// 		cerr << "--->>>发送报单录入请求失败" << endl;
// }

// void CustomCtpTradeSpi::reqOrderAction(CThostFtdcOrderField *pOrder)
// {
// 	static bool orderActionSentFlag = false; // 是否发送了报单
// 	if (orderActionSentFlag)
// 		return;

// 	CThostFtdcInputOrderActionField orderActionReq;
// 	memset(&orderActionReq, 0, sizeof(orderActionReq));
// 	///经纪公司代码
// 	strcpy(orderActionReq.BrokerID, pOrder->BrokerID);
// 	///投资者代码
// 	strcpy(orderActionReq.InvestorID, pOrder->InvestorID);
// 	///报单操作引用
// 	//	TThostFtdcOrderActionRefType	OrderActionRef;
// 	///报单引用
// 	strcpy(orderActionReq.OrderRef, pOrder->OrderRef);
// 	///请求编号
// 	//	TThostFtdcRequestIDType	RequestID;
// 	///前置编号
// 	orderActionReq.FrontID = trade_front_id;
// 	///会话编号
// 	orderActionReq.SessionID = session_id;
// 	///交易所代码
// 	//	TThostFtdcExchangeIDType	ExchangeID;
// 	///报单编号
// 	//	TThostFtdcOrderSysIDType	OrderSysID;
// 	///操作标志
// 	orderActionReq.ActionFlag = THOST_FTDC_AF_Delete;
// 	///价格
// 	//	TThostFtdcPriceType	LimitPrice;
// 	///数量变化
// 	//	TThostFtdcVolumeType	VolumeChange;
// 	///用户代码
// 	//	TThostFtdcUserIDType	UserID;
// 	///合约代码
// 	strcpy(orderActionReq.InstrumentID, pOrder->InstrumentID);
// 	static int requestID = 0; // 请求编号
// 	int rt = g_pTradeUserApi->ReqOrderAction(&orderActionReq, ++requestID);
// 	if (!rt)
// 		cout << ">>>>>>发送报单操作请求成功" << endl;
// 	else
// 		cerr << "--->>>发送报单操作请求失败" << endl;
// 	orderActionSentFlag = true;
// }


// void CustomCtpTradeSpi::OnRspQryInstrument(
// 	CThostFtdcInstrumentField *pInstrument,
// 	CThostFtdcRspInfoField *pRspInfo,
// 	int nRequestID,
// 	bool bIsLast)
// {
// 	if (!isErrorRspInfo(pRspInfo))
// 	{
// 		cout << "=====查询合约结果成功=====" << endl;
// 		cout << "交易所代码： " << pInstrument->ExchangeID << endl;
// 		cout << "合约代码： " << pInstrument->InstrumentID << endl;
// 		cout << "合约在交易所的代码： " << pInstrument->ExchangeInstID << endl;
// 		cout << "执行价： " << pInstrument->StrikePrice << endl;
// 		cout << "到期日： " << pInstrument->EndDelivDate << endl;
// 		cout << "当前交易状态： " << pInstrument->IsTrading << endl;
// 		// 请求查询投资者资金账户
// 		reqQueryTradingAccount();
// 	}
// }

// void CustomCtpTradeSpi::OnRspQryTradingAccount(
// 	CThostFtdcTradingAccountField *pTradingAccount,
// 	CThostFtdcRspInfoField *pRspInfo,
// 	int nRequestID,
// 	bool bIsLast)
// {
// 	if (!isErrorRspInfo(pRspInfo))
// 	{
// 		cout << "=====查询投资者资金账户成功=====" << endl;
// 		cout << "投资者账号： " << pTradingAccount->AccountID << endl;
// 		cout << "可用资金： " << pTradingAccount->Available << endl;
// 		cout << "可取资金： " << pTradingAccount->WithdrawQuota << endl;
// 		cout << "当前保证金: " << pTradingAccount->CurrMargin << endl;
// 		cout << "平仓盈亏： " << pTradingAccount->CloseProfit << endl;
// 		// 请求查询投资者持仓
// 		reqQueryInvestorPosition();
// 	}
// }

// void CustomCtpTradeSpi::OnRspQryInvestorPosition(
// 	CThostFtdcInvestorPositionField *pInvestorPosition,
// 	CThostFtdcRspInfoField *pRspInfo,
// 	int nRequestID,
// 	bool bIsLast)
// {
// 	if (!isErrorRspInfo(pRspInfo))
// 	{
// 		cout << "=====查询投资者持仓成功=====" << endl;
// 		if (pInvestorPosition)
// 		{
// 			cout << "合约代码： " << pInvestorPosition->InstrumentID << endl;
// 			cout << "开仓价格： " << pInvestorPosition->OpenAmount << endl;
// 			cout << "开仓量： " << pInvestorPosition->OpenVolume << endl;
// 			cout << "开仓方向： " << pInvestorPosition->PosiDirection << endl;
// 			cout << "占用保证金：" << pInvestorPosition->UseMargin << endl;
// 		}
// 		else
// 			cout << "----->该合约未持仓" << endl;
		
// 		// 报单录入请求（这里是一部接口，此处是按顺序执行）
// 		/*if (loginFlag)
// 			reqOrderInsert();*/
// 		//if (loginFlag)
// 		//	reqOrderInsertWithParams(g_pTradeInstrumentID, gLimitPrice, 1, gTradeDirection); // 自定义一笔交易

// 		// 策略交易
// 		cout << "=====开始进入策略交易=====" << endl;
// 		while (loginFlag)
// 			StrategyCheckAndTrade(g_pTradeInstrumentID, this);
// 	}
// }
