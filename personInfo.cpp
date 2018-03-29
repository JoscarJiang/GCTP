#include "personInfo.h"

TThostFtdcBrokerIDType BrokerID = "9999";                          // 模拟经纪商代码
TThostFtdcInvestorIDType InvestorID = "112502";                    // 投资者账户名
TThostFtdcPasswordType InvestorPassword = "19960122";              // 投资者密码

																	// 行情参数
CThostFtdcMdApi *MarketApi = nullptr;                              // 行情指针
char MdFrontAddr[] = "tcp://180.168.146.187:10010";                // 模拟行情前置地址
char Inid[4][7] = { "TF1706", "zn1705", "au1806", "CF705" };
char *InstrumentID[] = { Inid[0], Inid[1], Inid[2], Inid[3] };     // 行情合约代码列表，中、上、大、郑交易所各选一种
int instrumentNum = 4;                                             // 行情合约订阅数量

																	// 交易参数
CThostFtdcTraderApi *TradeApi = nullptr;                           // 交易指针
char TradeFrontAddr[] = "tcp://180.168.146.187:10001";             // 模拟交易前置地址
TThostFtdcInstrumentIDType TradeInstrumentID = "au1806";           // 所交易的合约代码
TThostFtdcDirectionType gTradeDirection = THOST_FTDC_D_Sell;       // 买卖方向
TThostFtdcPriceType gLimitPrice = 22735;                           // 交易价格

CommandQueue* CQueue = new CommandQueue;  
InfoRecord* info_record = new InfoRecord;
RecordManagement* RManagement = new RecordManagement;
CustomTradeSpi *TradeSpi = new CustomTradeSpi;

