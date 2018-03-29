#pragma once
#include "ApiCommand.h"

class QryPositionCommand :public ApiCommand{
public:
	QryPositionCommand(CThostFtdcTraderApi *api, CThostFtdcQryInvestorPositionField *accountField, int &requestID);
	int execute() override;
private:
	CThostFtdcQryInvestorPositionField *accountField;
};