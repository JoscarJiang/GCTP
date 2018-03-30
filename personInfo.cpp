#include "personInfo.h"

TThostFtdcBrokerIDType BrokerID = "9999";                          // 模拟经纪商代码
TThostFtdcInvestorIDType InvestorID = "112502";                    // 投资者账户名
TThostFtdcPasswordType InvestorPassword = "19960122";              // 投资者密码

// 行情参数
CThostFtdcMdApi *MarketApi = nullptr;                              // 行情指针
CustomMdSpi *MarketSpi = new CustomMdSpi;
char MdFrontAddr[] = "tcp://180.168.146.187:10011";                // 模拟行情前置地址


// 交易参数
CThostFtdcTraderApi *TradeApi = nullptr;                           // 交易指针
char TradeFrontAddr[] = "tcp://180.168.146.187:10001";             // 模拟交易前置地址
TThostFtdcDirectionType gTradeDirection = THOST_FTDC_D_Sell;       // 买卖方向
TThostFtdcPriceType gLimitPrice = 22735;                           // 交易价格

CommandQueue* CQueue = new CommandQueue;  
InfoRecord* info_record = new InfoRecord;
RecordManagement* RManagement = new RecordManagement;
CustomTradeSpi *TradeSpi = new CustomTradeSpi;
