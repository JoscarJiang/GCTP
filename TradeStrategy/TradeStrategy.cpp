#include "TradeStrategy.h"
#include "../RecordManagement/InstrumentData.h"
TradeStrategy::TradeStrategy(){

}
TradeStrategy::~TradeStrategy() {

}
void TradeStrategy::trade() {
	// �����Լ���Ҫ���׵ĺ�Լ
	TThostFtdcInstrumentIDType A_name = "au1806";
	TThostFtdcInstrumentIDType B_name = "ni1807";
	InstrumentData A(A_name);
	InstrumentData B(B_name);
	
	// ���뽻�ײ���ѭ��
	while (1) {

	}
}