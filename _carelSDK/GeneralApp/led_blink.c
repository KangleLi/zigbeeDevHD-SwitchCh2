/*=======================================================================*\
* ------------------------------------------------------------------------|
* �ļ�		��_carelSDK/Z3_Extend/mainCallback.c
* ����		��ͨ��LEDָʾ�Ƹ߼���������֧�ֵȷ�Ƶ������ʱƵ������̬����
* ��������		��2018/11/15
* �����		��Carel.Li
* �޸�����		��2019/1
* �޸�����		���ܹ���д
* �汾		��v1.1
*-------------------------------------------------------------------------|
\*=======================================================================*/
#include "../common.h"
#include<stdlib.h>


// LED�ṹ����
LIST(blink_leds);

EmberEventControl gAppLedBlink_EventControl;


/**************************************************************************|
 * ������	startApp_LedBlink
 * ���ܣ�	��ʼ��LED������ʱ����LED����
 * ���ڣ�	-
 * ������	delay-������ʱ
 * ���أ�	-
 ***/
void startApp_LedBlink(uint32_t delay){
	list_init(blink_leds);
	if(delay == 0)
		emberEventControlSetActive(gAppLedBlink_EventControl);
	else
		emberEventControlSetDelayMS(gAppLedBlink_EventControl,delay);
}


/**************************************************************************|
 * ������	gLed_Add
 * ���ܣ�	���LEDԪ��
 * ���ڣ�	-
 * ������	num-ָ����š�fc-ָ������������stillState-��ʼ״̬
 * ���أ�	-
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
 * ������	gLed_Active
 * ���ܣ�	LED����
 * ���ڣ�	-
 * ������	num-ָ����š�period-��˸����(������)��delay-����ʱ��
 * ���أ�	-
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
 * ������	gLed_Inactive
 * ���ܣ�	LEDȡ������
 * ���ڣ�	-
 * ������	num-ָ�����
 * ���أ�	-
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
 * ������	gLed_SetStillState
 * ���ܣ�	LED���þ�ֹ״̬
 * ���ڣ�	-
 * ������	num-ָ����š�newState-ָ��״̬
 * ���أ�	-
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
 * ������	gLed_BusySet
 * ���ܣ�	LED����ռ��״̬
 * ���ڣ�	-
 * ������	num-ָ����š�busy-æʹ�ܡ�period-ָ������(0Ϊ��ֹ)��startupState-����ʱLED״̬
 * ���أ�	-
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
 * ������	gAppLedBlink_EventFunction
 * ���ܣ�	LED��ʾ��������
 * ���ڣ�	25ms
 * ������	-
 * ���أ�	-
 ***/
//extern volatile uint8_t _OTAStatus;
void gAppLedBlink_EventFunction(void){
	struct LED_BLINK_t *ledItem;
	emberEventControlSetInactive(gAppLedBlink_EventControl);

//	// OTA����
//	if(_OTAStatus){
//		emberEventControlSetDelayMS(gAppLedBlink_EventControl,PERIOD_LED_BLINK);
//		return;
//	}

	if(*blink_leds != NULL){
		for(ledItem = *blink_leds;;ledItem = ledItem->next){
			// æ����
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
				// ��̬������
				if(ledItem->delay > 0){
					ledItem->delay --;
					// ��ǰ����δ����
					if(ledItem->ccnt > 0){
						ledItem->ccnt --;
					}
					// ��ǰ�����ѽ���
					else{
						ledItem->ccnt = ledItem->period?(ledItem->period - 1):0;	// TODO
						if(ledItem->state)
							ledItem->state = 0;
						else
							ledItem->state = 1;
					}
					ledItem->func_ctl(ledItem->state);
				}
				// ��̬ʱ�����
				else{
					// carel (�Ż���˸��������)
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

