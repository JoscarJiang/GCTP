#pragma once
//ͷ�ļ�����
#include "CustomMdSpi.h"
#include "CustomTradeSpi.h"
#include "RecordManagement/InfoRecord.h"
#include "CommandManagement/CommandQueue.h"
#include "RecordManagement.h"

#ifndef _a
#define _a
extern TThostFtdcBrokerIDType BrokerID;                           // ģ�⾭���̴���
extern TThostFtdcInvestorIDType InvestorID;                       // Ͷ�����˻���
extern TThostFtdcPasswordType InvestorPassword;                   // Ͷ��������
#endif

#ifndef _b
#define _b
// �������
extern CThostFtdcMdApi *MarketApi;                                // ����ָ��
extern char MdFrontAddr[];                                        // ģ������ǰ�õ�ַ
extern char Inid[4][7];
extern char *InstrumentID[];                                      // �����Լ�����б��С��ϡ���֣��������ѡһ��
extern int instrumentNum;                                         // �����Լ����
#endif

#ifndef _c
#define _c
// ���ײ���
extern CThostFtdcTraderApi *TradeApi;                             // ����ָ��
extern char TradeFrontAddr[];                                     // ģ�⽻��ǰ�õ�ַ
extern TThostFtdcInstrumentIDType TradeInstrumentID;              // �����׵ĺ�Լ����
extern TThostFtdcDirectionType gTradeDirection;                   // ��������
extern TThostFtdcPriceType gLimitPrice;                           // ���׼۸�
extern CustomTradeSpi *TradeSpi;
#endif

#ifndef _d
#define _d
// �������
extern CommandQueue* CQueue;                                      // �������
extern RecordManagement* RManagement;                             // ��¼����ӿ�
#endif

#ifndef _e
#define _e
// ��¼�ʹ�ӡ����
extern InfoRecord* info_record;                                   
#endif


