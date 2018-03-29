#pragma once
/*
类名称：   InstrumentData
作用：     记录市场（CustomMdSpi类）返回的价格、量等信息，并提供给TradeStrategy
使用位置： CustomMdSpi类、TradeStrategy类
创建人：   姜淇川
日期：     2018.3.28
*/

#include "CTPapi\ThostFtdcUserApiStruct.h"
#include <stack>
#include <mutex>
class InstrumentData {
public:
	InstrumentData(TThostFtdcInstrumentIDType InstrumentID);
	~InstrumentData();

	// CustomMdSpi类内接收数据，存入data_history
	void update_data(CThostFtdcDepthMarketDataField *input);
	// TradeStrategy类内调用获取data_history的数据 注：此处可扩展想要获取多少数据
	CThostFtdcDepthMarketDataField* get_data();
	

	std::stack<CThostFtdcDepthMarketDataField *> data_history; //存储数据历史
	TThostFtdcInstrumentIDType InstrumentID; // 存储该类代表的IntrumentID
private:
	std::mutex InstrMutex;
};
