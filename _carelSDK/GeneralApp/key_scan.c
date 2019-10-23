/*=======================================================================*\
* ------------------------------------------------------------------------|
* 文件		：_carelSDK/GeneralApp/keyscan.c
* 描述		：
* 创建日期		：2018/11/15
* 设计者		：Carel.Li
* 修改日期		：2019/1
* 修改内容		：架构重写
* 版本		：v1.1
*-------------------------------------------------------------------------|
\*=======================================================================*/
#include "../common.h"
#include <stdlib.h>

// KEY结构链表
LIST(scan_keys);

EmberEventControl gAppKeyScan_EventControl;


/**************************************************************************|
 * 函数：	gKey_Add
 * 功能：	添加KEY元素
 * 周期：	-
 * 参数：	num-指定编号
 * 		func_get-状态获取接口
 * 		pressed_callback-按下回调接口
 * 		released_callback-释放回调接口
 * 		pp_callback-按压态周期回调接口
 * 返回：	-
 ***/
void gKey_Add(uint8_t num, void *func_get, void *pressed_callback, \
		      void *released_callback, void *pp_callback){
	struct KEY_SCAN_t *itermKey = malloc(sizeof(struct KEY_SCAN_t));
	itermKey->num = num;
	itermKey->pcnt = 0;
	itermKey->state = 0;
	itermKey->seq = 0;
	itermKey->comcnt = 0;
	itermKey->func_get = (func_get_f)func_get;
	itermKey->pressed_callback = (pressed_callback_f)pressed_callback;
	itermKey->released_callback = (released_callback_f)released_callback;
	itermKey->pp_callback = (pp_callback_f)pp_callback;

	list_add(scan_keys, itermKey);
}

/**************************************************************************|
 * 函数：	getKeyItermPtr
 * 功能：	-
 * 周期：	-
 * 参数：	-
 * 返回：	-
 ***/
struct KEY_SCAN_t *getKeyItermPtr(uint8_t keyNum){
	struct KEY_SCAN_t *itermPtr;
	for(itermPtr = *scan_keys;;itermPtr=itermPtr->next){
		if(itermPtr->num == keyNum){
			return itermPtr;
		}

		if(itermPtr->next == NULL){
			break;
		}
	}
	return NULL;
}


/**************************************************************************|
 * 函数：	gAppKeyScan_EventFunction
 * 功能：	KEY扫描处理任务
 * 周期：	50ms
 * 参数：	-
 * 返回：	-
 ***/
//extern volatile uint8_t _OTAStatus;
void gAppKeyScan_EventFunction(void){
	struct KEY_SCAN_t *keyItem;
	emberEventControlSetInactive(gAppKeyScan_EventControl);

//	// OTA屏蔽
//	if(_OTAStatus){
//		emberEventControlSetDelayMS(gAppKeyScan_EventControl,PERIOD_KEY_SCAN);
//		return;
//	}

	if(*scan_keys != NULL){
			for(keyItem = *scan_keys;;keyItem = keyItem->next){
				switch(keyItem->seq){
				// 释放状态
				case 0:
					keyItem->state = 0;
					// 等待按下
					if(keyItem->func_get()){
						if(++(keyItem->comcnt) >= DELAY_KEY_AS){
							keyItem->comcnt = 0;
							keyItem->pressed_callback();
							keyItem->seq ++;
							break;
						}
					}
					else{
						keyItem->comcnt = 0;
					}
					break;
				// 按下状态
				case 1:
					keyItem->state = 1;
					if(++(keyItem->pcnt) >= 100000)
						keyItem->pcnt = 100000;
					// 等待释放
					if(!keyItem->func_get()){
						if(++(keyItem->comcnt) >= DELAY_KEY_AS){
							keyItem->comcnt = 0;
							keyItem->released_callback(keyItem->pcnt * PERIOD_KEY_SCAN);
							keyItem->pcnt = 0;
							keyItem->seq --;
							break;
						}
					}
					else{
						keyItem->comcnt = 0;
					}
					// 周期回调
					keyItem->pp_callback(keyItem->pcnt * PERIOD_KEY_SCAN);
					break;
				default:
					keyItem->seq = 0;
					keyItem->pcnt = 0;
					keyItem->comcnt = 0;
					break;
				}


				if(keyItem->next == NULL){
					break;
				}
			}
	}

	emberEventControlSetDelayMS(gAppKeyScan_EventControl,PERIOD_KEY_SCAN);
}


/**************************************************************************|
 * 函数：	startApp_Keyscan
 * 功能：	初始化Key链表，启动Key任务
 * 周期：	-
 * 参数：	delay-启动延时
 * 返回：	-
 ***/
void startApp_Keyscan(uint32_t delay){
	list_init(scan_keys);
	if(delay == 0){
		emberEventControlSetActive(gAppKeyScan_EventControl);
	}
	else{
		emberEventControlSetDelayMS(gAppKeyScan_EventControl,delay);
	}
}

void stopApp_Keyscan(void){
	emberEventControlSetInactive(gAppKeyScan_EventControl);
}


#if 0
#define KEY_0				BUTTON0
#define KEY_1				BUTTON1
#define PRESSED				BUTTON_PRESSED
#define RELEASED			BUTTON_RELEASED
#define TIM_KEY0PRESSED1	((uint32_t)(20 * 3))		// 3s
#define TIM_KEY0PRESSED2	((uint32_t)(20 * 8))		// 8s


// key0按下
extern void key0Pressed_Callback(void);
// key0一级长按
extern void key0LongPressed1_Callback(void);
// key0二级长按
extern void key0LongPressed2_Callback(void);
// key1按下
extern void key1Pressed_Callback(void);
// key1持续按下
extern void key1ContinuePressed_Callback(void);
// key1松开
extern void key1Released_Callback(void);

EmberEventControl gAppKeyScan_EventControl;
void gAppKeyScan_EventFunction(void){
	static uint8_t SEQ_keyscan0 = 0;
	static uint8_t SEQ_keyscan1 = 0;
	static uint32_t CNT_key0Pressed = 0;
	emberEventControlSetInactive(gAppKeyScan_EventControl);

	// 读取按键状态
	uint8_t st_key0 = halButtonPinState(KEY_0);
	uint8_t st_key1 = halButtonPinState(KEY_1);


	/* ---------------key0---------------- */
	switch(SEQ_keyscan0){
	// 判断状态
	case 0:
		if(st_key0 == RELEASED)
			SEQ_keyscan0 = 1;
		else if(st_key0 == PRESSED)
			SEQ_keyscan0 = 2;

		break;

	// 松开状态
	case 1:
		// 发生：按键按下
		if(st_key0 == PRESSED){
			SEQ_keyscan0 = 2;
			CNT_key0Pressed = 0;
			key0Pressed_Callback();
			break;
		}
		break;

	// 按下状态
	case 2:

		// 发生：到达一级长按时间
		if(++CNT_key0Pressed == TIM_KEY0PRESSED1){
			key0LongPressed1_Callback();
		}

		// 发生：到达二级长按时间
		if(CNT_key0Pressed == TIM_KEY0PRESSED2){
			key0LongPressed2_Callback();
		}

		// 发生：松开按键
		if(st_key0 == RELEASED){
			CNT_key0Pressed = 0;
			SEQ_keyscan0 = 1;
			break;
		}
		break;

	default:
		SEQ_keyscan0 = 0;
		CNT_key0Pressed = 0;
		break;
	}


	/* ---------------key1---------------- */
	switch(SEQ_keyscan1){
	// 判断状态
	case 0:
		if(st_key1 == RELEASED)
			SEQ_keyscan1 = 1;
		else if(st_key1 == PRESSED)
			SEQ_keyscan1 = 2;

		break;

	// 松开状态
	case 1:
		// 发生：按键按下
		if(st_key1 == PRESSED){
			SEQ_keyscan1 = 2;
			key1Pressed_Callback();
			break;
		}
		break;

	// 按下状态
	case 2://DEBUG("~~~~~~~~~~~~~~~~~~~~key1 press .....");
		// 发生：松开按键
		if(st_key1 == RELEASED){
			SEQ_keyscan1 = 1;
			key1Released_Callback();
			break;
		}

		key1ContinuePressed_Callback();

		break;

	default:
		SEQ_keyscan1 = 0;
		break;
	}


	emberEventControlSetDelayMS(gAppKeyScan_EventControl,PERIOD_KEY_SCAN);
}

#endif


