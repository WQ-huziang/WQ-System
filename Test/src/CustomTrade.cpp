#include <iostream>
#include <time.h>
#include <thread>
#include <chrono>
#include <cstring>
#include "CustomTrade.h"

using std::cout;
using std::cerr;
using std::endl;

// constructure
CustomTrade* CustomTrade::CreateCustomTrade(
	TThostFtdcInvestorIDType sInvesterID,
	TThostFtdcPasswordType gInvesterPassword,
	char dataDirPath[],
	bool isParallel,
	TThostFtdcBrokerIDType sBrokerID, 
	char gTradeFrontAddr[])
{
	CustomTrade *newCustomTrade = new CustomTrade(sBrokerID, sInvesterID, gInvesterPassword, dataDirPath);
	newCustomTrade->isParallel = isParallel;
    newCustomTrade->nRequestID = 0;
    newCustomTrade->isLogin = false;

	newCustomTrade->pTradeUserApi = CThostFtdcTraderApi::CreateFtdcTraderApi(dataDirPath);
	newCustomTrade->pTradeUserApi->RegisterSpi(newCustomTrade);
	newCustomTrade->pTradeUserApi->RegisterFront(gTradeFrontAddr);
	newCustomTrade->pTradeUserApi->Init();

	return newCustomTrade;
}

CustomTrade::~CustomTrade() {
	pTradeUserApi->Join();
	pTradeUserApi->Release();
}

void CustomTrade::userLogin() {
	if (!isParallel) {
		wait();
	}

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

void CustomTrade::userLogout() {
	if (!isParallel) {
		wait();
	}

	// Logout
	CThostFtdcUserLogoutField logoutReq;
	memset(&logoutReq, 0, sizeof(logoutReq));
	strcpy(logoutReq.BrokerID, sBrokerID);
	strcpy(logoutReq.UserID, sInvesterID);
	int rt = pTradeUserApi->ReqUserLogout(&logoutReq, nRequestID);
	if (!rt) {
		cout << ">>>>>> Send user logout request success" << endl;
	}
	else {
		cerr << "!!!!! Send user logout request success" << endl;
	}
}

void CustomTrade::orderInsert(
	TThostFtdcInstrumentIDType instrumentID,
	TThostFtdcPriceType price,
	TThostFtdcVolumeType volume,
	TThostFtdcDirectionType direction) 
{
	if (!isParallel) {
		wait();
	}
	if (!isLogin) {
		return;
	}

	CThostFtdcInputOrderField orderInsertReq;
	memset(&orderInsertReq, 0, sizeof(orderInsertReq));
	///���͹�˾����
	strcpy(orderInsertReq.BrokerID, sBrokerID);
	///Ͷ���ߴ���
	strcpy(orderInsertReq.InvestorID, sInvesterID);
	///��Լ����
	strcpy(orderInsertReq.InstrumentID, instrumentID);
	///��������
	strcpy(orderInsertReq.OrderRef, orderRef);
	///�����۸�����: �޼�
	orderInsertReq.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
	///��������: 
	orderInsertReq.Direction = direction;
	///��Ͽ�ƽ��־: ����
	orderInsertReq.CombOffsetFlag[0] = THOST_FTDC_OF_Open;
	///���Ͷ���ױ���־
	orderInsertReq.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;
	///�۸�
	orderInsertReq.LimitPrice = price;
	///������1
	orderInsertReq.VolumeTotalOriginal = volume;
	///��Ч������: ������Ч
	orderInsertReq.TimeCondition = THOST_FTDC_TC_GFD;
	///�ɽ�������: �κ�����
	orderInsertReq.VolumeCondition = THOST_FTDC_VC_AV;
	///��С�ɽ���: 1
	orderInsertReq.MinVolume = 1;
	///��������: ����
	orderInsertReq.ContingentCondition = THOST_FTDC_CC_Immediately;
	///ǿƽԭ��: ��ǿƽ
	orderInsertReq.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
	///�Զ������־: ��
	orderInsertReq.IsAutoSuspend = 0;
	///�û�ǿ����־: ��
	orderInsertReq.UserForceClose = 0;

	int rt = pTradeUserApi->ReqOrderInsert(&orderInsertReq, ++nRequestID);
	if (!rt) {
		cout << ">>>>> Send order insert request success" << endl;
	}
	else {
		cerr << "!!!!! Send order insert request failed" << endl;
	}
}

void CustomTrade::orderWithdraw(TThostFtdcInstrumentIDType instrumentID) {
	if (!isParallel) {
		wait();
	}
	if (!isLogin) {
		return;
	}

	CThostFtdcInputOrderActionField orderActionReq;
	memset(&orderActionReq, 0, sizeof(orderActionReq));
	///���͹�˾����
	strcpy(orderActionReq.BrokerID, sBrokerID);
	///Ͷ���ߴ���
	strcpy(orderActionReq.InvestorID, sInvesterID);
	///������������
	//	TThostFtdcOrderActionRefType	OrderActionRef;
	///��������
	strcpy(orderActionReq.OrderRef, orderRef);
	///������
	//	TThostFtdcRequestIDType	RequestID;
	///ǰ�ñ��
	orderActionReq.FrontID = tradeFrontID;
	///�Ự���
	orderActionReq.SessionID = sessionID;
	///����������
	//	TThostFtdcExchangeIDType	ExchangeID;
	///�������
	//	TThostFtdcOrderSysIDType	OrderSysID;
	///������־
	orderActionReq.ActionFlag = THOST_FTDC_AF_Delete;
	///�۸�
	//	TThostFtdcPriceType	LimitPrice;
	///�����仯
	//	TThostFtdcVolumeType	VolumeChange;
	///�û�����
	//	TThostFtdcUserIDType	UserID;
	///��Լ����
	strcpy(orderActionReq.InstrumentID, instrumentID);

	int rt = pTradeUserApi->ReqOrderAction(&orderActionReq, ++nRequestID);
	if (!rt) {
		cout << ">>>>> Send order withdraw request success" << endl;
	}
	else {
		cerr << "!!!!! Send order withdraw request failed" << endl;
	}
}


/* ----- TradeSpi�ص����� ----- */
inline void showErrorMessage(CThostFtdcRspInfoField *pRspInfo) 
{
	cerr << "Error code: " << pRspInfo->ErrorID << endl;
	cerr << "Error message: " << pRspInfo->ErrorMsg << endl;
}

bool CustomTrade::isMyOrder(CThostFtdcOrderField *pOrder)
{
	return ((pOrder->FrontID == tradeFrontID) &&
		(pOrder->SessionID == sessionID) &&
		(strcmp(pOrder->OrderRef, orderRef) == 0));
}

bool CustomTrade::isTradingOrder(CThostFtdcOrderField *pOrder)
{
	return ((pOrder->OrderStatus != THOST_FTDC_OST_PartTradedNotQueueing) &&
		(pOrder->OrderStatus != THOST_FTDC_OST_Canceled) &&
		(pOrder->OrderStatus != THOST_FTDC_OST_AllTraded));
}

void CustomTrade::OnFrontConnected()
{
	fprintf(fout, "===== Connection Success =====\n");
	signal();
}

void CustomTrade::OnRspUserLogin(
	CThostFtdcRspUserLoginField *pRspUserLogin,
	CThostFtdcRspInfoField *pRspInfo,
	int nRequestID,
	bool bIsLast)
{
	bool bResult = pRspInfo && (pRspInfo->ErrorID != 0);
	if (!bResult) {
		isLogin = true;
		fprintf(fout, "===== Login Success =====\n");
		fprintf(fout, "===== User Information as Follow =====\n");
		cout << "Tradeding day: " << pRspUserLogin->TradingDay << endl;
		cout << "Login time: " << pRspUserLogin->LoginTime << endl;
		cout << "Broker ID: " << pRspUserLogin->BrokerID << endl;
		cout << "User ID: " << pRspUserLogin->UserID << endl;
		// ����Ự����
		tradeFrontID = pRspUserLogin->FrontID;
		sessionID = pRspUserLogin->SessionID;
		strcpy(orderRef, pRspUserLogin->MaxOrderRef);
	} else {
		fprintf(ferr, "===== Login failed =====\n");
		showErrorMessage(pRspInfo);
	}
	signal();
}

void CustomTrade::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	bool bResult = pRspInfo && (pRspInfo->ErrorID != 0);
	if (bResult) {
		fprintf(ferr, "===== Something Error =====\n");
		showErrorMessage(pRspInfo);
	}
}

void CustomTrade::OnFrontDisconnected(int nReason)
{
	cerr << "===== Connection Break off =====" << endl;
	cerr << "Error code: 0x" << std::hex << nReason << endl;
}

void CustomTrade::OnHeartBeatWarning(int nTimeLapse)
{
	cerr << "===== Heart Beat Warning =====" << endl;
	cerr << "The interval connect time: " << nTimeLapse << endl;
}

void CustomTrade::OnRspUserLogout(
	CThostFtdcUserLogoutField *pUserLogout,
	CThostFtdcRspInfoField *pRspInfo,
	int nRequestID,
	bool bIsLast)
{
	bool bResult = pRspInfo && (pRspInfo->ErrorID != 0);
	if (!bResult) {
		isLogin = false;
		cout << "===== User Logout Success =====" << endl;
		cout << "===== User Information as Follow =====" << endl;
		cout << "Broker ID: " << pUserLogout->BrokerID << endl;
		cout << "User ID: " << pUserLogout->UserID << endl;
	} else {
		fprintf(ferr, "===== Logout Failed =====\n");
		showErrorMessage(pRspInfo);
	}
	signal();
}


void CustomTrade::OnRspOrderInsert(
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
	signal();
}

void CustomTrade::OnRspOrderAction(
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
	signal();
}

void CustomTrade::OnRtnOrder(CThostFtdcOrderField *pOrder)
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

void CustomTrade::OnRtnTrade(CThostFtdcTradeField *pTrade)
{
	cout << "===== Order Trade Successfully =====" << endl;
	cout << "Trade time: " << pTrade->TradeTime << endl;
	cout << "Instrument ID: " << pTrade->InstrumentID << endl;
	cout << "Price: " << pTrade->Price << endl;
	cout << "Volumn: " << pTrade->Volume << endl;
	cout << "Open or close: " << pTrade->Direction << endl;
}














// void CustomTradeSpi::reqSettlementInfoConfirm()
// {
// 	CThostFtdcSettlementInfoConfirmField settlementConfirmReq;
// 	memset(&settlementConfirmReq, 0, sizeof(settlementConfirmReq));
// 	strcpy(settlementConfirmReq.BrokerID, sBrokerID);
// 	strcpy(settlementConfirmReq.InvestorID, sInvesterID);
// 	static int requestID = 0; // ������
// 	int rt = g_pTradeUserApi->ReqSettlementInfoConfirm(&settlementConfirmReq, requestID);
// 	if (!rt)
// 		cout << ">>>>>>����Ͷ���߽�����ȷ������ɹ�" << endl;
// 	else
// 		cerr << "--->>>����Ͷ���߽�����ȷ������ʧ��" << endl;
// }

// void CustomTradeSpi::reqQueryInstrument()
// {
// 	CThostFtdcQryInstrumentField instrumentReq;
// 	memset(&instrumentReq, 0, sizeof(instrumentReq));
// 	strcpy(instrumentReq.InstrumentID, g_pTradeInstrumentID);
// 	static int requestID = 0; // ������
// 	int rt = g_pTradeUserApi->ReqQryInstrument(&instrumentReq, requestID);
// 	if (!rt)
// 		cout << ">>>>>>���ͺ�Լ��ѯ����ɹ�" << endl;
// 	else
// 		cerr << "--->>>���ͺ�Լ��ѯ����ʧ��" << endl;
// }

// void CustomTradeSpi::reqQueryTradingAccount()
// {
// 	CThostFtdcQryTradingAccountField tradingAccountReq;
// 	memset(&tradingAccountReq, 0, sizeof(tradingAccountReq));
// 	strcpy(tradingAccountReq.BrokerID, sBrokerID);
// 	strcpy(tradingAccountReq.InvestorID, sInvesterID);
// 	static int requestID = 0; // ������
// 	std::this_thread::sleep_for(std::chrono::milliseconds(700)); // ��ʱ����Ҫͣ��һ����ܲ�ѯ�ɹ�
// 	int rt = g_pTradeUserApi->ReqQryTradingAccount(&tradingAccountReq, requestID);
// 	if (!rt)
// 		cout << ">>>>>>����Ͷ�����ʽ��˻���ѯ����ɹ�" << endl;
// 	else
// 		cerr << "--->>>����Ͷ�����ʽ��˻���ѯ����ʧ��" << endl;
// }

// void CustomTradeSpi::reqQueryInvestorPosition()
// {
// 	CThostFtdcQryInvestorPositionField postionReq;
// 	memset(&postionReq, 0, sizeof(postionReq));
// 	strcpy(postionReq.BrokerID, sBrokerID);
// 	strcpy(postionReq.InvestorID, sInvesterID);
// 	strcpy(postionReq.InstrumentID, g_pTradeInstrumentID);
// 	static int requestID = 0; // ������
// 	std::this_thread::sleep_for(std::chrono::milliseconds(700)); // ��ʱ����Ҫͣ��һ����ܲ�ѯ�ɹ�
// 	int rt = g_pTradeUserApi->ReqQryInvestorPosition(&postionReq, requestID);
// 	if (!rt)
// 		cout << ">>>>>>����Ͷ���ֲֲ߳�ѯ����ɹ�" << endl;
// 	else
// 		cerr << "--->>>����Ͷ���ֲֲ߳�ѯ����ʧ��" << endl;
// }

// void CustomTradeSpi::reqOrderInsert()
// {
// 	CThostFtdcInputOrderField orderInsertReq;
// 	memset(&orderInsertReq, 0, sizeof(orderInsertReq));
// 	///���͹�˾����
// 	strcpy(orderInsertReq.BrokerID, sBrokerID);
// 	///Ͷ���ߴ���
// 	strcpy(orderInsertReq.InvestorID, sInvesterID);
// 	///��Լ����
// 	strcpy(orderInsertReq.InstrumentID, g_pTradeInstrumentID);
// 	///��������
// 	strcpy(orderInsertReq.OrderRef, order_ref);
// 	///�����۸�����: �޼�
// 	orderInsertReq.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
// 	///��������: 
// 	orderInsertReq.Direction = gTradeDirection;
// 	///��Ͽ�ƽ��־: ����
// 	orderInsertReq.CombOffsetFlag[0] = THOST_FTDC_OF_Open;
// 	///���Ͷ���ױ���־
// 	orderInsertReq.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;
// 	///�۸�
// 	orderInsertReq.LimitPrice = gLimitPrice;
// 	///������1
// 	orderInsertReq.VolumeTotalOriginal = 1;
// 	///��Ч������: ������Ч
// 	orderInsertReq.TimeCondition = THOST_FTDC_TC_GFD;
// 	///�ɽ�������: �κ�����
// 	orderInsertReq.VolumeCondition = THOST_FTDC_VC_AV;
// 	///��С�ɽ���: 1
// 	orderInsertReq.MinVolume = 1;
// 	///��������: ����
// 	orderInsertReq.ContingentCondition = THOST_FTDC_CC_Immediately;
// 	///ǿƽԭ��: ��ǿƽ
// 	orderInsertReq.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
// 	///�Զ������־: ��
// 	orderInsertReq.IsAutoSuspend = 0;
// 	///�û�ǿ����־: ��
// 	orderInsertReq.UserForceClose = 0;

// 	static int requestID = 0; // ������
// 	int rt = g_pTradeUserApi->ReqOrderInsert(&orderInsertReq, ++requestID);
// 	if (!rt)
// 		cout << ">>>>>>���ͱ���¼������ɹ�" << endl;
// 	else
// 		cerr << "--->>>���ͱ���¼������ʧ��" << endl;
// }

// void CustomTradeSpi::reqOrderInsert(
// 	TThostFtdcInstrumentIDType instrumentID,
// 	TThostFtdcPriceType price,
// 	TThostFtdcVolumeType volume,
// 	TThostFtdcDirectionType direction)
// {
// 	CThostFtdcInputOrderField orderInsertReq;
// 	memset(&orderInsertReq, 0, sizeof(orderInsertReq));
// 	///���͹�˾����
// 	strcpy(orderInsertReq.BrokerID, sBrokerID);
// 	///Ͷ���ߴ���
// 	strcpy(orderInsertReq.InvestorID, sInvesterID);
// 	///��Լ����
// 	strcpy(orderInsertReq.InstrumentID, instrumentID);
// 	///��������
// 	strcpy(orderInsertReq.OrderRef, order_ref);
// 	///�����۸�����: �޼�
// 	orderInsertReq.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
// 	///��������: 
// 	orderInsertReq.Direction = direction;
// 	///��Ͽ�ƽ��־: ����
// 	orderInsertReq.CombOffsetFlag[0] = THOST_FTDC_OF_Open;
// 	///���Ͷ���ױ���־
// 	orderInsertReq.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;
// 	///�۸�
// 	orderInsertReq.LimitPrice = price;
// 	///������1
// 	orderInsertReq.VolumeTotalOriginal = volume;
// 	///��Ч������: ������Ч
// 	orderInsertReq.TimeCondition = THOST_FTDC_TC_GFD;
// 	///�ɽ�������: �κ�����
// 	orderInsertReq.VolumeCondition = THOST_FTDC_VC_AV;
// 	///��С�ɽ���: 1
// 	orderInsertReq.MinVolume = 1;
// 	///��������: ����
// 	orderInsertReq.ContingentCondition = THOST_FTDC_CC_Immediately;
// 	///ǿƽԭ��: ��ǿƽ
// 	orderInsertReq.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
// 	///�Զ������־: ��
// 	orderInsertReq.IsAutoSuspend = 0;
// 	///�û�ǿ����־: ��
// 	orderInsertReq.UserForceClose = 0;

// 	static int requestID = 0; // ������
// 	int rt = g_pTradeUserApi->ReqOrderInsert(&orderInsertReq, ++requestID);
// 	if (!rt)
// 		cout << ">>>>>>���ͱ���¼������ɹ�" << endl;
// 	else
// 		cerr << "--->>>���ͱ���¼������ʧ��" << endl;
// }

// void CustomTradeSpi::reqOrderAction(CThostFtdcOrderField *pOrder)
// {
// 	static bool orderActionSentFlag = false; // �Ƿ����˱���
// 	if (orderActionSentFlag)
// 		return;

// 	CThostFtdcInputOrderActionField orderActionReq;
// 	memset(&orderActionReq, 0, sizeof(orderActionReq));
// 	///���͹�˾����
// 	strcpy(orderActionReq.BrokerID, pOrder->BrokerID);
// 	///Ͷ���ߴ���
// 	strcpy(orderActionReq.InvestorID, pOrder->InvestorID);
// 	///������������
// 	//	TThostFtdcOrderActionRefType	OrderActionRef;
// 	///��������
// 	strcpy(orderActionReq.OrderRef, pOrder->OrderRef);
// 	///������
// 	//	TThostFtdcRequestIDType	RequestID;
// 	///ǰ�ñ��
// 	orderActionReq.FrontID = trade_front_id;
// 	///�Ự���
// 	orderActionReq.SessionID = session_id;
// 	///����������
// 	//	TThostFtdcExchangeIDType	ExchangeID;
// 	///�������
// 	//	TThostFtdcOrderSysIDType	OrderSysID;
// 	///������־
// 	orderActionReq.ActionFlag = THOST_FTDC_AF_Delete;
// 	///�۸�
// 	//	TThostFtdcPriceType	LimitPrice;
// 	///�����仯
// 	//	TThostFtdcVolumeType	VolumeChange;
// 	///�û�����
// 	//	TThostFtdcUserIDType	UserID;
// 	///��Լ����
// 	strcpy(orderActionReq.InstrumentID, pOrder->InstrumentID);
// 	static int requestID = 0; // ������
// 	int rt = g_pTradeUserApi->ReqOrderAction(&orderActionReq, ++requestID);
// 	if (!rt)
// 		cout << ">>>>>>���ͱ�����������ɹ�" << endl;
// 	else
// 		cerr << "--->>>���ͱ�����������ʧ��" << endl;
// 	orderActionSentFlag = true;
// }



// void CustomTradeSpi::OnRspSettlementInfoConfirm(
// 	CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm,
// 	CThostFtdcRspInfoField *pRspInfo,
// 	int nRequestID,
// 	bool bIsLast)
// {
// 	if (!isErrorRspInfo(pRspInfo))
// 	{
// 		cout << "=====Ͷ���߽�����ȷ�ϳɹ�=====" << endl;
// 		cout << "ȷ�����ڣ� " << pSettlementInfoConfirm->ConfirmDate << endl;
// 		cout << "ȷ��ʱ�䣺 " << pSettlementInfoConfirm->ConfirmTime << endl;
// 		// �����ѯ��Լ
// 		reqQueryInstrument();
// 	}
// }

// void CustomTradeSpi::OnRspQryInstrument(
// 	CThostFtdcInstrumentField *pInstrument,
// 	CThostFtdcRspInfoField *pRspInfo,
// 	int nRequestID,
// 	bool bIsLast)
// {
// 	if (!isErrorRspInfo(pRspInfo))
// 	{
// 		cout << "=====��ѯ��Լ����ɹ�=====" << endl;
// 		cout << "���������룺 " << pInstrument->ExchangeID << endl;
// 		cout << "��Լ���룺 " << pInstrument->InstrumentID << endl;
// 		cout << "��Լ�ڽ������Ĵ��룺 " << pInstrument->ExchangeInstID << endl;
// 		cout << "ִ�мۣ� " << pInstrument->StrikePrice << endl;
// 		cout << "�����գ� " << pInstrument->EndDelivDate << endl;
// 		cout << "��ǰ����״̬�� " << pInstrument->IsTrading << endl;
// 		// �����ѯͶ�����ʽ��˻�
// 		reqQueryTradingAccount();
// 	}
// }

// void CustomTradeSpi::OnRspQryTradingAccount(
// 	CThostFtdcTradingAccountField *pTradingAccount,
// 	CThostFtdcRspInfoField *pRspInfo,
// 	int nRequestID,
// 	bool bIsLast)
// {
// 	if (!isErrorRspInfo(pRspInfo))
// 	{
// 		cout << "=====��ѯͶ�����ʽ��˻��ɹ�=====" << endl;
// 		cout << "Ͷ�����˺ţ� " << pTradingAccount->AccountID << endl;
// 		cout << "�����ʽ� " << pTradingAccount->Available << endl;
// 		cout << "��ȡ�ʽ� " << pTradingAccount->WithdrawQuota << endl;
// 		cout << "��ǰ��֤��: " << pTradingAccount->CurrMargin << endl;
// 		cout << "ƽ��ӯ���� " << pTradingAccount->CloseProfit << endl;
// 		// �����ѯͶ���ֲ߳�
// 		reqQueryInvestorPosition();
// 	}
// }

// void CustomTradeSpi::OnRspQryInvestorPosition(
// 	CThostFtdcInvestorPositionField *pInvestorPosition,
// 	CThostFtdcRspInfoField *pRspInfo,
// 	int nRequestID,
// 	bool bIsLast)
// {
// 	if (!isErrorRspInfo(pRspInfo))
// 	{
// 		cout << "=====��ѯͶ���ֲֳ߳ɹ�=====" << endl;
// 		if (pInvestorPosition)
// 		{
// 			cout << "��Լ���룺 " << pInvestorPosition->InstrumentID << endl;
// 			cout << "���ּ۸� " << pInvestorPosition->OpenAmount << endl;
// 			cout << "�������� " << pInvestorPosition->OpenVolume << endl;
// 			cout << "���ַ��� " << pInvestorPosition->PosiDirection << endl;
// 			cout << "ռ�ñ�֤��" << pInvestorPosition->UseMargin << endl;
// 		}
// 		else
// 			cout << "----->�ú�Լδ�ֲ�" << endl;
		
// 		// ����¼������������һ���ӿڣ��˴��ǰ�˳��ִ�У�
// 		/*if (loginFlag)
// 			reqOrderInsert();*/
// 		//if (loginFlag)
// 		//	reqOrderInsertWithParams(g_pTradeInstrumentID, gLimitPrice, 1, gTradeDirection); // �Զ���һ�ʽ���

// 		// ���Խ���
// 		cout << "=====��ʼ������Խ���=====" << endl;
// 		while (loginFlag)
// 			StrategyCheckAndTrade(g_pTradeInstrumentID, this);
// 	}
// }
