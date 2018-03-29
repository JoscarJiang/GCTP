#include "RecordManagement.h"
#include <iostream>


RecordManagement::RecordManagement() {
	std::cout<<">>> RecordManagement has been established "<<std::endl;
}

RecordManagement::~RecordManagement() {
	std::cout << ">>> RecordManagement has been destroyed " << std::endl;
}

void RecordManagement::collect_instrID(InstrumentData* instr) {
	//test
	instr_pool.push_back(instr);
	std::cout << ">>> 推进队列： " <<instr->InstrumentID<< std::endl;
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