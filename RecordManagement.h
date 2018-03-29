#pragma once
/*
�����ƣ�   RecordManagement
���ã�     ��¼�г���CustomMdSpi�ࣩ���صļ۸�������Ϣ����������Ӧ��InstrumentData����
ʹ��λ�ã� CustomMdSpi�ࡢInstrumentData��
�����ˣ�   ��俴�
���ڣ�     2018.3.29
*/

#include "CTPapi\ThostFtdcUserApiStruct.h"
#include "InstrumentData.h"
#include <vector>

class RecordManagement {
public:

	RecordManagement();
	~RecordManagement();

	//��InstrumentData���ʼ��ʹ�ã���������ӿڣ�����Ҫ����instr_pool��ͻ
	void collect_instrID(InstrumentData* instr);
	
	//����ӿں�InstrumentData�������ͨ����ѡ���Ӧ��Instrument�����������
	void time_to_update_instr_data(CThostFtdcDepthMarketDataField *input);

	std::vector<InstrumentData*> instr_pool;
	CThostFtdcDepthMarketDataField *data;
};
