#pragma once
/*
类名称：   RecordManagement
作用：     记录市场（CustomMdSpi类）返回的价格、量等信息，并更新相应的InstrumentData对象
使用位置： CustomMdSpi类、InstrumentData类
创建人：   姜淇川
日期：     2018.3.29
*/

#include "../CTPapi/ThostFtdcUserApiStruct.h"
#include "InstrumentData.h"
#include <vector>

class RecordManagement {
public:

	RecordManagement();
	~RecordManagement();

	//在InstrumentData里初始化使用，早于行情接口，不需要考虑instr_pool冲突
	void collect_instrID(InstrumentData* instr);
	
	//行情接口和InstrumentData类的数据通道，选择对应的Instrument对象更新数据
	void time_to_update_instr_data(CThostFtdcDepthMarketDataField *input);

	std::vector<InstrumentData*> instr_pool;
	CThostFtdcDepthMarketDataField *data;
};
