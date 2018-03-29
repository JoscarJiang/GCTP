#pragma once
#include "ApiCommand.h"

class QryInstrumentCommand :public ApiCommand {
public:
	QryInstrumentCommand(CThostFtdcTraderApi *api, CThostFtdcQryInstrumentField *QryInstrumentField, int &requestID);
	int execute() override;
private:
	CThostFtdcQryInstrumentField * QryInstrumentField;
};