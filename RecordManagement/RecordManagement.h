#pragma once
/*
�����ƣ�   RecordManagement
���ã�     1. ��ʼ��������TradeStrategy�ഴ��InstrumentData�����б�
		   2. ��ȡ�г���Ϣ����¼�г���CustomMdSpi�ࣩ���صļ۸�������Ϣ����������Ӧ��InstrumentData������Ϣ����TradeStrategy���п��Ա����ò�ѯ
		   3. ִ�н��ײ����ؽ�����Ϣ�� ����TradeStrategy��ִ��Ҫ�󣬲�����CustomTradeSpi����Ӧ�ĺ���

ʹ��λ�ã� CustomMdSpi�ࡢCustomTradeSpi�ࡢTradeStrategy��
�����ˣ�   ��俴�
���ڣ�     2018.3.29
������£� 2018.3.30
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

	//��InstrumentData���ʼ��ʹ�ã���������ӿڣ�����Ҫ����instr_pool��ͻ
	void collect_instrID(InstrumentData* instr);
	
	//ѡ�����е�Instruments�����г�����
	void subscribe_depth_data();

	//����ӿں�InstrumentData�������ͨ����ѡ���Ӧ��Instrument�����������
	void time_to_update_instr_data(CThostFtdcDepthMarketDataField *input);

	//Strategy����û�ȡ����
	CThostFtdcDepthMarketDataField* fetch_data(TThostFtdcInstrumentIDType InstrID);

	std::vector<InstrumentData*> instr_pool;

	CThostFtdcDepthMarketDataField *data;

	bool MarketIsConnected = 0;
};
