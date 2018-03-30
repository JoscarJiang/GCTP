#include <iostream>
#include <time.h>
#include <thread>
#include <chrono>
#include <windows.h>
#include "CustomTradeSpi.h"
#include "CommandManagement/InsertOrderCommand.h"
#include "CommandManagement/LoginCommand.h"
#include "CommandManagement/ComfirmSettlementCommand.h"
#include "CommandManagement/QryInstrumentCommand.h"
#include "CommandManagement/QryTradingAccountCommand.h"
#include "CommandManagement/QryPositionCommand.h"
#include "personInfo.h"

LARGE_INTEGER litmp;
LONGLONG QPart1, QPart2;
double dfMinus, dfFreq, dfTim;

// 会话参数
TThostFtdcFrontIDType	trade_front_id;	    //前置编号
TThostFtdcSessionIDType	session_id;     	//会话编号
TThostFtdcOrderRefType	order_ref;	        //报单引用

CustomTradeSpi::CustomTradeSpi() {
	requestID = 0;
	std::cout << ">>> create CustomTraderSpi" << std::endl;
}

CustomTradeSpi:: ~CustomTradeSpi() {
	std::cout << ">>> delete CustomTraderSpi" << std::endl;
	CQueue->working = false; //停止执行指令管理进程
}

/****************************Api交易函数****************************************/
// 登录(Init自动调用)
void CustomTradeSpi::OnFrontConnected()
{
	std::cout << "===== Trader Api Connection Established =====" << std::endl;
	CThostFtdcReqUserLoginField *loginReq = new CThostFtdcReqUserLoginField;
	strcpy_s(loginReq->BrokerID, BrokerID);
	strcpy_s(loginReq->UserID, InvestorID);
	strcpy_s(loginReq->Password, InvestorPassword);
	std::shared_ptr<ApiCommand> command = std::make_shared<LoginCommand>(TradeApi, loginReq, requestID);
	CQueue->addCommand(command);
}

// 结算确认（手动调用，下同）
void CustomTradeSpi::settlementConfirmReq() {
	// 投资者结算结果确认
	CThostFtdcSettlementInfoConfirmField *settlementConfirmReq = new CThostFtdcSettlementInfoConfirmField;
	strcpy_s(settlementConfirmReq->BrokerID, BrokerID);
	strcpy_s(settlementConfirmReq->InvestorID, InvestorID);
	std::shared_ptr<ApiCommand> command = std::make_shared<ComfirmSettlementCommand>(TradeApi, settlementConfirmReq, requestID);
	CQueue->addCommand(command);
}

// 查询合约
void CustomTradeSpi::instrumentReq(TThostFtdcInstrumentIDType InstrumentID) {
	// 请求查询合约
	CThostFtdcQryInstrumentField *instrumentReq = new CThostFtdcQryInstrumentField;
	strcpy_s(instrumentReq->InstrumentID, InstrumentID);
	std::shared_ptr<ApiCommand> command = std::make_shared<QryInstrumentCommand>(TradeApi, instrumentReq, requestID);
	CQueue->addCommand(command);
}

// 查询资金账户
void CustomTradeSpi::tradingAccountReq() {
	// 请求查询投资者资金账户
	CThostFtdcQryTradingAccountField *tradingAccountReq = new CThostFtdcQryTradingAccountField;
	strcpy_s(tradingAccountReq->BrokerID, BrokerID);
	strcpy_s(tradingAccountReq->InvestorID, InvestorID);
	std::shared_ptr<ApiCommand> command = std::make_shared<QryTradingAccountCommand>(TradeApi, tradingAccountReq, requestID);
	CQueue->addCommand(command);
}

// 查询客户总体持仓情况
void CustomTradeSpi::postionReq(TThostFtdcInstrumentIDType InstrumentID){
	// 请求查询投资者持仓
	CThostFtdcQryInvestorPositionField *postionReq = new CThostFtdcQryInvestorPositionField;
	strcpy_s(postionReq->BrokerID, BrokerID);
	strcpy_s(postionReq->InvestorID, InvestorID);
	strcpy_s(postionReq->InstrumentID, InstrumentID);
	std::shared_ptr<ApiCommand> command = std::make_shared<QryPositionCommand>(TradeApi, postionReq, requestID);
	CQueue->addCommand(command);
}

// 报单插入
void CustomTradeSpi::reqOrderInsert(TThostFtdcInstrumentIDType instrumentID, TThostFtdcPriceType price, TThostFtdcVolumeType volume, TThostFtdcDirectionType direction)
{
	CThostFtdcInputOrderField orderInsertReq;
	memset(&orderInsertReq, 0, sizeof(orderInsertReq));
	///经纪公司代码
	strcpy_s(orderInsertReq.BrokerID, BrokerID);
	///投资者代码
	strcpy_s(orderInsertReq.InvestorID, InvestorID);
	///合约代码
	strcpy_s(orderInsertReq.InstrumentID, instrumentID);
	///报单引用
	strcpy_s(orderInsertReq.OrderRef, order_ref);
	///报单价格条件: 限价
	orderInsertReq.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
	///买卖方向: 
	orderInsertReq.Direction = direction;
	///组合开平标志: 开仓
	orderInsertReq.CombOffsetFlag[0] = THOST_FTDC_OF_Open;
	///组合投机套保标志
	orderInsertReq.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;
	///价格
	orderInsertReq.LimitPrice = price;
	///数量：1
	orderInsertReq.VolumeTotalOriginal = volume;
	///有效期类型: 当日有效
	orderInsertReq.TimeCondition = THOST_FTDC_TC_GFD;
	///成交量类型: 任何数量
	orderInsertReq.VolumeCondition = THOST_FTDC_VC_AV;
	///最小成交量: 1
	orderInsertReq.MinVolume = 1;
	///触发条件: 立即
	orderInsertReq.ContingentCondition = THOST_FTDC_CC_Immediately;
	///强平原因: 非强平
	orderInsertReq.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
	///自动挂起标志: 否
	orderInsertReq.IsAutoSuspend = 0;
	///用户强评标志: 否
	orderInsertReq.UserForceClose = 0;

	static int requestID = 0; // 请求编号

	//测试时间起始位置
	QueryPerformanceFrequency(&litmp);
	dfFreq = (double)litmp.QuadPart;// 获得计数器的时钟频率 
	QueryPerformanceCounter(&litmp);
	QPart1 = litmp.QuadPart;// 获得初始值 
	std::shared_ptr<ApiCommand> command = std::make_shared<InsertOrderCommand>(TradeApi, &orderInsertReq, requestID);
	CQueue->addCommand(command);
}

// 登录成功后回复
void CustomTradeSpi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (!isErrorRspInfo(pRspInfo))
	{
		std::cout << "===== Account Login Successfully =====" << std::endl;
		loginFlag = true;
		std::cout << ">>> Date: " << pRspUserLogin->TradingDay << std::endl;
		std::cout << ">>> Login Time: " << pRspUserLogin->LoginTime << std::endl;
		std::cout << ">>> BrokerID:  " << pRspUserLogin->BrokerID << std::endl;
		std::cout << ">>> UserID:  " << pRspUserLogin->UserID << std::endl;

		// 保存会话参数
		trade_front_id = pRspUserLogin->FrontID;
		session_id = pRspUserLogin->SessionID;
		strcpy_s(order_ref, pRspUserLogin->MaxOrderRef);
	}
	else {
		std::cout << "===== Account Login Fail =====" << std::endl;
	}
}

// 结算确认成功
void CustomTradeSpi::OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	if (!isErrorRspInfo(pRspInfo))
	{
		std::cout << "===== 投资者结算结果确认成功 =====" << std::endl;
		std::cout << "确认日期： " << pSettlementInfoConfirm->ConfirmDate << std::endl;
		std::cout << "确认时间： " << pSettlementInfoConfirm->ConfirmTime << std::endl;
	}
};

// 查询合约成功则查询资金账户
void CustomTradeSpi::OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	if (!isErrorRspInfo(pRspInfo) && pInstrument)
	{
		std::cout << "===== 查询合约结果成功 =====" << std::endl;
		std::cout << "交易所代码： " << pInstrument->ExchangeID << std::endl;
		std::cout << "合约代码： " << pInstrument->InstrumentID << std::endl;
		std::cout << "合约在交易所的代码： " << pInstrument->ExchangeInstID << std::endl;
		std::cout << "执行价： " << pInstrument->StrikePrice << std::endl;
		std::cout << "到期日： " << pInstrument->EndDelivDate << std::endl;
		std::cout << "当前交易状态： " << pInstrument->IsTrading << std::endl;
	}
}

// 查询资金账户成功则查询客户总体持仓情况
void CustomTradeSpi::OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	if (!isErrorRspInfo(pRspInfo) && pTradingAccount) {
		std::cout << "=====查询投资者资金账户成功=====" << std::endl;
		std::cout << "投资者账号： " << pTradingAccount->AccountID << std::endl;
		std::cout << "可用资金： " << pTradingAccount->Available << std::endl;
		std::cout << "可取资金： " << pTradingAccount->WithdrawQuota << std::endl;
		std::cout << "当前保证金: " << pTradingAccount->CurrMargin << std::endl;
		std::cout << "平仓盈亏： " << pTradingAccount->CloseProfit << std::endl;
		info_record->update_person_info(pTradingAccount);

	}
}

// 查询客户总体持仓情况则报单录入
void CustomTradeSpi::OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	if (!isErrorRspInfo(pRspInfo))
	{
		std::cout << "===== 查询投资者持仓成功 =====" << std::endl;
		if (pInvestorPosition)
		{
			std::cout << "合约代码： " << pInvestorPosition->InstrumentID << std::endl;
			std::cout << "开仓价格： " << pInvestorPosition->OpenAmount << std::endl;
			std::cout << "开仓量： " << pInvestorPosition->OpenVolume << std::endl;
			std::cout << "开仓方向： " << pInvestorPosition->PosiDirection << std::endl;
			std::cout << "占用保证金：" << pInvestorPosition->UseMargin << std::endl;
		}
		else
			std::cout << "----->该合约未持仓" << std::endl;

		//reqOrderInsert(TradeInstrumentID, price, 1, THOST_FTDC_D_Buy);
		//StrategyCheckAndTrade(TradeInstrumentID, this);

	}
}




void CustomTradeSpi::OnFrontDisconnected(int nReason)
{
	std::cerr << "===== FrontDisconnected =====" << std::endl;
	std::cerr << "Error Code： " << nReason << std::endl;
}

void CustomTradeSpi::OnHeartBeatWarning(int nTimeLapse)
{
	std::cerr << "===== HeartBeatWarning =====" << std::endl;
	std::cerr << "The Time Last Connected： " << nTimeLapse << std::endl;
}


void CustomTradeSpi::OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (!isErrorRspInfo(pRspInfo))
	{
		loginFlag = false; // 登出就不能再交易了 
		std::cout << "===== Account Logout Successfully =====" << std::endl;
		std::cout << "BrokerID: " << pUserLogout->BrokerID << std::endl;
		std::cout << "UserID: " << pUserLogout->UserID << std::endl;
	}
	else {
		std::cout << "===== Account Logout Fail =====" << std::endl;
	}
}


void CustomTradeSpi::OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	if (!isErrorRspInfo(pRspInfo))
	{
		std::cout << "===== 报单录入成功 =====" << std::endl;
		std::cout << "合约代码： " << pInputOrder->InstrumentID << std::endl;
		std::cout << "价格： " << pInputOrder->LimitPrice << std::endl;
		std::cout << "数量： " << pInputOrder->VolumeTotalOriginal << std::endl;
		std::cout << "开仓方向： " << pInputOrder->Direction << std::endl;
	}
}

void CustomTradeSpi::OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	if (!isErrorRspInfo(pRspInfo))
	{
		std::cout << "=====报单操作成功=====" << std::endl;
		std::cout << "合约代码： " << pInputOrderAction->InstrumentID << std::endl;
		std::cout << "操作标志： " << pInputOrderAction->ActionFlag;
	}
}


void CustomTradeSpi::OnRspQryInvestorPositionDetail(CThostFtdcInvestorPositionDetailField *pInvestorPositionDetail, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	std::cout << "=====InvestorPositionDetail=====" << std::endl;
	if (pInvestorPositionDetail) {
		//存入
		//std::cout << "=====InvestorPositionDetail=====" << std::endl;
		std::cout << pInvestorPositionDetail->InstrumentID << std::endl;
	}
	//if (pRspInfo) {
		//存入
	isErrorRspInfo(pRspInfo);
	//}
	//存入nRequestID

	//
	//if(bIsLast){
	//结束
	//}
	
}; //未写
void OnRspQrySettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};//未写

void CustomTradeSpi::OnRtnOrder(CThostFtdcOrderField *pOrder) {
	char str[10];
	sprintf_s(str, "%d", pOrder->OrderSubmitStatus);
	int orderState = atoi(str) - 48;	//报单状态0=已经提交，3=已经接受

	std::cout << "=====收到报单应答=====" << std::endl;

	if ((pOrder->FrontID == trade_front_id) &&
		(pOrder->SessionID == session_id) &&
		(strcmp(pOrder->OrderRef, order_ref) == 0)) // if my order?
	{
		if ((pOrder->OrderStatus != THOST_FTDC_OST_PartTradedNotQueueing) &&
			(pOrder->OrderStatus != THOST_FTDC_OST_Canceled) &&
			(pOrder->OrderStatus != THOST_FTDC_OST_AllTraded)) // if trading order?
		{
			std::cout << "--->>> 等待成交中！" << std::endl;
			//reqOrderAction(pOrder); // 这里可以撤单
			//reqUserLogout(); // 登出测试
		}
		else if (pOrder->OrderStatus == THOST_FTDC_OST_Canceled)
			std::cout << "--->>> 撤单成功！" << std::endl;
	}
}


///成交通知 jc
void CustomTradeSpi::OnRtnTrade(CThostFtdcTradeField *pTrade)
{
	QueryPerformanceCounter(&litmp);
	QPart2 = litmp.QuadPart;//获得中止值 
	dfMinus = (double)(QPart2 - QPart1);
	dfTim = dfMinus / dfFreq;// 获得对应的时间值，单位为秒  
	std::cout << "交易持续时间："<< dfTim << std::endl;
	std::cout << "=====报单成功成交=====" << std::endl;
	std::cout << "成交时间： " << pTrade->TradeTime << std::endl;
	std::cout << "合约代码： " << pTrade->InstrumentID << std::endl;
	std::cout << "成交价格： " << pTrade->Price << std::endl;
	std::cout << "成交量： " << pTrade->Volume << std::endl;
	std::cout << "开平仓方向： " << pTrade->Direction << std::endl;
	info_record->record_trade_info(pTrade);
}

///报单录入错误回报 
void CustomTradeSpi::OnErrRtnOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo) {
	if (pInputOrder) {
		std::cout << pInputOrder->InstrumentID << std::endl;
	}
	isErrorRspInfo(pRspInfo);
};

///报单操作错误回报 
void CustomTradeSpi::OnErrRtnOrderAction(CThostFtdcOrderActionField *pOrderAction, CThostFtdcRspInfoField *pRspInfo) {
	if(pOrderAction)
		std::cout << pOrderAction->OrderSysID << std::endl;
	isErrorRspInfo(pRspInfo);
};

///合约交易状态通知 jc 
void CustomTradeSpi::OnRtnInstrumentStatus(CThostFtdcInstrumentStatusField *pInstrumentStatus) { 
	if (pInstrumentStatus) {
	//std::cout << "ExchangeID: " << pInstrumentStatus->ExchangeID << std::endl;///交易所代码
	//std::cout << "ExchangeInstID: " << pInstrumentStatus->ExchangeInstID << std::endl;///合约在交易所的代码
	//std::cout << pInstrumentStatus->SettlementGroupID << std::endl;///结算组代码
	std::cout << "InstrumentID: " << pInstrumentStatus->InstrumentID << std::endl;///合约代码
	std::cout << "Status: " << pInstrumentStatus->InstrumentStatus << std::endl;	///合约交易状态
	//std::cout << pInstrumentStatus->TradingSegmentSN << std::endl;///交易阶段编号
	///自动切换
	//#define THOST_FTDC_IER_Automatic '1'
	///手动切换
	//#define THOST_FTDC_IER_Manual '2'
	///熔断
	//#define THOST_FTDC_IER_Fuse '3'

	/*这里被注释了*/
	//std::cout << "The Time into The Status: " << pInstrumentStatus->EnterTime << std::endl;///进入本状态时间
	//std::cout << "Reason: " << pInstrumentStatus->EnterReason << std::endl;///进入本状态原因

	}
};

void CustomTradeSpi::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	isErrorRspInfo(pRspInfo);
}

bool CustomTradeSpi::isErrorRspInfo(CThostFtdcRspInfoField *pRspInfo)
{
	bool bResult = pRspInfo && (pRspInfo->ErrorID != 0);
	if (bResult)
		std::cerr << "Error Retuened --->>> ErrorID=" << pRspInfo->ErrorID << ", ErrorMsg=" << pRspInfo->ErrorMsg << std::endl;
	return bResult;
}

