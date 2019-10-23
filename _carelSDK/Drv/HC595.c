#include "../common.h"
#if 0
// ԭ��
//  	д8bits���ݵ���λ�Ĵ���  ��
//		��λ�Ĵ���ת�� -> �洢�Ĵ���
// ���ţ�
//		11-SRCLK		��λ�Ĵ���ʱ�ӣ���������Ч
//		12-RCLK			�洢�Ĵ���ʱ�ӣ���������Ч��������
//		14-SER			��������
// ���գ�
//		SRCLK			4(PA1)
//		RCLK			5(PA2)
//		SER				7(PA3)

#define _PIN_HL_SRCLK_H()			GPIO_PinOutSet(gpioPortA, 1)
#define _PIN_HL_RCLK_H()			GPIO_PinOutSet(gpioPortA, 2)
#define _PIN_HL_SER_H()				GPIO_PinOutSet(gpioPortA, 3)
#define _PIN_HL_SRCLK_L()			GPIO_PinOutClear(gpioPortA, 1)
#define _PIN_HL_RCLK_L()			GPIO_PinOutClear(gpioPortA, 2)
#define _PIN_HL_SER_L()				GPIO_PinOutClear(gpioPortA, 3)

// ����ʱ
#define _Local_DrvDelay(t)	{ \
	for(uint16_t i = 0;i<t;i++){ \
		__asm__( "nop" ); \
		__asm__( "nop" ); \
		__asm__( "nop" ); \
	} \
}

static void _Local_DrvWr8bits(uint8_t dat);






void _DrvInit_HC595(void){
	CMU_ClockEnable(cmuClock_GPIO, true);
	GPIO_PinModeSet(gpioPortA, 1, gpioModePushPull, 0);
	GPIO_PinModeSet(gpioPortA, 2, gpioModePushPull, 0);
	GPIO_PinModeSet(gpioPortA, 3, gpioModePushPull, 0);

	_Local_DrvWr8bits(0);	// �ر����м̵�������
	DEBUG("[Drv]all channel off 0xFF");
}

static void _Local_DrvWr8bits(uint8_t dat){
	_Local_DrvDelay(10);
	for(uint8_t i = 0; i < 8; i++){
		if(dat & (1 << (7 - i))){
			_PIN_HL_SER_H();
		}
		else{
			_PIN_HL_SER_L();
		}
		_Local_DrvDelay(5);
		_PIN_HL_SRCLK_H();
		_Local_DrvDelay(20);
		_PIN_HL_SRCLK_L();
	}
	_Local_DrvDelay(5);
	_PIN_HL_RCLK_H();
	_Local_DrvDelay(20);
	_PIN_HL_RCLK_L();
}


// 7531
// 6420
void _DrvWr_HC595(uint8_t ch, uint8_t newStatus){

//return;
	uint8_t _outDat = 0xFF;

	uint8_t _sp_h = 0;
	uint8_t _sp_l = 0;
	if(ch == 1){
		_sp_h = 7;
		_sp_l = 6;
	}
	else if(ch == 2){
		_sp_h = 5;
		_sp_l = 4;
	}
	else if(ch == 3){
		_sp_h = 3;
		_sp_l = 2;
	}
	else if(ch == 4){
		_sp_h = 1;
		_sp_l = 0;
	}
	else{
		return;
	}

//	// ��һbit�ı�
//	_outDat &= ~(1 << _sp_);
//	_outDat |= ((newStatus?1:0) << _sp_);

	// д��
	if(newStatus){
		_outDat &= ~(1 << _sp_h);
	}
	else{
		_outDat &= ~(1 << _sp_l);
	}
	_Local_DrvWr8bits(~_outDat);

	// ��ʱ����������ƽ����������
	extern EmberEventControl _DrvHC595_PinSet_EC;
	emberEventControlSetDelayMS(_DrvHC595_PinSet_EC, _HC595_INTERVAL);

	DEBUG("[Drv]ch %d, dat %d, outDat[0x%X]\n", ch, newStatus, _outDat);
}

// 7531
// 6420
void _DrvWr_HC595_All(uint8_t newStatusMask){

//return;
	uint8_t _outDat = 0xFF;

	if(newStatusMask & (1 << 1)){
		_outDat &= ~(1 << 7);
	}
	else{
		_outDat &= ~(1 << 6);
	}
#if (CHANNEL_NUM > 1)
	if(newStatusMask & (1 << 2)){
		_outDat &= ~(1 << 5);
	}
	else{
		_outDat &= ~(1 << 4);
	}
#if (CHANNEL_NUM > 2)
	if(newStatusMask & (1 << 3)){
		_outDat &= ~(1 << 3);
	}
	else{
		_outDat &= ~(1 << 2);
	}
#if (CHANNEL_NUM > 3)
	if(newStatusMask & (1 << 4)){
		_outDat &= ~(1 << 1);
	}
	else{
		_outDat &= ~(1 << 0);
	}
#endif
#endif
#endif

	_Local_DrvWr8bits(~_outDat);

	// ��ʱ����������ƽ����������
	extern EmberEventControl _DrvHC595_PinSet_EC;
	emberEventControlSetDelayMS(_DrvHC595_PinSet_EC, _HC595_INTERVAL);

	DEBUG("[Drv]all --wr, outDat[0x%X]\n", _outDat);
}

EmberEventControl _DrvHC595_PinSet_EC;
void _DrvHC595_PinSet_EF(void){
	emberEventControlSetInactive(_DrvHC595_PinSet_EC);

	_Local_DrvWr8bits(0);	// �ر����м̵�������
	DEBUG("[Drv]all channel off 0xFF");
}


#endif










