#include "personInfo.h"

TThostFtdcBrokerIDType BrokerID = "9999";                          // ģ�⾭���̴���
TThostFtdcInvestorIDType InvestorID = "112502";                    // Ͷ�����˻���
TThostFtdcPasswordType InvestorPassword = "19960122";              // Ͷ��������

																	// �������
CThostFtdcMdApi *MarketApi = nullptr;                              // ����ָ��
char MdFrontAddr[] = "tcp://180.168.146.187:10010";                // ģ������ǰ�õ�ַ
char Inid[2][31] = { "au1806", "ni1807" };
char *InstrumentID[] = { Inid[0], Inid[1]};     // �����Լ�����б��С��ϡ���֣��������ѡһ��
int instrumentNum = 4;                                             // �����Լ��������

																	// ���ײ���
CThostFtdcTraderApi *TradeApi = nullptr;                           // ����ָ��
char TradeFrontAddr[] = "tcp://180.168.146.187:10001";             // ģ�⽻��ǰ�õ�ַ
TThostFtdcInstrumentIDType TradeInstrumentID = "au1806";           // �����׵ĺ�Լ����
TThostFtdcDirectionType gTradeDirection = THOST_FTDC_D_Sell;       // ��������
TThostFtdcPriceType gLimitPrice = 22735;                           // ���׼۸�

CommandQueue* CQueue = new CommandQueue;  
InfoRecord* info_record = new InfoRecord;
RecordManagement* RManagement = new RecordManagement;
CustomTradeSpi *TradeSpi = new CustomTradeSpi;

