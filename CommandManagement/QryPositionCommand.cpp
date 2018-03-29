#include "QryPositionCommand.h"

QryPositionCommand::QryPositionCommand(CThostFtdcTraderApi *api, CThostFtdcQryInvestorPositionField *accountField,
	int &requestID) :ApiCommand(requestID, api){
	this->accountField = accountField;
}

int QryPositionCommand::execute(){
	int rt = api->ReqQryInvestorPosition(accountField, requestID);
	if (!rt)
		std::cout << ">>>>>>发送投资者持仓查询请求成功" << std::endl;
	else
		std::cerr << "--->>>发送投资者持仓查询请求失败" << std::endl;

	return rt;
}