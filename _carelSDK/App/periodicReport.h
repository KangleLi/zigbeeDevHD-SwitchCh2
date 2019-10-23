
#ifndef CARELSDK_APP_PERIODICREPORT_H_
#define CARELSDK_APP_PERIODICREPORT_H_

// config
#define _PERPRIOD_NO_ONOFF					1
#define _PERPRIOD_NO_INDICATOR				2
//


#define _SHORTTEMP_PERIOD				50			// 5s
#define _SHORTTEMP_COUNT				3			// 3´Î


typedef struct _PERREPORT_T{
	struct _PERREPORT_T *next;
	uint8_t no;
	uint16_t clusterId;
	uint16_t attributeId;
	bool isMfg;
	uint32_t period;
	uint32_t cnt;
	bool oneEp;
} _perReport_t;

typedef enum {
	_PerReport_Cmd_Disable = 1,
	_PerReport_Cmd_Enable,
	_PerReport_Cmd_ShortTemp,
	_PerReport_Cmd_RefleshPeriod,
} _perReport_Cmd_t;



void _perReportCmd(uint8_t no, _perReport_Cmd_t cmd);
// periodµ¥Î»0.1s
void _perReportItem_Add(uint8_t no, uint16_t cId, uint16_t aId, bool isMfg, uint32_t period, bool oneEp);

void _perReportItem_Del(uint8_t no);

void _periodicReportInit(void);

#endif /* CARELSDK_APP_PERIODICREPORT_H_ */
