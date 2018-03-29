#include "ComfirmSettlementCommand.h"

ComfirmSettlementCommand::ComfirmSettlementCommand(CThostFtdcTraderApi *api,
	CThostFtdcSettlementInfoConfirmField *comfirmField, int &requestID) :ApiCommand(requestID, api){
	this->comfirmField = comfirmField;
}

int ComfirmSettlementCommand::execute(){
	int rt = api->ReqSettlementInfoConfirm(comfirmField, requestID);
	if (!rt)
		std::cout << ">>>>>>����Ͷ���߽�����ȷ������ɹ�" << std::endl;
	else
		std::cerr << "--->>>����Ͷ���߽�����ȷ������ʧ��" << std::endl;
	return rt;
}