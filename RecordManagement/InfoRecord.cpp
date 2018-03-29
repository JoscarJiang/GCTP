#include "infoRecord.h"
#include <fstream>
#include <iostream>


InfoRecord::InfoRecord() {
	flag = 1;
	my_info = new person_info;
}
InfoRecord::~InfoRecord() {
	flag = 0;
	delete my_info;
}
void InfoRecord::update_person_info(CThostFtdcTradingAccountField *pTradingAccount) {
	//Here we can add what you want 
	memcpy(my_info->AccountID, pTradingAccount->AccountID, sizeof(*(pTradingAccount->AccountID))); //这里大小对的吧
	my_info->Available = pTradingAccount->Available;
	my_info->WithdrawQuota = pTradingAccount->WithdrawQuota;
	printf("Update the Personal Info!/n");
}
void InfoRecord::record_trade_info(CThostFtdcTradeField *pTrade) {
	tradeInfo one_trade = new struct trade_info;
	one_trade->Direction = pTrade->Direction;
	memcpy(one_trade->TradeTime, pTrade->TradeTime, 9 * sizeof(char));
	one_trade->Price = pTrade->Price;
	one_trade->Volume = pTrade->Volume;
	memcpy(one_trade->InstrumentID, pTrade->InstrumentID, 31 * sizeof(char));
	
	std::lock_guard<std::mutex> lock(queueMutex);
	my_trade.push(one_trade);
}

void* write_record(void* _This) {
	//打开文件，写入
	std::ofstream out("D:\\out.txt", std::ios::app);
	InfoRecord* This_ = (InfoRecord*)_This;
	int num = 1;
	while (This_->flag) {
		if (!This_->my_trade.empty()) {
			std::lock_guard<std::mutex> lock(This_->queueMutex);
			auto trade = This_->my_trade.front();
			//自己可以加想写的东西
			out<<num<<std::endl;
			This_->my_trade.pop();
		}
	}
	out.close();
	return NULL;
}