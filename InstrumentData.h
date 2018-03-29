#pragma once
/*
�����ƣ�   InstrumentData
���ã�     ��¼�г���CustomMdSpi�ࣩ���صļ۸�������Ϣ�����ṩ��TradeStrategy
ʹ��λ�ã� CustomMdSpi�ࡢTradeStrategy��
�����ˣ�   ��俴�
���ڣ�     2018.3.28
*/

#include "CTPapi\ThostFtdcUserApiStruct.h"
#include <stack>
#include <mutex>
class InstrumentData {
public:
	InstrumentData(TThostFtdcInstrumentIDType InstrumentID);
	~InstrumentData();

	// CustomMdSpi���ڽ������ݣ�����data_history
	void update_data(CThostFtdcDepthMarketDataField *input);
	// TradeStrategy���ڵ��û�ȡdata_history������ ע���˴�����չ��Ҫ��ȡ��������
	CThostFtdcDepthMarketDataField* get_data();
	

	std::stack<CThostFtdcDepthMarketDataField *> data_history; //�洢������ʷ
	TThostFtdcInstrumentIDType InstrumentID; // �洢��������IntrumentID
private:
	std::mutex InstrMutex;
};
