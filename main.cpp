#include "personInfo.h"
#include "TradeStrategy/TradeStrategy.h"
#pragma comment (lib, "CTPapi/thostmduserapi.lib")
#pragma comment (lib, "CTPapi/thosttraderapi.lib")


int main() {
	// ������г�ʼ��
	std::cout << ">>> initializing command queue......" << std::endl;
	pthread_t pth;
	pthread_create(&pth, NULL, run_instr, (void*)CQueue);

	// ��ӡ���г�ʼ��
	std::cout << ">>> initializing print thread......" << std::endl;
	pthread_t pin;
	pthread_create(&pin, NULL, write_record, (void*)info_record);

	// ��ʼ�������߳�
	std::cout << ">>> initializing Market Api......" << std::endl;
	MarketApi = CThostFtdcMdApi::CreateFtdcMdApi();     // ��������ʵ��
	MarketApi->RegisterSpi(MarketSpi);                  // ע���¼���
	MarketApi->RegisterFront(MdFrontAddr);              // ��������ǰ�õ�ַ
	MarketApi->Init();                                  // ��������
	
	// ��ʼ�������߳�
	std::cout << ">>> initializing Trade Api......" << std::endl;
	TradeApi = CThostFtdcTraderApi::CreateFtdcTraderApi();        // ��������ʵ��
	TradeApi->RegisterSpi(TradeSpi);                              // ע���¼���
	TradeApi->SubscribePublicTopic(THOST_TERT_RESTART);           // ���Ĺ�����
	TradeApi->SubscribePrivateTopic(THOST_TERT_RESTART);          // ����˽����
	TradeApi->RegisterFront(TradeFrontAddr);                      // ���ý���ǰ�õ�ַ
	TradeApi->Init();
	
	//�������Խ���
	TradeStrategy* TS = new TradeStrategy;
	TS->trade();

	// ��������
	MarketApi->Join();
	TradeApi->Join();
	pthread_join(pth, NULL);
	pthread_join(pin, NULL);
	
	// �ȵ��߳��˳�
	delete MarketSpi;
	MarketApi->Release();
	delete TradeSpi;
	TradeApi->Release();
	getchar();
	return 0;
}