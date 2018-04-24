/*
 * @Author: w2w 
 * @Date: 2018-02-28 10:58:13 
 * @Last Modified by:   w2w 
 * @Last Modified time: 2018-02-28 10:58:13 
 */

#ifndef CONSTANT_H
#define CONSTANT_H

typedef char ControlFlagType;

/////////////////////////////////
// ControlFlagType: control flag
/////////////////////////////////
// order insert
#define CHAR_OrderInsert '0'
// order withdraw
#define CHAR_OrderWithdraw '1'
// subscribe instrument
#define CHAR_Subscribe '2'
// unsubscribe instrument
#define CHAR_Unsubscribe '3'

// Index for Sources
const short SOURCE_CTP = 1;

// Exchange Names

// Exchange ids
const short EXCHANGE_ID_SSE = 1; //ÉÏº£Ö¤È¯½»Ò×Ëù


//char * const  EXCHANGE_SSE = "SSE"; //ÉÏº£Ö¤È¯½»Ò×Ëù
const char EXCHANGE_SSE[] = "SSE"; //ÉÏº£Ö¤È¯½»Ò×Ëù
//const char* EXCHANGE_SSE = "SSE"; //ÉÏº£Ö¤È¯½»Ò×Ëù
//char * a = "aaaa"
//char a[] = "aaa";


///////////////////////////////////
// WZActionFlagType: ±¨µ¥²Ù×÷±êÖ¾
///////////////////////////////////
//É¾³ý
#define WZ_CHAR_Delete          '0'
//¹ÒÆð
#define WZ_CHAR_Suspend         '1'
//¼¤»î
#define WZ_CHAR_Active          '2'
//ÐÞ¸Ä
#define WZ_CHAR_Modify          '3'

typedef char WZActionFlagType;

///////////////////////////////////
// WZContingentConditionType: ´¥·¢Ìõ¼þ
///////////////////////////////////
//Á¢¼´
#define WZ_CHAR_Immediately     '1'
//Ö¹Ëð
#define WZ_CHAR_Touch           '2'
//Ö¹Ó®
#define WZ_CHAR_TouchProfit     '3'
//Ô¤Âñµ¥
#define WZ_CHAR_ParkedOrder     '4'
//×îÐÂ¼Û´óÓÚÌõ¼þ¼Û
#define WZ_CHAR_LastPriceGreaterThanStopPrice '5'
//×îÐÂ¼Û´óÓÚµÈÓÚÌõ¼þ¼Û
#define WZ_CHAR_LastPriceGreaterEqualStopPrice '6'
//×îÐÂ¼ÛÐ¡ÓÚÌõ¼þ¼Û
#define WZ_CHAR_LastPriceLesserThanStopPrice '7'
//×îÐÂ¼ÛÐ¡ÓÚµÈÓÚÌõ¼þ¼Û
#define WZ_CHAR_LastPriceLesserEqualStopPrice '8'
//ÂôÒ»¼Û´óÓÚÌõ¼þ¼Û
#define WZ_CHAR_AskPriceGreaterThanStopPrice '9'
//ÂôÒ»¼Û´óÓÚµÈÓÚÌõ¼þ¼Û
#define WZ_CHAR_AskPriceGreaterEqualStopPrice 'A'
//ÂôÒ»¼ÛÐ¡ÓÚÌõ¼þ¼Û
#define WZ_CHAR_AskPriceLesserThanStopPrice 'B'
//ÂôÒ»¼ÛÐ¡ÓÚµÈÓÚÌõ¼þ¼Û
#define WZ_CHAR_AskPriceLesserEqualStopPrice 'C'
//ÂòÒ»¼Û´óÓÚÌõ¼þ¼Û
#define WZ_CHAR_BidPriceGreaterThanStopPrice 'D'
//ÂòÒ»¼Û´óÓÚµÈÓÚÌõ¼þ¼Û
#define WZ_CHAR_BidPriceGreaterEqualStopPrice 'E'
//ÂòÒ»¼ÛÐ¡ÓÚÌõ¼þ¼Û
#define WZ_CHAR_BidPriceLesserThanStopPrice 'F'
//ÂòÒ»¼ÛÐ¡ÓÚµÈÓÚÌõ¼þ¼Û
#define WZ_CHAR_BidPriceLesserEqualStopPrice 'H'

typedef char WZContingentConditionType;

///////////////////////////////////
// WZDirectionType: ÂòÂô·½Ïò
///////////////////////////////////
//Âò
#define WZ_CHAR_Buy             '0'
//Âô
#define WZ_CHAR_Sell            '1'

typedef char WZDirectionType;

///////////////////////////////////
// WZForceCloseReasonType: Ç¿Æ½Ô­Òò
///////////////////////////////////
//·ÇÇ¿Æ½
#define WZ_CHAR_NotForceClose   '0'
//×Ê½ð²»×ã
#define WZ_CHAR_LackDeposit     '1'
//¿Í»§³¬²Ö
#define WZ_CHAR_ClientOverPositionLimit '2'
//»áÔ±³¬²Ö
#define WZ_CHAR_MemberOverPositionLimit '3'
//³Ö²Ö·ÇÕûÊý±¶
#define WZ_CHAR_NotMultiple     '4'
//Î¥¹æ
#define WZ_CHAR_Violation       '5'
//ÆäËü
#define WZ_CHAR_Other           '6'
//×ÔÈ»ÈËÁÙ½ü½»¸î
#define WZ_CHAR_PersonDeliv     '7'

typedef char WZForceCloseReasonType;

///////////////////////////////////
// WZHedgeFlagType: Í¶»úÌ×±£±êÖ¾
///////////////////////////////////
//Í¶»ú
#define WZ_CHAR_Speculation     '1'
//Ì×Àû
#define WZ_CHAR_Argitrage       '2'
//Ì×±£
#define WZ_CHAR_Hedge           '3'
//×öÊÐÉÌ(femas)
#define WZ_CHAR_MarketMaker     '4'
//Æ¥ÅäËùÓÐµÄÖµ(femas)
#define WZ_CHAR_AllValue        '9'

typedef char WZHedgeFlagType;

///////////////////////////////////
// WZOffsetFlagType: ¿ªÆ½±êÖ¾
///////////////////////////////////
//¿ª²Ö
#define WZ_CHAR_Open            '0'
//Æ½²Ö
#define WZ_CHAR_Close           '1'
//Ç¿Æ½
#define WZ_CHAR_ForceClose      '2'
//Æ½½ñ
#define WZ_CHAR_CloseToday      '3'
//Æ½×ò
#define WZ_CHAR_CloseYesterday  '4'
//Ç¿¼õ
#define WZ_CHAR_ForceOff        '5'
//±¾µØÇ¿Æ½
#define WZ_CHAR_LocaWZorceClose '6'
//²»·Ö¿ªÆ½
#define WZ_CHAR_Non             'N'

typedef char WZOffsetFlagType;

///////////////////////////////////
// WZOrderPriceTypeType: ±¨µ¥¼Û¸ñÌõ¼þ
///////////////////////////////////
//ÈÎÒâ¼Û
#define WZ_CHAR_AnyPrice        '1'
//ÏÞ¼Û
#define WZ_CHAR_LimitPrice      '2'
//×îÓÅ¼Û
#define WZ_CHAR_BestPrice       '3'

typedef char WZOrderPriceTypeType;

///////////////////////////////////
// WZOrderStatusType: ±¨µ¥×´Ì¬
///////////////////////////////////
//È«²¿³É½»£¨×îÖÕ×´Ì¬£©
#define WZ_CHAR_AllTraded       '0'
//²¿·Ö³É½»»¹ÔÚ¶ÓÁÐÖÐ
#define WZ_CHAR_PartTradedQueueing '1'
//²¿·Ö³É½»²»ÔÚ¶ÓÁÐÖÐ£¨²¿³É²¿³·£¬ ×îÖÕ×´Ì¬£©
#define WZ_CHAR_PartTradedNotQueueing '2'
//Î´³É½»»¹ÔÚ¶ÓÁÐÖÐ
#define WZ_CHAR_NoTradeQueueing '3'
//Î´³É½»²»ÔÚ¶ÓÁÐÖÐ£¨±»¾Ü¾ø£¬×îÖÕ×´Ì¬£©
#define WZ_CHAR_NoTradeNotQueueing '4'
//³·µ¥
#define WZ_CHAR_Canceled        '5'
//¶©µ¥ÒÑ±¨Èë½»Ò×ËùÎ´Ó¦´ð
#define WZ_CHAR_AcceptedNoReply '6'
//Î´Öª
#define WZ_CHAR_Unknown         'a'
//ÉÐÎ´´¥·¢
#define WZ_CHAR_NotTouched      'b'
//ÒÑ´¥·¢
#define WZ_CHAR_Touched         'c'
//·Ïµ¥´íÎó£¨×îÖÕ×´Ì¬£©
#define WZ_CHAR_Error           'd'
//¶©µ¥ÒÑÐ´Èë
#define WZ_CHAR_OrderInserted   'i'
//Ç°ÖÃÒÑ½ÓÊÜ
#define WZ_CHAR_OrderAccepted   'j'

typedef char WZOrderStatusType;

///////////////////////////////////
// WZPosiDirectionType: ³Ö²Ö¶à¿Õ·½Ïò
///////////////////////////////////
//¾»
#define WZ_CHAR_Net             '1'
//¶àÍ·
#define WZ_CHAR_Long            '2'
//¿ÕÍ·
#define WZ_CHAR_Short           '3'

typedef char WZPosiDirectionType;

///////////////////////////////////
// WZPositionDateType: ³Ö²ÖÈÕÆÚ
///////////////////////////////////
//½ñÈÕ³Ö²Ö
#define WZ_CHAR_Today           '1'
//ÀúÊ·³Ö²Ö
#define WZ_CHAR_History         '2'
//Á½ÖÖ³Ö²Ö
#define WZ_CHAR_Both            '3'

typedef char WZPositionDateType;

///////////////////////////////////
// WZTimeConditionType: ÓÐÐ§ÆÚÀàÐÍ
///////////////////////////////////
//Á¢¼´Íê³É£¬·ñÔò³·Ïú
#define WZ_CHAR_IOC             '1'
//±¾½ÚÓÐÐ§
#define WZ_CHAR_GFS             '2'
//µ±ÈÕÓÐÐ§
#define WZ_CHAR_GFD             '3'
//Ö¸¶¨ÈÕÆÚÇ°ÓÐÐ§
#define WZ_CHAR_GTD             '4'
//³·ÏúÇ°ÓÐÐ§
#define WZ_CHAR_GTC             '5'
//¼¯ºÏ¾º¼ÛÓÐÐ§
#define WZ_CHAR_GFA             '6'
//FAK»òIOC(yisheng)
#define WZ_CHAR_FAK             'A'
//FOK(yisheng)
#define WZ_CHAR_FOK             'O'

typedef char WZTimeConditionType;

///////////////////////////////////
// WZVolumeConditionType: ³É½»Á¿ÀàÐÍ
///////////////////////////////////
//ÈÎºÎÊýÁ¿
#define WZ_CHAR_AV              '1'
//×îÐ¡ÊýÁ¿
#define WZ_CHAR_MV              '2'
//È«²¿ÊýÁ¿
#define WZ_CHAR_CV              '3'

typedef char WZVolumeConditionType;

///////////////////////////////////
// WZYsHedgeFlagType: Ò×Ê¢Í¶»ú±£ÖµÀàÐÍ
///////////////////////////////////
//±£Öµ
#define WZ_CHAR_YsB             'B'
//Ì×Àû
#define WZ_CHAR_YsL             'L'
//ÎÞ
#define WZ_CHAR_YsNon           'N'
//Í¶»ú
#define WZ_CHAR_YsT             'T'

typedef char WZYsHedgeFlagType;

///////////////////////////////////
// WZYsOrderStateType: Ò×Ê¢Î¯ÍÐ×´Ì¬ÀàÐÍ
///////////////////////////////////
//ÖÕ¶ËÌá½»
#define WZ_CHAR_YsSubmit        '0'
//ÒÑÊÜÀí
#define WZ_CHAR_YsAccept        '1'
//²ßÂÔ´ý´¥·¢
#define WZ_CHAR_YsTriggering    '2'
//½»Ò×Ëù´ý´¥·¢
#define WZ_CHAR_YsExctriggering '3'
//ÒÑÅÅ¶Ó
#define WZ_CHAR_YsQueued        '4'
//²¿·Ö³É½»
#define WZ_CHAR_YsPartFinished  '5'
//ÍêÈ«³É½»
#define WZ_CHAR_YsFinished      '6'
//´ý³·Ïû(ÅÅ¶ÓÁÙÊ±×´Ì¬)
#define WZ_CHAR_YsCanceling     '7'
//´ýÐÞ¸Ä(ÅÅ¶ÓÁÙÊ±×´Ì¬)
#define WZ_CHAR_YsModifying     '8'
//ÍêÈ«³·µ¥
#define WZ_CHAR_YsCanceled      '9'
//ÒÑ³·Óàµ¥
#define WZ_CHAR_YsLeftDeleted   'A'
//Ö¸ÁîÊ§°Ü
#define WZ_CHAR_YsFail          'B'
//²ßÂÔÉ¾³ý
#define WZ_CHAR_YsDeleted       'C'
//ÒÑ¹ÒÆð
#define WZ_CHAR_YsSuppended     'D'
//µ½ÆÚÉ¾³ý
#define WZ_CHAR_YsDeletedForExpire 'E'
//ÒÑÉúÐ§¡ª¡ªÑ¯¼Û³É¹¦
#define WZ_CHAR_YsEffect        'F'
//ÒÑÉêÇë¡ª¡ªÐÐÈ¨¡¢ÆúÈ¨¡¢Ì×ÀûµÈÉêÇë³É¹¦
#define WZ_CHAR_YsApply         'G'

typedef char WZYsOrderStateType;

///////////////////////////////////
// WZYsOrderTypeType: Ò×Ê¢Î¯ÍÐÀàÐÍ
///////////////////////////////////
//ÊÐ¼Û
#define WZ_CHAR_YsMarket        '1'
//ÏÞ¼Û
#define WZ_CHAR_YsLimit         '2'

typedef char WZYsOrderTypeType;

///////////////////////////////////
// WZYsPositionEffectType: Ò×Ê¢¿ªÆ½ÀàÐÍ
///////////////////////////////////
//Æ½²Ö
#define WZ_CHAR_YsClose         'C'
//²»·Ö¿ªÆ½
#define WZ_CHAR_YsNon           'N'
//¿ª²Ö
#define WZ_CHAR_YsOpen          'O'
//Æ½µ±ÈÕ
#define WZ_CHAR_YsCloseToday    'T'

typedef char WZYsPositionEffectType;

///////////////////////////////////
// WZYsSideTypeType: Ò×Ê¢ÂòÂôÀàÐÍ
///////////////////////////////////
//Ë«±ß
#define WZ_CHAR_YsAll           'A'
//ÂòÈë
#define WZ_CHAR_YsBuy           'B'
//ÎÞ
#define WZ_CHAR_YsNon           'N'
//Âô³ö
#define WZ_CHAR_YsSell          'S'

typedef char WZYsSideTypeType;

///////////////////////////////////
// WZYsTimeConditionType: Ò×Ê¢Î¯ÍÐÓÐÐ§ÀàÐÍ
///////////////////////////////////
//µ±ÈÕÓÐÐ§
#define WZ_CHAR_YsGFD           '0'
//³·ÏúÇ°ÓÐÐ§
#define WZ_CHAR_YsGTC           '1'
//Ö¸¶¨ÈÕÆÚÇ°ÓÐÐ§
#define WZ_CHAR_YsGTD           '2'
//FAK»òIOC
#define WZ_CHAR_YsFAK           '3'
//FOK
#define WZ_CHAR_YsFOK           '4'

typedef char WZYsTimeConditionType;

#endif
