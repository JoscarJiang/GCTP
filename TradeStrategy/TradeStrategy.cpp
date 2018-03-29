#include "TradeStrategy.h"
#include "../InstrumentData.h"
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

	while (1) {
		CThostFtdcDepthMarketDataField *temp = new CThostFtdcDepthMarketDataField;
		TThostFtdcInstrumentIDType instrumentID  = "au1806";
		TThostFtdcPriceType price = 0;
		TThostFtdcVolumeType volume = 100 ;
		if (temp->LastPrice < temp->AskPrice1) {
			TradeSpi->reqOrderInsert(instrumentID, temp->AskPrice1, volume, THOST_FTDC_D_Buy);
		}
	}
}