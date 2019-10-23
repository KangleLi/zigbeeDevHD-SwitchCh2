/*=======================================================================*\
* ------------------------------------------------------------------------|
* �ļ�		��_carelSDK/Project/DimSw_CH1_2/ports.c
* ����		���ض���Dimmer Switch ��LED��KEY�ȵײ���ֲʵ��
* ��������		��2019/1
* �����		��Carel.Li
* �޸�����		��
* �޸�����		��
* �汾		��v1.1
*-------------------------------------------------------------------------|
\*=======================================================================*/
#include "../common.h"

//////////////////////////////////////////////////////////////////////////
// 1·ָʾ�� + ����ָʾ�� PF3
//////////////////////////////////////////////////////////////////////////
void LED1_Handler(uint8_t newState){
	if(newState)
		GPIO_PinOutClear(gpioPortF, 3);
	else
		GPIO_PinOutSet(gpioPortF, 3);
}

void LED1_HalInit(void){
	CMU_ClockEnable(cmuClock_GPIO, true);
	GPIO_PinModeSet(gpioPortF, 3, gpioModePushPull, 1);
}


//////////////////////////////////////////////////////////////////////////
// 2·ָʾ�� PA3
//////////////////////////////////////////////////////////////////////////
void LED2_Handler(uint8_t newState){
	if(newState)
		GPIO_PinOutClear(gpioPortA, 3);
	else
		GPIO_PinOutSet(gpioPortA, 3);
}

void LED2_HalInit(void){
	CMU_ClockEnable(cmuClock_GPIO, true);
	GPIO_PinModeSet(gpioPortA, 3, gpioModePushPull, 1);
}

//////////////////////////////////////////////////////////////////////////
// 1·����  PC7
//////////////////////////////////////////////////////////////////////////
uint8_t KEY1_Get(void){
	return (!GPIO_PinInGet(gpioPortC, 7))?1:0;
}
extern void key1Pressed_Callback(void);
void KEY1_pdCB(void){
	key1Pressed_Callback();
}
extern void key1Released_Callback(uint32_t tim);
void KEY1_rdCB(uint32_t tim){
	key1Released_Callback(tim);
}
extern void key1PersistPressed_Callback(uint32_t tim);
void KEY1_ppCB(uint32_t tim){
	key1PersistPressed_Callback(tim);
}

void KEY1_HalInit(void){
	CMU_ClockEnable(cmuClock_GPIO, true);
	GPIO_PinModeSet(gpioPortC, 7, gpioModeInput, 1);
}


//////////////////////////////////////////////////////////////////////////
// 2·���� PD11
//////////////////////////////////////////////////////////////////////////
uint8_t KEY2_Get(void){
	return (!GPIO_PinInGet(gpioPortD, 11))?1:0;
}
extern void key2Pressed_Callback(void);
void KEY2_pdCB(void){
	key2Pressed_Callback();
}
extern void key2Released_Callback(uint32_t tim);
void KEY2_rdCB(uint32_t tim){
	key2Released_Callback(tim);
}
extern void key2PersistPressed_Callback(uint32_t tim);
void KEY2_ppCB(uint32_t tim){
	key2PersistPressed_Callback(tim);
}

void KEY2_HalInit(void){
	CMU_ClockEnable(cmuClock_GPIO, true);
	GPIO_PinModeSet(gpioPortD, 11, gpioModeInput, 1);
}
