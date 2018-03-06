#pragma once

/* ----- Complete Md Class ----- */  
#include <vector>
#include <cstring>
#include "ThostFtdcMdApi.h"
using namespace std;

#define Quote 0
#define Subscribe 1

/*
 * this class inhert interface CThostFtdcMdSpi
 * this class package MdApi for main.cpp use
 */
class CustomMd: public CThostFtdcMdSpi
{
private:
	CThostFtdcMdApi *pMdUserApi;              // MdApi's pointer

	TThostFtdcBrokerIDType sBrokerID;         // broker ID
	TThostFtdcInvestorIDType sInvesterID;     // user ID
	TThostFtdcPasswordType sInvesterPassword; // user password

	vector<char*> vSubInstrumentID;           // the subscribe instrument
	vector<char*> vQuoteInstrumentID;         // the quoit instrument
public:
	/// structure function
	/// gp_InstrumentID = empty
	CustomMd(TThostFtdcInvestorIDType gInvesterID,
		TThostFtdcPasswordType gInvesterPassword,
		TThostFtdcBrokerIDType gBrokerID = "9999", 
		char gMdFrontAddr[] = "tcp://180.168.146.187:10011",
		char dataDicPath[] = "data/");

	/// instructure function
	~CustomMd();

	void userLogin();

	void userLogout();

	/// 
	void subscribeInstrument();

	void unsubscribeInstrument();

	void quoteInstrument();

	void unQuoteInstrument();

	/* ----- Overlap the MdSpi ----- */
	///µ±¿Í»§¶ËÓë½»Ò×ºóÌ¨½¨Á¢ÆðÍ¨ÐÅÁ¬½ÓÊ±£¨»¹Î´µÇÂ¼Ç°£©£¬¸Ã·½·¨±»µ÷ÓÃ¡£
	void OnFrontConnected();

	///µ±¿Í»§¶ËÓë½»Ò×ºóÌ¨Í¨ÐÅÁ¬½Ó¶Ï¿ªÊ±£¬¸Ã·½·¨±»µ÷ÓÃ¡£µ±·¢ÉúÕâ¸öÇé¿öºó£¬API»á×Ô¶¯ÖØÐÂÁ¬½Ó£¬¿Í»§¶Ë¿É²»×ö´¦Àí¡£
	///@param nReason ´íÎóÔ­Òò
	///        0x1001 ÍøÂç¶ÁÊ§°Ü
	///        0x1002 ÍøÂçÐ´Ê§°Ü
	///        0x2001 ½ÓÊÕÐÄÌø³¬Ê±
	///        0x2002 ·¢ËÍÐÄÌøÊ§°Ü
	///        0x2003 ÊÕµ½´íÎó±¨ÎÄ
	void OnFrontDisconnected(int nReason);

	///ÐÄÌø³¬Ê±¾¯¸æ¡£µ±³¤Ê±¼äÎ´ÊÕµ½±¨ÎÄÊ±£¬¸Ã·½·¨±»µ÷ÓÃ¡£
	///@param nTimeLapse ¾àÀëÉÏ´Î½ÓÊÕ±¨ÎÄµÄÊ±¼ä
	void OnHeartBeatWarning(int nTimeLapse);

	///µÇÂ¼ÇëÇóÏìÓ¦
	void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///µÇ³öÇëÇóÏìÓ¦
	void OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///´íÎóÓ¦´ð
	void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///¶©ÔÄÐÐÇéÓ¦´ð
	void OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///È¡Ïû¶©ÔÄÐÐÇéÓ¦´ð
	void OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///¶©ÔÄÑ¯¼ÛÓ¦´ð
	void OnRspSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///È¡Ïû¶©ÔÄÑ¯¼ÛÓ¦´ð
	void OnRspUnSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///Éî¶ÈÐÐÇéÍ¨Öª
	void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData);

	///Ñ¯¼ÛÍ¨Öª
	void OnRtnForQuoteRsp(CThostFtdcForQuoteRspField *pForQuoteRsp);
};

