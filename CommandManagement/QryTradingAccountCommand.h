#pragma once

#include "ApiCommand.h"

class QryTradingAccountCommand :public ApiCommand{
public:
	QryTradingAccountCommand(CThostFtdcTraderApi *api, CThostFtdcQryTradingAccountField *accountField, int &requestID);
	int execute() override;
private:
	CThostFtdcQryTradingAccountField *accountField;
};