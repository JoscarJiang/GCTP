#include "TradeStrategy.h"
#include <Windows.h>

TradeStrategy::TradeStrategy(){

}
TradeStrategy::~TradeStrategy() {

}
void TradeStrategy::trade() {
	// �����Լ���Ҫ���׵ĺ�Լ
	TThostFtdcInstrumentIDType A_name = "au1806";
	TThostFtdcInstrumentIDType B_name = "ni1807";
	
	// ��RManagement�д���InstrumentDataʵ��
	RManagement->create_instr_object(A_name);//����instrument ���� ������Ϣ�� �������׵�
	RManagement->create_instr_object(B_name);
	
	// ����������������ֻ��Ҫ����һ��
	RManagement->subscribe_depth_data();     //��ȡ�г���Ϣ��һ��Ҫ����
	
	TradeSpi->settlementConfirmReq();
	TradeSpi->instrumentReq(A_name);
	TradeSpi->instrumentReq(B_name);
	TradeSpi->tradingAccountReq();
	TradeSpi->postionReq(A_name);
	TradeSpi->postionReq(B_name);
	
	//TradeSpi->reqOrderInsert(A_name,300,1, THOST_FTDC_D_Buy);

	// ���뽻�ײ���ѭ��
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