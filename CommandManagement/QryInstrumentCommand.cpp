#include "QryInstrumentCommand.h"

QryInstrumentCommand::QryInstrumentCommand(CThostFtdcTraderApi *api, CThostFtdcQryInstrumentField *QryInstrumentField, int &requestID)
	:ApiCommand(requestID, api) {
	this->QryInstrumentField = QryInstrumentField;
}

int QryInstrumentCommand::execute() {
	int rt = api->ReqQryInstrument(QryInstrumentField, requestID);

	if (!rt)
		std::cout << ">>>发送合约查询请求成功, requestID is" << requestID << std::endl;
	else
		std::cout << ">>>发送合约查询请求失败, requestID is" << requestID << std::endl;
	return rt;
}