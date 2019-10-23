
#ifndef CARELSDK_APP_ONOFF_H_
#define CARELSDK_APP_ONOFF_H_

#define _TOGGLE_INTERVAL				200



typedef struct _TOGGLE_T{
	struct _TOGGLE_T *next;
	uint8_t ch;
	uint8_t cmd;
} _toggle_t;



void _ctlToggle_Init(void);
void _ctlIndicatorSyncAll(void);


#endif /* CARELSDK_APP_ONOFF_H_ */
