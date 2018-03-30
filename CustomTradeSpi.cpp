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

// �Ự����
TThostFtdcFrontIDType	trade_front_id;	    //ǰ�ñ��
TThostFtdcSessionIDType	session_id;     	//�Ự���
TThostFtdcOrderRefType	order_ref;	        //��������

CustomTradeSpi::CustomTradeSpi() {
	requestID = 0;
	std::cout << ">>> create CustomTraderSpi" << std::endl;
}

CustomTradeSpi:: ~CustomTradeSpi() {
	std::cout << ">>> delete CustomTraderSpi" << std::endl;
	CQueue->working = false; //ִֹͣ��ָ��������
}

/****************************Api���׺���****************************************/
// ��¼(Init�Զ�����)
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

// ����ȷ�ϣ��ֶ����ã���ͬ��
void CustomTradeSpi::settlementConfirmReq() {
	// Ͷ���߽�����ȷ��
	CThostFtdcSettlementInfoConfirmField *settlementConfirmReq = new CThostFtdcSettlementInfoConfirmField;
	strcpy_s(settlementConfirmReq->BrokerID, BrokerID);
	strcpy_s(settlementConfirmReq->InvestorID, InvestorID);
	std::shared_ptr<ApiCommand> command = std::make_shared<ComfirmSettlementCommand>(TradeApi, settlementConfirmReq, requestID);
	CQueue->addCommand(command);
}

// ��ѯ��Լ
void CustomTradeSpi::instrumentReq(TThostFtdcInstrumentIDType InstrumentID) {
	// �����ѯ��Լ
	CThostFtdcQryInstrumentField *instrumentReq = new CThostFtdcQryInstrumentField;
	strcpy_s(instrumentReq->InstrumentID, InstrumentID);
	std::shared_ptr<ApiCommand> command = std::make_shared<QryInstrumentCommand>(TradeApi, instrumentReq, requestID);
	CQueue->addCommand(command);
}

// ��ѯ�ʽ��˻�
void CustomTradeSpi::tradingAccountReq() {
	// �����ѯͶ�����ʽ��˻�
	CThostFtdcQryTradingAccountField *tradingAccountReq = new CThostFtdcQryTradingAccountField;
	strcpy_s(tradingAccountReq->BrokerID, BrokerID);
	strcpy_s(tradingAccountReq->InvestorID, InvestorID);
	std::shared_ptr<ApiCommand> command = std::make_shared<QryTradingAccountCommand>(TradeApi, tradingAccountReq, requestID);
	CQueue->addCommand(command);
}

// ��ѯ�ͻ�����ֲ����
void CustomTradeSpi::postionReq(TThostFtdcInstrumentIDType InstrumentID){
	// �����ѯͶ���ֲ߳�
	CThostFtdcQryInvestorPositionField *postionReq = new CThostFtdcQryInvestorPositionField;
	strcpy_s(postionReq->BrokerID, BrokerID);
	strcpy_s(postionReq->InvestorID, InvestorID);
	strcpy_s(postionReq->InstrumentID, InstrumentID);
	std::shared_ptr<ApiCommand> command = std::make_shared<QryPositionCommand>(TradeApi, postionReq, requestID);
	CQueue->addCommand(command);
}

// ��������
void CustomTradeSpi::reqOrderInsert(TThostFtdcInstrumentIDType instrumentID, TThostFtdcPriceType price, TThostFtdcVolumeType volume, TThostFtdcDirectionType direction)
{
	CThostFtdcInputOrderField orderInsertReq;
	memset(&orderInsertReq, 0, sizeof(orderInsertReq));
	///���͹�˾����
	strcpy_s(orderInsertReq.BrokerID, BrokerID);
	///Ͷ���ߴ���
	strcpy_s(orderInsertReq.InvestorID, InvestorID);
	///��Լ����
	strcpy_s(orderInsertReq.InstrumentID, instrumentID);
	///��������
	strcpy_s(orderInsertReq.OrderRef, order_ref);
	///�����۸�����: �޼�
	orderInsertReq.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
	///��������: 
	orderInsertReq.Direction = direction;
	///��Ͽ�ƽ��־: ����
	orderInsertReq.CombOffsetFlag[0] = THOST_FTDC_OF_Open;
	///���Ͷ���ױ���־
	orderInsertReq.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;
	///�۸�
	orderInsertReq.LimitPrice = price;
	///������1
	orderInsertReq.VolumeTotalOriginal = volume;
	///��Ч������: ������Ч
	orderInsertReq.TimeCondition = THOST_FTDC_TC_GFD;
	///�ɽ�������: �κ�����
	orderInsertReq.VolumeCondition = THOST_FTDC_VC_AV;
	///��С�ɽ���: 1
	orderInsertReq.MinVolume = 1;
	///��������: ����
	orderInsertReq.ContingentCondition = THOST_FTDC_CC_Immediately;
	///ǿƽԭ��: ��ǿƽ
	orderInsertReq.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
	///�Զ������־: ��
	orderInsertReq.IsAutoSuspend = 0;
	///�û�ǿ����־: ��
	orderInsertReq.UserForceClose = 0;

	static int requestID = 0; // ������

	//����ʱ����ʼλ��
	QueryPerformanceFrequency(&litmp);
	dfFreq = (double)litmp.QuadPart;// ��ü�������ʱ��Ƶ�� 
	QueryPerformanceCounter(&litmp);
	QPart1 = litmp.QuadPart;// ��ó�ʼֵ 
	std::shared_ptr<ApiCommand> command = std::make_shared<InsertOrderCommand>(TradeApi, &orderInsertReq, requestID);
	CQueue->addCommand(command);
}

// ��¼�ɹ���ظ�
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

		// ����Ự����
		trade_front_id = pRspUserLogin->FrontID;
		session_id = pRspUserLogin->SessionID;
		strcpy_s(order_ref, pRspUserLogin->MaxOrderRef);
	}
	else {
		std::cout << "===== Account Login Fail =====" << std::endl;
	}
}

// ����ȷ�ϳɹ�
void CustomTradeSpi::OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	if (!isErrorRspInfo(pRspInfo))
	{
		std::cout << "===== Ͷ���߽�����ȷ�ϳɹ� =====" << std::endl;
		std::cout << "ȷ�����ڣ� " << pSettlementInfoConfirm->ConfirmDate << std::endl;
		std::cout << "ȷ��ʱ�䣺 " << pSettlementInfoConfirm->ConfirmTime << std::endl;
	}
};

// ��ѯ��Լ�ɹ����ѯ�ʽ��˻�
void CustomTradeSpi::OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	if (!isErrorRspInfo(pRspInfo) && pInstrument)
	{
		std::cout << "===== ��ѯ��Լ����ɹ� =====" << std::endl;
		std::cout << "���������룺 " << pInstrument->ExchangeID << std::endl;
		std::cout << "��Լ���룺 " << pInstrument->InstrumentID << std::endl;
		std::cout << "��Լ�ڽ������Ĵ��룺 " << pInstrument->ExchangeInstID << std::endl;
		std::cout << "ִ�мۣ� " << pInstrument->StrikePrice << std::endl;
		std::cout << "�����գ� " << pInstrument->EndDelivDate << std::endl;
		std::cout << "��ǰ����״̬�� " << pInstrument->IsTrading << std::endl;
	}
}

// ��ѯ�ʽ��˻��ɹ����ѯ�ͻ�����ֲ����
void CustomTradeSpi::OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	if (!isErrorRspInfo(pRspInfo) && pTradingAccount) {
		std::cout << "=====��ѯͶ�����ʽ��˻��ɹ�=====" << std::endl;
		std::cout << "Ͷ�����˺ţ� " << pTradingAccount->AccountID << std::endl;
		std::cout << "�����ʽ� " << pTradingAccount->Available << std::endl;
		std::cout << "��ȡ�ʽ� " << pTradingAccount->WithdrawQuota << std::endl;
		std::cout << "��ǰ��֤��: " << pTradingAccount->CurrMargin << std::endl;
		std::cout << "ƽ��ӯ���� " << pTradingAccount->CloseProfit << std::endl;
		info_record->update_person_info(pTradingAccount);

	}
}

// ��ѯ�ͻ�����ֲ�����򱨵�¼��
void CustomTradeSpi::OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	if (!isErrorRspInfo(pRspInfo))
	{
		std::cout << "===== ��ѯͶ���ֲֳ߳ɹ� =====" << std::endl;
		if (pInvestorPosition)
		{
			std::cout << "��Լ���룺 " << pInvestorPosition->InstrumentID << std::endl;
			std::cout << "���ּ۸� " << pInvestorPosition->OpenAmount << std::endl;
			std::cout << "�������� " << pInvestorPosition->OpenVolume << std::endl;
			std::cout << "���ַ��� " << pInvestorPosition->PosiDirection << std::endl;
			std::cout << "ռ�ñ�֤��" << pInvestorPosition->UseMargin << std::endl;
		}
		else
			std::cout << "----->�ú�Լδ�ֲ�" << std::endl;

		//reqOrderInsert(TradeInstrumentID, price, 1, THOST_FTDC_D_Buy);
		//StrategyCheckAndTrade(TradeInstrumentID, this);

	}
}




void CustomTradeSpi::OnFrontDisconnected(int nReason)
{
	std::cerr << "===== FrontDisconnected =====" << std::endl;
	std::cerr << "Error Code�� " << nReason << std::endl;
}

void CustomTradeSpi::OnHeartBeatWarning(int nTimeLapse)
{
	std::cerr << "===== HeartBeatWarning =====" << std::endl;
	std::cerr << "The Time Last Connected�� " << nTimeLapse << std::endl;
}


void CustomTradeSpi::OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (!isErrorRspInfo(pRspInfo))
	{
		loginFlag = false; // �ǳ��Ͳ����ٽ����� 
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
		std::cout << "===== ����¼��ɹ� =====" << std::endl;
		std::cout << "��Լ���룺 " << pInputOrder->InstrumentID << std::endl;
		std::cout << "�۸� " << pInputOrder->LimitPrice << std::endl;
		std::cout << "������ " << pInputOrder->VolumeTotalOriginal << std::endl;
		std::cout << "���ַ��� " << pInputOrder->Direction << std::endl;
	}
}

void CustomTradeSpi::OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	if (!isErrorRspInfo(pRspInfo))
	{
		std::cout << "=====���������ɹ�=====" << std::endl;
		std::cout << "��Լ���룺 " << pInputOrderAction->InstrumentID << std::endl;
		std::cout << "������־�� " << pInputOrderAction->ActionFlag;
	}
}


void CustomTradeSpi::OnRspQryInvestorPositionDetail(CThostFtdcInvestorPositionDetailField *pInvestorPositionDetail, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	std::cout << "=====InvestorPositionDetail=====" << std::endl;
	if (pInvestorPositionDetail) {
		//����
		//std::cout << "=====InvestorPositionDetail=====" << std::endl;
		std::cout << pInvestorPositionDetail->InstrumentID << std::endl;
	}
	//if (pRspInfo) {
		//����
	isErrorRspInfo(pRspInfo);
	//}
	//����nRequestID

	//
	//if(bIsLast){
	//����
	//}
	
}; //δд
void OnRspQrySettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};//δд

void CustomTradeSpi::OnRtnOrder(CThostFtdcOrderField *pOrder) {
	char str[10];
	sprintf_s(str, "%d", pOrder->OrderSubmitStatus);
	int orderState = atoi(str) - 48;	//����״̬0=�Ѿ��ύ��3=�Ѿ�����

	std::cout << "=====�յ�����Ӧ��=====" << std::endl;

	if ((pOrder->FrontID == trade_front_id) &&
		(pOrder->SessionID == session_id) &&
		(strcmp(pOrder->OrderRef, order_ref) == 0)) // if my order?
	{
		if ((pOrder->OrderStatus != THOST_FTDC_OST_PartTradedNotQueueing) &&
			(pOrder->OrderStatus != THOST_FTDC_OST_Canceled) &&
			(pOrder->OrderStatus != THOST_FTDC_OST_AllTraded)) // if trading order?
		{
			std::cout << "--->>> �ȴ��ɽ��У�" << std::endl;
			//reqOrderAction(pOrder); // ������Գ���
			//reqUserLogout(); // �ǳ�����
		}
		else if (pOrder->OrderStatus == THOST_FTDC_OST_Canceled)
			std::cout << "--->>> �����ɹ���" << std::endl;
	}
}


///�ɽ�֪ͨ jc
void CustomTradeSpi::OnRtnTrade(CThostFtdcTradeField *pTrade)
{
	QueryPerformanceCounter(&litmp);
	QPart2 = litmp.QuadPart;//�����ֵֹ 
	dfMinus = (double)(QPart2 - QPart1);
	dfTim = dfMinus / dfFreq;// ��ö�Ӧ��ʱ��ֵ����λΪ��  
	std::cout << "���׳���ʱ�䣺"<< dfTim << std::endl;
	std::cout << "=====�����ɹ��ɽ�=====" << std::endl;
	std::cout << "�ɽ�ʱ�䣺 " << pTrade->TradeTime << std::endl;
	std::cout << "��Լ���룺 " << pTrade->InstrumentID << std::endl;
	std::cout << "�ɽ��۸� " << pTrade->Price << std::endl;
	std::cout << "�ɽ����� " << pTrade->Volume << std::endl;
	std::cout << "��ƽ�ַ��� " << pTrade->Direction << std::endl;
	info_record->record_trade_info(pTrade);
}

///����¼�����ر� 
void CustomTradeSpi::OnErrRtnOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo) {
	if (pInputOrder) {
		std::cout << pInputOrder->InstrumentID << std::endl;
	}
	isErrorRspInfo(pRspInfo);
};

///������������ر� 
void CustomTradeSpi::OnErrRtnOrderAction(CThostFtdcOrderActionField *pOrderAction, CThostFtdcRspInfoField *pRspInfo) {
	if(pOrderAction)
		std::cout << pOrderAction->OrderSysID << std::endl;
	isErrorRspInfo(pRspInfo);
};

///��Լ����״̬֪ͨ jc 
void CustomTradeSpi::OnRtnInstrumentStatus(CThostFtdcInstrumentStatusField *pInstrumentStatus) { 
	if (pInstrumentStatus) {
	//std::cout << "ExchangeID: " << pInstrumentStatus->ExchangeID << std::endl;///����������
	//std::cout << "ExchangeInstID: " << pInstrumentStatus->ExchangeInstID << std::endl;///��Լ�ڽ������Ĵ���
	//std::cout << pInstrumentStatus->SettlementGroupID << std::endl;///���������
	std::cout << "InstrumentID: " << pInstrumentStatus->InstrumentID << std::endl;///��Լ����
	std::cout << "Status: " << pInstrumentStatus->InstrumentStatus << std::endl;	///��Լ����״̬
	//std::cout << pInstrumentStatus->TradingSegmentSN << std::endl;///���׽׶α��
	///�Զ��л�
	//#define THOST_FTDC_IER_Automatic '1'
	///�ֶ��л�
	//#define THOST_FTDC_IER_Manual '2'
	///�۶�
	//#define THOST_FTDC_IER_Fuse '3'

	/*���ﱻע����*/
	//std::cout << "The Time into The Status: " << pInstrumentStatus->EnterTime << std::endl;///���뱾״̬ʱ��
	//std::cout << "Reason: " << pInstrumentStatus->EnterReason << std::endl;///���뱾״̬ԭ��

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

