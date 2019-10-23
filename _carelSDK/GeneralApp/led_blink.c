/*=======================================================================*\
* ------------------------------------------------------------------------|
* 文件		：_carelSDK/Z3_Extend/mainCallback.c
* 描述		：通用LED指示灯高级驱动管理，支持等幅频闪、定时频闪、静态控制
* 创建日期		：2018/11/15
* 设计者		：Carel.Li
* 修改日期		：2019/1
* 修改内容		：架构重写
* 版本		：v1.1
*-------------------------------------------------------------------------|
\*=======================================================================*/
#include "../common.h"
#include<stdlib.h>


// LED结构链表
LIST(blink_leds);

EmberEventControl gAppLedBlink_EventControl;


/**************************************************************************|
 * 函数：	startApp_LedBlink
 * 功能：	初始化LED链表、延时启动LED任务
 * 周期：	-
 * 参数：	delay-启动延时
 * 返回：	-
 ***/
void startApp_LedBlink(uint32_t delay){
	list_init(blink_leds);
	if(delay == 0)
		emberEventControlSetActive(gAppLedBlink_EventControl);
	else
		emberEventControlSetDelayMS(gAppLedBlink_EventControl,delay);
}


/**************************************************************************|
 * 函数：	gLed_Add
 * 功能：	添加LED元素
 * 周期：	-
 * 参数：	num-指定编号、fc-指定操作函数、stillState-初始状态
 * 返回：	-
 ***/
void gLed_Add(uint8_t num, gLedFunc_ctl fc, uint8_t stillState){
	struct LED_BLINK_t *itermLed = malloc(sizeof(struct LED_BLINK_t));
	itermLed->ccnt = 0;
	itermLed->busy = 0;
	itermLed->busyPeriod = 0;
	itermLed->busyCnt = 0;
	itermLed->delay = 0;
	itermLed->func_ctl = fc;
	itermLed->num = num;
	itermLed->period = 0;
	itermLed->state = stillState;
	itermLed->stillstate = stillState;
	list_add(blink_leds,itermLed);
}


/**************************************************************************|
 * 函数：	gLed_Active
 * 功能：	LED激活
 * 周期：	-
 * 参数：	num-指定编号、period-闪烁周期(半周期)、delay-激活时间
 * 返回：	-
 ***/
void gLed_Active(uint8_t num, uint32_t period, uint32_t delay){
	struct LED_BLINK_t *ledItem;
	if(*blink_leds == NULL){
		return;
	}
	for(ledItem = *blink_leds;;ledItem = ledItem->next){
		if(ledItem->num == num){
			ledItem->ccnt = period;
			ledItem->delay = delay;
			ledItem->period = period;
			ledItem->state = ledItem->stillstate;
			break;
		}
		if(ledItem->next == NULL)
			break;
	}
}


/**************************************************************************|
 * 函数：	gLed_Inactive
 * 功能：	LED取消激活
 * 周期：	-
 * 参数：	num-指定编号
 * 返回：	-
 ***/
void gLed_Inactive(uint8_t num){
	struct LED_BLINK_t *ledItem;
	if(*blink_leds == NULL){
		return;
	}
	for(ledItem = *blink_leds;;ledItem = ledItem->next){
		if(ledItem->num == num){
			ledItem->ccnt = 0;
			ledItem->delay = 0;
			ledItem->period = 0;
			ledItem->state = ledItem->stillstate;
			break;
		}
		if(ledItem->next == NULL)
			break;
	}
}


/**************************************************************************|
 * 函数：	gLed_SetStillState
 * 功能：	LED设置静止状态
 * 周期：	-
 * 参数：	num-指定编号、newState-指定状态
 * 返回：	-
 ***/
void gLed_SetStillState(uint8_t num, uint8_t newState){
	struct LED_BLINK_t *ledItem;
	if(*blink_leds == NULL){
		return;
	}
	for(ledItem = *blink_leds;;ledItem = ledItem->next){
		if(ledItem->num == num){
			ledItem->stillstate = newState;
			break;
		}
		if(ledItem->next == NULL)
			break;
	}
}


/**************************************************************************|
 * 函数：	gLed_BusySet
 * 功能：	LED设置占用状态
 * 周期：	-
 * 参数：	num-指定编号、busy-忙使能、period-指定周期(0为静止)、startupState-启动时LED状态
 * 返回：	-
 ***/
void gLed_BusySet(uint8_t num, uint8_t busy, uint32_t period, uint8_t startupState){
	struct LED_BLINK_t *ledItem;
	if(*blink_leds == NULL){
		return;
	}
	for(ledItem = *blink_leds;;ledItem = ledItem->next){
		if(ledItem->num == num){
			if(busy > 0){
				ledItem->busy = 1;
				ledItem->busyCnt = period;
				ledItem->busyPeriod = period;
				ledItem->state = startupState?1:0;
			}
			else{
				ledItem->busy = 0;
				ledItem->busyCnt = 0;
				ledItem->busyPeriod = 0;
				ledItem->state = 0;
			}
			break;
		}
		if(ledItem->next == NULL)
			break;
	}
}


/**************************************************************************|
 * 函数：	gAppLedBlink_EventFunction
 * 功能：	LED显示处理任务
 * 周期：	25ms
 * 参数：	-
 * 返回：	-
 ***/
//extern volatile uint8_t _OTAStatus;
void gAppLedBlink_EventFunction(void){
	struct LED_BLINK_t *ledItem;
	emberEventControlSetInactive(gAppLedBlink_EventControl);

//	// OTA屏蔽
//	if(_OTAStatus){
//		emberEventControlSetDelayMS(gAppLedBlink_EventControl,PERIOD_LED_BLINK);
//		return;
//	}

	if(*blink_leds != NULL){
		for(ledItem = *blink_leds;;ledItem = ledItem->next){
			// 忙处理
			if(ledItem->busy > 0){
				if(ledItem->busyPeriod > 0){
					if(ledItem->busyCnt > 0){
						ledItem->busyCnt --;

					}
					else{
						ledItem->busyCnt = ledItem->busyPeriod - 1;
						ledItem->state = (ledItem->state)?0:1;
					}
				}
				ledItem->func_ctl(ledItem->state);
			}
			else{
				// 动态过程中
				if(ledItem->delay > 0){
					ledItem->delay --;
					// 当前周期未结束
					if(ledItem->ccnt > 0){
						ledItem->ccnt --;
					}
					// 当前周期已结束
					else{
						ledItem->ccnt = ledItem->period?(ledItem->period - 1):0;	// TODO
						if(ledItem->state)
							ledItem->state = 0;
						else
							ledItem->state = 1;
					}
					ledItem->func_ctl(ledItem->state);
				}
				// 动态时间结束
				else{
					// carel (优化闪烁结束处理)
					if(ledItem->ccnt > 0){
						ledItem->ccnt --;
					}
					else
						ledItem->func_ctl(ledItem->stillstate);
					// 20190726
				}
			}

			if(ledItem->next == NULL){
				break;
			}
		}
	}
	else{
		//emberAfCorePrintln("NULL");
	}

	emberEventControlSetDelayMS(gAppLedBlink_EventControl,PERIOD_LED_BLINK);
}

