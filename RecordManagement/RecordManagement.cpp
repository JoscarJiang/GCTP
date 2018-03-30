#include <iostream>
#include "../personInfo.h"


RecordManagement::RecordManagement() {
	std::cout<<">>> RecordManagement has been established "<<std::endl;
}

RecordManagement::~RecordManagement() {
	//销毁vector里面所有的类对象
	for (std::vector<InstrumentData*>::iterator it = instr_pool.begin(); it != instr_pool.end(); it++) {
		if (NULL != *it) {
			delete *it;
			*it = NULL;
		}
	}
	instr_pool.clear();
	std::cout << ">>> RecordManagement has been destroyed " << std::endl;
}

void RecordManagement::create_instr_object(TThostFtdcInstrumentIDType InstrID) {
	InstrumentData* instr = new InstrumentData(InstrID);
	this->collect_instrID(instr); //会不会直接被销毁？？？
}

void RecordManagement::collect_instrID(InstrumentData* instr) {
	instr_pool.push_back(instr);
	std::cout << ">>> 推进队列： " <<instr->InstrumentID<< std::endl;
}

void RecordManagement::subscribe_depth_data() {
	int instrumentNum = this->instr_pool.size();      // 行情合约订阅数量
	char** InstrID = new char* [instrumentNum];       // 行情合约代码列表,有疑问 new (char*)[instrumentNum];报错
	for (int i=0;i < instrumentNum;++i) {
		std::cout << this->instr_pool[i]->InstrumentID << std::endl;
		InstrID[i] = this->instr_pool[i]->InstrumentID;
	}      
	while (!MarketIsConnected);                       //等待Market连接成功
	MarketSpi->subscribe_market_data(InstrID, instrumentNum);
	delete[] InstrID;
}


void RecordManagement::time_to_update_instr_data(CThostFtdcDepthMarketDataField *input) {
	for (unsigned int i = 0; i < instr_pool.size(); i++) {
		//test
		std::cout << input->InstrumentID << std::endl;
		std::cout << instr_pool[i]->InstrumentID << std::endl;
		//test
		if (!_stricmp(input->InstrumentID, instr_pool[i]->InstrumentID)) {
			instr_pool[i]->update_data(input);
		}
	}
}

CThostFtdcDepthMarketDataField* RecordManagement::fetch_data(TThostFtdcInstrumentIDType InstrID) {
	for (unsigned int i = 0; i < instr_pool.size(); i++) {
		if (!_stricmp(InstrID, instr_pool[i]->InstrumentID)) {
			if (instr_pool[i]->data_history.size() > 0) {
				return instr_pool[i]->get_data();
			}
		}
	}
	return NULL;
}