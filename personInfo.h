#pragma once
//头文件定义
#include "CustomMdSpi.h"
#include "CustomTradeSpi.h"
#include "RecordManagement/InfoRecord.h"
#include "CommandManagement/CommandQueue.h"
#include "RecordManagement/RecordManagement.h"

#ifndef _a
#define _a
extern TThostFtdcBrokerIDType BrokerID;                           // 模拟经纪商代码
extern TThostFtdcInvestorIDType InvestorID;                       // 投资者账户名
extern TThostFtdcPasswordType InvestorPassword;                   // 投资者密码
#endif

#ifndef _b
#define _b
// 行情参数
extern CThostFtdcMdApi *MarketApi;                                // 行情指针
extern char MdFrontAddr[];                                        // 模拟行情前置地址
extern char Inid[2][31];
extern char *InstrumentID[];                                      // 行情合约代码列表，中、上、大、郑交易所各选一种
extern int instrumentNum;                                         // 行情合约订阅
#endif

#ifndef _c
#define _c
// 交易参数
extern CThostFtdcTraderApi *TradeApi;                             // 交易指针
extern char TradeFrontAddr[];                                     // 模拟交易前置地址
extern TThostFtdcInstrumentIDType TradeInstrumentID;              // 所交易的合约代码
extern TThostFtdcDirectionType gTradeDirection;                   // 买卖方向
extern TThostFtdcPriceType gLimitPrice;                           // 交易价格
extern CustomTradeSpi *TradeSpi;
#endif

#ifndef _d
#define _d
// 命令队列
extern CommandQueue* CQueue;                                      // 命令队列
extern RecordManagement* RManagement;                             // 记录管理接口
#endif

#ifndef _e
#define _e
// 记录和打印队列
extern InfoRecord* info_record;                                   
#endif


