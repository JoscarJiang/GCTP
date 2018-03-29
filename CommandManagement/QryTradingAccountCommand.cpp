#include "QryTradingAccountCommand.h"

QryTradingAccountCommand::QryTradingAccountCommand(CThostFtdcTraderApi *api, CThostFtdcQryTradingAccountField *accountField,
	int &requestID) :ApiCommand(requestID, api){
	this->accountField = accountField;
}

int QryTradingAccountCommand::execute(){
	int rt = api->ReqQryTradingAccount(accountField, requestID);
	if (!rt)
		std::cout << ">>>>>>����Ͷ�����ʽ��˻���ѯ����ɹ�, requestID is " << requestID << std::endl;
	else {
		std::cerr << "--->>>����Ͷ�����ʽ��˻���ѯ����ʧ��, requestID is " << requestID << std::endl;
	}
	return rt;
}