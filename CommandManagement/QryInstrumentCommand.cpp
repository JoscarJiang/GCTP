#include "QryInstrumentCommand.h"

QryInstrumentCommand::QryInstrumentCommand(CThostFtdcTraderApi *api, CThostFtdcQryInstrumentField *QryInstrumentField, int &requestID)
	:ApiCommand(requestID, api) {
	this->QryInstrumentField = QryInstrumentField;
}

int QryInstrumentCommand::execute() {
	int rt = api->ReqQryInstrument(QryInstrumentField, requestID);

	if (!rt)
		std::cout << ">>>���ͺ�Լ��ѯ����ɹ�, requestID is" << requestID << std::endl;
	else
		std::cout << ">>>���ͺ�Լ��ѯ����ʧ��, requestID is" << requestID << std::endl;
	return rt;
}