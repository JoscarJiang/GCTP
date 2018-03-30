#include "InstrumentData.h"
#include "../personInfo.h"

InstrumentData::InstrumentData(TThostFtdcInstrumentIDType InstrumentID){
	strcpy_s(this->InstrumentID,InstrumentID);
}

InstrumentData::~InstrumentData() {
	std::cout<<"get out"<<std::endl;
	// ���data_history���ͷſռ䣬�˴�Ҳ����ֱ��д��
	for (;!data_history.empty();) {
		CThostFtdcDepthMarketDataField *data = data_history.top();
		data_history.pop();
		delete data;
	}
}

void InstrumentData::update_data(CThostFtdcDepthMarketDataField *input) {
	
	CThostFtdcDepthMarketDataField *data = new CThostFtdcDepthMarketDataField;
	strcpy_s(data->InstrumentID, input->InstrumentID);
	data->Volume = input->Volume;
	data->AskPrice1 = input->AskPrice1;
	data->AskVolume1 = input->AskVolume1;
	data->BidPrice1 = input->BidVolume1;
	data->ClosePrice = input->ClosePrice;
	data->OpenPrice = input->OpenPrice;
	data->HighestPrice = input->HighestPrice;
	data->LowestPrice = input->LowestPrice;
	// д��ǰ�������뿪�������뿪��get_data���Է���data_history
	std::lock_guard<std::mutex> lock(InstrMutex);
	data_history.push(data);
}


CThostFtdcDepthMarketDataField* InstrumentData::get_data() {
	// ������ǰ�������뿪�������뿪��update_data���Է���data_history
	std::lock_guard<std::mutex> lock(InstrMutex);
	return data_history.top();
}