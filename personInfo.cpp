#include "personInfo.h"

TThostFtdcBrokerIDType BrokerID = "9999";                          // ģ�⾭���̴���
TThostFtdcInvestorIDType InvestorID = "112502";                    // Ͷ�����˻���
TThostFtdcPasswordType InvestorPassword = "19960122";              // Ͷ��������

// �������
CThostFtdcMdApi *MarketApi = nullptr;                              // ����ָ��
CustomMdSpi *MarketSpi = new CustomMdSpi;
char MdFrontAddr[] = "tcp://180.168.146.187:10011";                // ģ������ǰ�õ�ַ


// ���ײ���
CThostFtdcTraderApi *TradeApi = nullptr;                           // ����ָ��
char TradeFrontAddr[] = "tcp://180.168.146.187:10001";             // ģ�⽻��ǰ�õ�ַ
TThostFtdcDirectionType gTradeDirection = THOST_FTDC_D_Sell;       // ��������
TThostFtdcPriceType gLimitPrice = 22735;                           // ���׼۸�

CommandQueue* CQueue = new CommandQueue;  
InfoRecord* info_record = new InfoRecord;
RecordManagement* RManagement = new RecordManagement;
CustomTradeSpi *TradeSpi = new CustomTradeSpi;
