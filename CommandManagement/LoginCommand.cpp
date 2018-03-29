#include "LoginCommand.h"

LoginCommand::LoginCommand(CThostFtdcTraderApi *api, CThostFtdcReqUserLoginField *loginField, int &requestID)
	:ApiCommand(requestID, api) {
	this->loginField = loginField;
}

int LoginCommand::execute() {
	int rt = api->ReqUserLogin(loginField, requestID);
	if (!rt)
		std::cout << ">>>Login Request Succeed, requestID is" << requestID << std::endl;
	else
		std::cout << ">>>Login Request Fail, requestID is" << requestID << std::endl;
	return rt;
}