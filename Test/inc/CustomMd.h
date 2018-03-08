#pragma once

/* ----- Complete Md Class ----- */  
#include "ThostFtdcMdApi.h"
#include "CustomUser.h"
#include "CustomSemaphore.h"

// #define Quote 0
// #define Subscribe 1

/*
 * this class inhert interface CThostFtdcMdSpi
 * this class package MdApi for main.cpp use
 */
class CustomMd: public CThostFtdcMdSpi, public CustomUser, public Semaphore
{
private:
	bool isParallel;                          // parallel or not
	int nRequestID;                           // request ID

	CThostFtdcMdApi *pMdUserApi;              // MdApi's pointer

/* ----- custom function ----- */
public:
	/// constructure function
	/// dataDirPath must be absolute
	static CustomMd* CreateCustomMd(
		TThostFtdcInvestorIDType gInvesterID,
		TThostFtdcPasswordType gInvesterPassword,
		char dataDirPath[] = "/home/huziang/Desktop/api/Test/data/Md/",
		bool isParallel = false,
		TThostFtdcBrokerIDType gBrokerID = "9999", 
		char gMdFrontAddr[] = "tcp://180.168.146.187:10011");

	/// constructure function
	CustomMd(
		TThostFtdcBrokerIDType gBrokerID,
		TThostFtdcInvestorIDType gInvesterID,
		TThostFtdcPasswordType gInvesterPassword,
		char *dataDirPath) 
		: CustomUser(gBrokerID, gInvesterID, gInvesterPassword, dataDirPath) {}

	/// instructure function
	~CustomMd();

	void userLogin();

	void userLogout();

	/// add subcribe instrument
	void subscribeInstrument(char *ppInstrumentID[], int nCount);

	/// delete subcribe instrument
	void unsubscribeInstrument(char *ppInstrumentID[], int nCount);

	void quoteInstrument();

	void unQuoteInstrument();

/* ----- Overlap the MdSpi ----- */
public:
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

