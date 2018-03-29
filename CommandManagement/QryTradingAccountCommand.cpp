#include "QryTradingAccountCommand.h"

QryTradingAccountCommand::QryTradingAccountCommand(CThostFtdcTraderApi *api, CThostFtdcQryTradingAccountField *accountField,
	int &requestID) :ApiCommand(requestID, api){
	this->accountField = accountField;
}

int QryTradingAccountCommand::execute(){
	int rt = api->ReqQryTradingAccount(accountField, requestID);
	if (!rt)
		std::cout << ">>>>>>发送投资者资金账户查询请求成功, requestID is " << requestID << std::endl;
	else {
		std::cerr << "--->>>发送投资者资金账户查询请求失败, requestID is " << requestID << std::endl;
	}
	return rt;
}