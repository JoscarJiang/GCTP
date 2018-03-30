#pragma once
/*
类名称：   RecordManagement
作用：     1. 初始化：根据TradeStrategy类创建InstrumentData对象列表
		   2. 获取市场信息：记录市场（CustomMdSpi类）返回的价格、量等信息，并更新相应的InstrumentData对象信息，在TradeStrategy类中可以被调用查询
		   3. 执行交易并返回交易信息： 接收TradeStrategy的执行要求，并调用CustomTradeSpi类相应的函数

使用位置： CustomMdSpi类、CustomTradeSpi类、TradeStrategy类
创建人：   姜淇川
日期：     2018.3.29
最近更新： 2018.3.30
*/

#include "../CTPapi/ThostFtdcUserApiStruct.h"
#include "InstrumentData.h"
#include <vector>

class RecordManagement {
public:

	RecordManagement();
	~RecordManagement();

	//create InstrumentData object
	void create_instr_object(TThostFtdcInstrumentIDType InstrID);

	//在InstrumentData里初始化使用，早于行情接口，不需要考虑instr_pool冲突
	void collect_instrID(InstrumentData* instr);
	
	//选择所有的Instruments接收市场数据
	void subscribe_depth_data();

	//行情接口和InstrumentData类的数据通道，选择对应的Instrument对象更新数据
	void time_to_update_instr_data(CThostFtdcDepthMarketDataField *input);

	std::vector<InstrumentData*> instr_pool;

	CThostFtdcDepthMarketDataField *data;

	bool MarketIsConnected = 0;
};
