#pragma once
/*
类名称：   TradeStrategy
作用：     实现策略（目前仅供测试）
使用位置： CustomMdSpi类、InstrumentData类
创建人：   姜淇川
日期：     2018.3.29
*/
#include "../personInfo.h"

class TradeStrategy {
public:
	TradeStrategy();
	~TradeStrategy();
	void trade();
};
