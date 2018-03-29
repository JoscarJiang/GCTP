#pragma once
#include "../CTPapi/ThostFtdcTraderApi.h"
#include <iostream>

//��װapi����Ľӿ�

class ApiCommand{
public:
	virtual ~ApiCommand();
	virtual int execute() = 0;
protected:
	ApiCommand(int &requestID, CThostFtdcTraderApi *api);
	int &requestID;
	CThostFtdcTraderApi *api;
};