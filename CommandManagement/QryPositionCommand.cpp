#include "QryPositionCommand.h"

QryPositionCommand::QryPositionCommand(CThostFtdcTraderApi *api, CThostFtdcQryInvestorPositionField *accountField,
	int &requestID) :ApiCommand(requestID, api){
	this->accountField = accountField;
}

int QryPositionCommand::execute(){
	int rt = api->ReqQryInvestorPosition(accountField, requestID);
	if (!rt)
		std::cout << ">>>>>>����Ͷ���ֲֲ߳�ѯ����ɹ�" << std::endl;
	else
		std::cerr << "--->>>����Ͷ���ֲֲ߳�ѯ����ʧ��" << std::endl;

	return rt;
}