#include "InsertOrderCommand.h"

InsertOrderCommand::InsertOrderCommand(CThostFtdcTraderApi *api, CThostFtdcInputOrderField *orderField,
	int &requestID) :ApiCommand(requestID, api){
	this->orderField = orderField;
}

int InsertOrderCommand::execute(){
	int rt = api->ReqOrderInsert(orderField, requestID);

	if (!rt)
		std::cout << ">>>>>>发送报单录入请求成功" << std::endl;
	else
		std::cerr << "--->>>发送报单录入请求失败" << std::endl;
	return rt;
}