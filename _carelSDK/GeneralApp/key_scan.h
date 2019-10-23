
#ifndef CARELSDK_GENERALAPP_KEY_SCAN_H_
#define CARELSDK_GENERALAPP_KEY_SCAN_H_

//////////////////////////////////////////////////////////////////////////
// ↓↓↓ 配置 ↓↓↓
//////////////////////////////////////////////////////////////////////////
#define PERIOD_KEY_SCAN				((uint32_t)(50))	// Key任务周期
#define DELAY_KEY_AS				1					// 防抖级别(>0)
//////////////////////////////////////////////////////////////////////////
// ↑↑↑  配置 ↑↑↑
//////////////////////////////////////////////////////////////////////////



// Key 方法接口定义
typedef uint8_t (*func_get_f)(void);
typedef void (*pressed_callback_f)(void);
typedef void (*released_callback_f)(uint32_t);
typedef void (*pp_callback_f)(uint32_t);

// Key 结构体
struct KEY_SCAN_t{
	struct KEY_SCAN_t *next;
	uint8_t num;							// 编号
	uint8_t seq;							// 状态机序列
	uint8_t state;							// 状态 [1-按下   0-释放]
	uint16_t comcnt;						// 公共计数
	uint32_t pcnt;							// 按下计数
	func_get_f func_get; 					// 状态获取函数
	pressed_callback_f pressed_callback;	// 按下回调
	released_callback_f released_callback;	// 松开回调
	pp_callback_f pp_callback;				// 按压态周期回调
};

// API
void gKey_Add(uint8_t num, void *func_get, void *pressed_callback, \
		      void *released_callback, void *pp_callback);
void startApp_Keyscan(uint32_t delay);
void stopApp_Keyscan(void);
struct KEY_SCAN_t *getKeyItermPtr(uint8_t keyNum);

#endif /* CARELSDK_GENERALAPP_KEY_SCAN_H_ */
