#pragma once

#include"CTPapi\ThostFtdcUserApiDataType.h"
#include"CTPapi\ThostFtdcUserApiStruct.h"
#include <queue>
#include <mutex>
#include <pthread.h>
#include <chrono>

//单开一个线程去执行写入
//用队列去存personhe和trade信息

typedef struct person_info {
	TThostFtdcBrokerIDType BrokerID;
	TThostFtdcAccountIDType AccountID;
	TThostFtdcMoneyType Available;//可用资金
	TThostFtdcMoneyType	WithdrawQuota;//可取资金
}*personInfo;

typedef struct trade_info {
	TThostFtdcDateType	TradeTime;
	TThostFtdcInstrumentIDType	InstrumentID;
	TThostFtdcPriceType	Price;
	TThostFtdcDirectionType	Direction;
	TThostFtdcVolumeType	Volume;
}*tradeInfo;

class InfoRecord {
public:
	InfoRecord();
	~InfoRecord();
	void update_person_info(CThostFtdcTradingAccountField *pTradingAccount);
	void record_trade_info(CThostFtdcTradeField *pTrade);
	
	std::mutex queueMutex;
	personInfo my_info;
	std::queue<tradeInfo> my_trade;
};
void* write_record(void* _This);
