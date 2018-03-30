#include "TradeStrategy.h"
#include <Windows.h>

TradeStrategy::TradeStrategy(){

}
TradeStrategy::~TradeStrategy() {

}
void TradeStrategy::trade() {
	// 定义自己想要交易的合约
	TThostFtdcInstrumentIDType A_name = "au1806";
	TThostFtdcInstrumentIDType B_name = "ni1807";
	
	// 在RManagement中创建InstrumentData实例
	RManagement->create_instr_object(A_name);//创建instrument 对象， 接收消息， 发出交易等
	RManagement->create_instr_object(B_name);
	
	// 发送行情数据请求，只需要调用一次
	RManagement->subscribe_depth_data();     //获取市场信息，一定要调用
	
	TradeSpi->settlementConfirmReq();
	TradeSpi->instrumentReq(A_name);
	TradeSpi->instrumentReq(B_name);
	TradeSpi->tradingAccountReq();
	TradeSpi->postionReq(A_name);
	TradeSpi->postionReq(B_name);
	
	//TradeSpi->reqOrderInsert(A_name,300,1, THOST_FTDC_D_Buy);

	// 进入交易策略循环
	while (1) {
		CThostFtdcDepthMarketDataField* Temp_A = RManagement->fetch_data(A_name);
		if (Temp_A) {
			std::cout << Temp_A->BidPrice1;
			Sleep(1000);
		}
		else {
			std::cout<<"Return is NULL"<<std::endl;
			Sleep(1000);
		}
		
	}
}