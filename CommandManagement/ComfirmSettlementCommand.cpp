#include "ComfirmSettlementCommand.h"

ComfirmSettlementCommand::ComfirmSettlementCommand(CThostFtdcTraderApi *api,
	CThostFtdcSettlementInfoConfirmField *comfirmField, int &requestID) :ApiCommand(requestID, api){
	this->comfirmField = comfirmField;
}

int ComfirmSettlementCommand::execute(){
	int rt = api->ReqSettlementInfoConfirm(comfirmField, requestID);
	if (!rt)
		std::cout << ">>>>>>发送投资者结算结果确认请求成功" << std::endl;
	else
		std::cerr << "--->>>发送投资者结算结果确认请求失败" << std::endl;
	return rt;
}