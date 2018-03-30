#include <iostream>
#include "../personInfo.h"


RecordManagement::RecordManagement() {
	std::cout<<">>> RecordManagement has been established "<<std::endl;
}

RecordManagement::~RecordManagement() {
	//����vector�������е������
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
	this->collect_instrID(instr); //�᲻��ֱ�ӱ����٣�����
}

void RecordManagement::collect_instrID(InstrumentData* instr) {
	instr_pool.push_back(instr);
	std::cout << ">>> �ƽ����У� " <<instr->InstrumentID<< std::endl;
}

void RecordManagement::subscribe_depth_data() {
	int instrumentNum = this->instr_pool.size();      // �����Լ��������
	char** InstrID = new char* [instrumentNum];       // �����Լ�����б�,������ new (char*)[instrumentNum];����
	for (int i=0;i < instrumentNum;++i) {
		std::cout << this->instr_pool[i]->InstrumentID << std::endl;
		InstrID[i] = this->instr_pool[i]->InstrumentID;
	}      
	while (!MarketIsConnected);                       //�ȴ�Market���ӳɹ�
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