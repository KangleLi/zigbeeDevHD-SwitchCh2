
#ifndef CARELSDK_GENERALAPP_LED_BLINK_H_
#define CARELSDK_GENERALAPP_LED_BLINK_H_


//////////////////////////////////////////////////////////////////////////
// ↓↓↓ 配置 ↓↓↓
//////////////////////////////////////////////////////////////////////////
#define PERIOD_LED_BLINK			((uint32_t)(25))	// Led_Blink任务周期
//////////////////////////////////////////////////////////////////////////
// ↑↑↑  配置 ↑↑↑
//////////////////////////////////////////////////////////////////////////




// Led操作函数
typedef void (*gLedFunc_ctl)(uint8_t newState);

// Led 结构体
struct LED_BLINK_t{
	struct LED_BLINK_t *next;
	uint8_t num;						// 编号
	uint8_t busy;
	uint32_t busyPeriod;
	uint32_t busyCnt;
	uint8_t state;						// 状态
	uint32_t period;					// 周期
	uint32_t ccnt;						// 当前计数
	uint32_t delay;						// 动态时间
	uint8_t stillstate;				// 静止状态
	gLedFunc_ctl func_ctl;				// 操作函数
};


// Led_Blink任务句柄
extern EmberEventControl gApp_EventControl;

// API
void startApp_LedBlink(uint32_t delay);
void gLed_Add(uint8_t num, gLedFunc_ctl fc, uint8_t stillState);
void gLed_Active(uint8_t num, uint32_t period, uint32_t delay);
void gLed_Inactive(uint8_t num);
void gLed_SetStillState(uint8_t num, uint8_t newState);

void gLed_BusySet(uint8_t num, uint8_t busy, uint32_t period, uint8_t startupState);



#endif /* CARELSDK_GENERALAPP_LED_BLINK_H_ */


