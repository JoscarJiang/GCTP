#include "InsertOrderCommand.h"

InsertOrderCommand::InsertOrderCommand(CThostFtdcTraderApi *api, CThostFtdcInputOrderField *orderField,
	int &requestID) :ApiCommand(requestID, api){
	this->orderField = orderField;
}

int InsertOrderCommand::execute(){
	int rt = api->ReqOrderInsert(orderField, requestID);

	if (!rt)
		std::cout << ">>>>>>���ͱ���¼������ɹ�" << std::endl;
	else
		std::cerr << "--->>>���ͱ���¼������ʧ��" << std::endl;
	return rt;
}