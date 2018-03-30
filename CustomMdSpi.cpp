#include <unordered_map>
#include "CustomMdSpi.h"
#include <windows.h>
#include "personInfo.h"
LARGE_INTEGER Mlitmp;
LONGLONG MQPart1, MQPart2;
double MdfMinus, MdfFreq, MdfTim;

CustomMdSpi::CustomMdSpi() {
	QueryPerformanceFrequency(&Mlitmp);
	MdfFreq = (double)Mlitmp.QuadPart;// 获得计数器的时钟频率 
	MQPart1 = Mlitmp.QuadPart;        // 获得初始值
	std::cout << ">>> create CustomMdSpi" << std::endl;
}

CustomMdSpi:: ~CustomMdSpi() {
	std::cout << ">>> delete CustomMdSpi" << std::endl;
}
void CustomMdSpi::OnFrontConnected() {
	std::cout << "======== Market Connection Established ========" << std::endl;
	CThostFtdcReqUserLoginField loginReq;
	memset(&loginReq, 0, sizeof(loginReq));
	strcpy_s(loginReq.BrokerID, BrokerID);
	strcpy_s(loginReq.UserID, InvestorID);
	strcpy_s(loginReq.Password, InvestorPassword);
	static int requestID = 0;
	int rt = MarketApi->ReqUserLogin(&loginReq, ++requestID);

	if (!rt)
		std::cout << ">>> login request success" << std::endl;
	else
		std::cout << ">>> login request failed" << std::endl;
};
void CustomMdSpi::OnFrontDisconnected(int nReason) {
	std::cerr << "===== Front Disconnected =====" << std::endl;
	std::cerr << ">>> Error Code： " << nReason << std::endl;
};

void CustomMdSpi::OnHeartBeatWarning(int nTimeLapse) {
	std::cerr << "===== HeartBeat Warning =====" << std::endl;
	std::cerr << ">>> The Time Last Connected： " << nTimeLapse << std::endl;
};

void CustomMdSpi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	bool bResult = pRspInfo && (pRspInfo->ErrorID != 0);
	if (!bResult)
	{
		std::cout << ">>> Market Account Login Successfully" << std::endl;
		std::cout << ">>> Date:" << pRspUserLogin->TradingDay << std::endl;
		std::cout << ">>> Time:" << pRspUserLogin->LoginTime << std::endl;
		std::cout << ">>> Broker:" << pRspUserLogin->BrokerID << std::endl;
		std::cout << ">>> User:" << pRspUserLogin->UserID << std::endl;

		RManagement->MarketIsConnected = 1;
	}
	else
		std::cerr << ">>> Return Error. ErrorID=" << pRspInfo->ErrorID << ", ErrorMsg" << pRspInfo->ErrorMsg << std::endl;
};

void CustomMdSpi::subscribe_market_data(char** InstrumentID,int instrumentNum) {

	std::cout << InstrumentID[0] << std::endl;
	std::cout << InstrumentID[1] << std::endl;
	int rt = MarketApi->SubscribeMarketData(InstrumentID, instrumentNum);
	if (!rt)
		std::cout << ">>> Subscribe request sent" << std::endl;
	else
		std::cout << ">>> Subscribe request failed" << std::endl;

}

void CustomMdSpi::OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	bool bResult = pRspInfo && (pRspInfo->ErrorID != 0);
	if (!bResult)
	{
		std::cout << "===== Logout Successfully=====" << std::endl;
		std::cout << ">>> BrokerID: " << pUserLogout->BrokerID << std::endl;
		std::cout << ">>> UserID: " << pUserLogout->UserID << std::endl;
	}
	else
		std::cerr << "Return Error--->>> ErrorID=" << pRspInfo->ErrorID << ", ErrorMsg=" << pRspInfo->ErrorMsg << std::endl;
};
void CustomMdSpi::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	bool bResult = pRspInfo && (pRspInfo->ErrorID != 0);
	if (bResult)
		std::cerr << "Return Error--->>> ErrorID=" << pRspInfo->ErrorID << ", ErrorMsg=" << pRspInfo->ErrorMsg << std::endl;
};
void CustomMdSpi::OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	bool bResult = pRspInfo && (pRspInfo->ErrorID != 0);
	if (!bResult)
	{
		std::cout << "===== Subscribe Market Data Success =====" << std::endl;
		std::cout << "InstrumentID： " << pSpecificInstrument->InstrumentID << std::endl;
		// save market data to files  
		char filePath[100] = { '\0' };
		sprintf_s(filePath, "%s_market_data.csv", pSpecificInstrument->InstrumentID);
		std::ofstream outFile;
		outFile.open(filePath, std::ios::out);
		outFile << "InstrumentID" << ","
			<< "updateTime" << ","
			<< "lastPrice" << ","
			<< "lastVolume" << ","
			<< "buy1Price" << ","
			<< "buy1Volume" << ","
			<< "sell1Price" << ","
			<< "sell1Volume" << ","
			<< "holdVolume" << ","
			<< "turnover"
			<< std::endl;
		outFile.close();
	}
	else
		std::cerr << "Return Error, ErrorID=" << pRspInfo->ErrorID << ", ErrorMsg=" << pRspInfo->ErrorMsg << std::endl;
};
void CustomMdSpi::OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};
void CustomMdSpi::OnRspSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};
void CustomMdSpi::OnRspUnSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};
void CustomMdSpi::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData) {

	// 测试时间起始位置

	// QueryPerformanceCounter(&litmp);
	// QPart1 = litmp.QuadPart;// 获得初始值 

	QueryPerformanceCounter(&Mlitmp);
	MQPart2 = Mlitmp.QuadPart;//获得中止值 
	MdfMinus = (double)(MQPart2 - MQPart1);
	MdfTim = MdfMinus / MdfFreq;// 获得对应的时间值，单位为秒  
	std::cout << "查询间隔时间：" << MdfTim << std::endl;
	TimeFile << MdfTim << std::endl;
	/*
	// detailed market data  
	std::cout << "=====Get Depth Market Data=====" << std::endl;
	std::cout << "Date： " << pDepthMarketData->TradingDay << std::endl;
	std::cout << "ExchangeID: " << pDepthMarketData->ExchangeID << std::endl;
	std::cout << "InstrumentID: " << pDepthMarketData->InstrumentID << std::endl;
	std::cout << "ExchangeInstID: " << pDepthMarketData->ExchangeInstID << std::endl;
	std::cout << "LastPrice: " << pDepthMarketData->LastPrice << std::endl;
	std::cout << "Volume: " << pDepthMarketData->Volume << std::endl;
	
	
	// save to local database  
	char filePath[100] = { '\0' };
	sprintf_s(filePath, "%s_market_data.csv", pDepthMarketData->InstrumentID);
	std::ofstream outFile;
	outFile.open(filePath, std::ios::app);
	outFile << pDepthMarketData->InstrumentID << ","
		<< pDepthMarketData->UpdateTime << "." << pDepthMarketData->UpdateMillisec << ","
		<< pDepthMarketData->LastPrice << ","
		<< pDepthMarketData->Volume << ","
		<< pDepthMarketData->BidPrice1 << ","
		<< pDepthMarketData->BidVolume1 << ","
		<< pDepthMarketData->AskPrice1 << ","
		<< pDepthMarketData->AskVolume1 << ","
		<< pDepthMarketData->OpenInterest << ","
		<< pDepthMarketData->Turnover << std::endl;
	outFile.close();
	*/

	RManagement->time_to_update_instr_data(pDepthMarketData); // 更新数据
	QueryPerformanceCounter(&Mlitmp);
	MQPart1 = Mlitmp.QuadPart;// 获得初始值
};
void CustomMdSpi::OnRtnForQuoteRsp(CThostFtdcForQuoteRspField *pForQuoteRsp) {
	std::cout << "===== Quotation Responce =====" << std::endl;
	std::cout << "TradingDay: " << pForQuoteRsp->TradingDay << std::endl;
	std::cout << "ExchangeID: " << pForQuoteRsp->ExchangeID << std::endl;
	std::cout << "InstrumentID: " << pForQuoteRsp->InstrumentID << std::endl;
	std::cout << "ForQuoteSysID: " << pForQuoteRsp->ForQuoteSysID << std::endl;
};
