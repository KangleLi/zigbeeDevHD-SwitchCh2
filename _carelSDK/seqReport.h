#ifndef CARELSDK_SEQREPORT_H_
#define CARELSDK_SEQREPORT_H_




#define _PERIOD_SEQ_REPORT				30					// 序列上报控制周期


typedef struct _SEQ_REPORT{
	struct _SEQ_REPORT *next;

	uint16_t clusterId;
	uint16_t attributeId;
	bool ifMfg;
//	uint32_t delayMs;
//	uint32_t curCnt;
//	bool ifCyc;
	uint8_t endpoint;

} _seq_report_t;


void _SEQ_Report_Init(void);

void _SEQ_Report_Add(uint8_t endpoint, uint16_t clusterId, uint16_t attributeId, bool ifMfg);

#endif /* CARELSDK_SEQREPORT_H_ */
