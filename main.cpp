#include "personInfo.h"
#include "TradeStrategy/TradeStrategy.h"
#pragma comment (lib, "CTPapi/thostmduserapi.lib")
#pragma comment (lib, "CTPapi/thosttraderapi.lib")

int main() {
	// 命令队列初始化
	std::cout << ">>> initializing command queue......" << std::endl;
	pthread_t pth;
	pthread_create(&pth, NULL, run_instr, (void*)CQueue);

	// 打印队列初始化
	std::cout << ">>> initializing print thread......" << std::endl;
	pthread_t pin;
	pthread_create(&pin, NULL, write_record, (void*)info_record);

	// 初始化行情线程
	std::cout << ">>> initializing Market Api......" << std::endl;
	MarketApi = CThostFtdcMdApi::CreateFtdcMdApi();     // 创建行情实例
	CThostFtdcMdSpi *MarketSpi = new CustomMdSpi;       // 创建行情回调实例
	MarketApi->RegisterSpi(MarketSpi);                  // 注册事件类
	MarketApi->RegisterFront(MdFrontAddr);              // 设置行情前置地址
	MarketApi->Init();                                  // 连接运行
	//MarketApi->Join();
	// 初始化交易线程
	std::cout << ">>> initializing Trade Api......" << std::endl;
	TradeApi = CThostFtdcTraderApi::CreateFtdcTraderApi();        // 创建交易实例
	//CustomTradeSpi *TradeSpi = new CustomTradeSpi;                // 创建交易回调实例
	TradeApi->RegisterSpi(TradeSpi);                              // 注册事件类
	TradeApi->SubscribePublicTopic(THOST_TERT_RESTART);           // 订阅公共流
	TradeApi->SubscribePrivateTopic(THOST_TERT_RESTART);          // 订阅私有流
	TradeApi->RegisterFront(TradeFrontAddr);                      // 设置交易前置地址
	TradeApi->Init();
	
	TradeStrategy* TS = new TradeStrategy;
	//TS->trade();

	
	// 连接运行
	TradeApi->Join();
	
	pthread_join(pth, NULL);
	pthread_join(pin, NULL);
	
	// 等到线程退出
	delete MarketSpi;
	MarketApi->Release();
	delete TradeSpi;
	TradeApi->Release();
	getchar();

	return 0;

}