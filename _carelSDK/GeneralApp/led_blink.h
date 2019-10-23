
#ifndef CARELSDK_GENERALAPP_LED_BLINK_H_
#define CARELSDK_GENERALAPP_LED_BLINK_H_


//////////////////////////////////////////////////////////////////////////
// ������ ���� ������
//////////////////////////////////////////////////////////////////////////
#define PERIOD_LED_BLINK			((uint32_t)(25))	// Led_Blink��������
//////////////////////////////////////////////////////////////////////////
// ������  ���� ������
//////////////////////////////////////////////////////////////////////////




// Led��������
typedef void (*gLedFunc_ctl)(uint8_t newState);

// Led �ṹ��
struct LED_BLINK_t{
	struct LED_BLINK_t *next;
	uint8_t num;						// ���
	uint8_t busy;
	uint32_t busyPeriod;
	uint32_t busyCnt;
	uint8_t state;						// ״̬
	uint32_t period;					// ����
	uint32_t ccnt;						// ��ǰ����
	uint32_t delay;						// ��̬ʱ��
	uint8_t stillstate;				// ��ֹ״̬
	gLedFunc_ctl func_ctl;				// ��������
};


// Led_Blink������
extern EmberEventControl gApp_EventControl;

// API
void startApp_LedBlink(uint32_t delay);
void gLed_Add(uint8_t num, gLedFunc_ctl fc, uint8_t stillState);
void gLed_Active(uint8_t num, uint32_t period, uint32_t delay);
void gLed_Inactive(uint8_t num);
void gLed_SetStillState(uint8_t num, uint8_t newState);

void gLed_BusySet(uint8_t num, uint8_t busy, uint32_t period, uint8_t startupState);



#endif /* CARELSDK_GENERALAPP_LED_BLINK_H_ */


