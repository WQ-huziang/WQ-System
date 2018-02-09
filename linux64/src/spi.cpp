#include "spi.hpp"
#include <iostream>
#include <unordered_map>
using std::cout;
using std::endl;
using std::cerr;

// ---- 全局参数声明 ---- //
extern CThostFtdcMdApi *g_pMdUserApi;            // 行情指针
extern char gMdFrontAddr[];                      // 模拟行情前置地址
extern TThostFtdcBrokerIDType gBrokerID;         // 模拟经纪商代码
extern TThostFtdcInvestorIDType gInvesterID;     // 投资者账户名
extern TThostFtdcPasswordType gInvesterPassword; // 投资者密码
extern char *g_pInstrumentID[];                  // 行情合约代码列表，中、上、大、郑交易所各选一种
extern int instrumentNum;                        // 行情合约订阅数量
extern std::unordered_map<std::string, TickToKlineHelper> g_KlineHash; // k线存储表

// ---- ctp_api回调函数 ---- //
///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
void CompleteMdSpi::OnFrontConnected() {
    std::cout << "=====建立网络连接成功=====" << std::endl;
	// 开始登录
	CThostFtdcReqUserLoginField loginReq;
	memset(&loginReq, 0, sizeof(loginReq));
	strcpy(loginReq.BrokerID, gBrokerID);
	strcpy(loginReq.UserID, gInvesterID);
	strcpy(loginReq.Password, gInvesterPassword);
	static int requestID = 0; // 请求编号
	int rt = g_pMdUserApi->ReqUserLogin(&loginReq, requestID);
	if (!rt)
		std::cout << ">>>>>>发送登录请求成功" << std::endl;
	else
		std::cerr << "--->>>发送登录请求失败" << std::endl;
}

///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
///@param nReason 错误原因
///        0x1001 网络读失败
///        0x1002 网络写失败
///        0x2001 接收心跳超时
///        0x2002 发送心跳失败
///        0x2003 收到错误报文
void CompleteMdSpi::OnFrontDisconnected(int nReason) {
	std::cerr << "=====网络连接断开=====" << std::endl;
	std::cerr << "错误码： " << nReason << std::endl;
}
	
///心跳超时警告。当长时间未收到报文时，该方法被调用。
///@param nTimeLapse 距离上次接收报文的时间
void CompleteMdSpi::OnHeartBeatWarning(int nTimeLapse) {

}

///登录请求响应
void CompleteMdSpi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, 
 	CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {

}

///登出请求响应
void CompleteMdSpi::OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, 
 	CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast); {

}

///错误应答
void CompleteMdSpi::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {

}

///订阅行情应答
void CompleteMdSpi::OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, 
 	CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {

}

///取消订阅行情应答
void CompleteMdSpi::OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, 
 	CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {

}

///订阅询价应答
void CompleteMdSpi::OnRspSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, 
 	CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {

}

///取消订阅询价应答
void CompleteMdSpi::OnRspUnSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, 
 	CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {

}

///深度行情通知
void CompleteMdSpi::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData) {

}

///询价通知
void CompleteMdSpi::OnRtnForQuoteRsp(CThostFtdcForQuoteRspField *pForQuoteRsp) {

}