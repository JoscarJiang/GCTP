#pragma once

#include "../CTPapi/ThostFtdcUserApiDataType.h"
#include "../CTPapi/ThostFtdcUserApiStruct.h"
#include <queue>
#include <mutex>
#include <pthread.h>
#include <chrono>

//����һ���߳�ȥִ��д��
//�ö���ȥ��personhe��trade��Ϣ

typedef struct person_info {
	TThostFtdcBrokerIDType BrokerID;
	TThostFtdcAccountIDType AccountID;
	TThostFtdcMoneyType Available;//�����ʽ�
	TThostFtdcMoneyType	WithdrawQuota;//��ȡ�ʽ�
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
	bool flag;
};

void* write_record(void* _This);
