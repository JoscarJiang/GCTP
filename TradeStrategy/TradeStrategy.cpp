#include "TradeStrategy.h"
#include "../RecordManagement/InstrumentData.h"
TradeStrategy::TradeStrategy(){

}
TradeStrategy::~TradeStrategy() {

}
void TradeStrategy::trade() {
	// 定义自己想要交易的合约
	TThostFtdcInstrumentIDType A_name = "au1806";
	TThostFtdcInstrumentIDType B_name = "ni1807";
	InstrumentData A(A_name);
	InstrumentData B(B_name);
	
	// 进入交易策略循环
	while (1) {

	}
}