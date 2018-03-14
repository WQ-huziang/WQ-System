#pragma once

/* ----- Complete Md Class ----- */  
#include "myapi/CustomMd.h"
#include "api/ctp/ThostFtdcMdApi.h"
#include "util/CustomUser.h"
#include "util/CustomSemaphore.h"

/*
 * this class inhert interface CThostFtdcMdSpi
 * this class package MdApi for main.cpp use
 */
class CustomCtpMd: public CustomMd, public CThostFtdcMdSpi, public CustomUser
{
private:
	Semaphore *mylock;                          // when connect can this send other req
	int nRequestID;                           // request ID
	CThostFtdcMdApi *pMdUserApi;              // MdApi's pointer

/* ----- custom function ----- */
public:
	/// constructure function
	/// dataDirPath must be absolute
	static CustomCtpMd* CreateCustomCtpMd(
		TThostFtdcInvestorIDType gInvesterID,
		TThostFtdcPasswordType gInvesterPassword,
		TThostFtdcBrokerIDType gBrokerID,
		char dataDirPath[], 
		char gMdFrontAddr[],
		int clientID,
		int serverID);

	/// constructure function
	CustomCtpMd(
		TThostFtdcBrokerIDType gBrokerID,
		TThostFtdcInvestorIDType gInvesterID,
		TThostFtdcPasswordType gInvesterPassword,
		char *dataDirPath,
		int clientID,
		int serverID) 
		: CustomUser(gBrokerID, gInvesterID, gInvesterPassword, dataDirPath), CustomMd(clientID, serverID) {}

	/// instructure function
	~CustomCtpMd();

	/// divided string to strings by space
	/// this function need to delete by other place !!!!!!
	static int ProcessInstrumentIDList(char *ppInstrumentID[], const int length, const char pInstrumentIDList[]);

	void Wait();

	void UserLogin();

	void UserLogout();

	/// add subcribe instrument
	void SubscribeInstrument(char **ppInstrumentID, int nCount);

	/// delete subcribe instrument
	void UnsubscribeInstrument(char **ppInstrumentID, int nCount);

	void QuoteInstrument();

	void UnQuoteInstrument();

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

