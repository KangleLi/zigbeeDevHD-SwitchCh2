#include "../common.h"



static void _Local_DrvWr8bits(uint8_t dat);

/* -- IO 对照表 --
 * load1	load2	load3	load4	load5	load6	load7	load8
 *  |		 |		 |		 |		 |		 |		 |		 |
 * PB15		PB11	PC8		PC9		PF2		PC11	PD15	PA5
 */

static void _Local_DrvWr8bits(uint8_t dat){
	if(dat&(1 << 7)){
		GPIO_PinOutSet(gpioPortB, 15);
	}else{
		GPIO_PinOutClear(gpioPortB, 15);
	}

	if(dat&(1 << 6)){
		GPIO_PinOutSet(gpioPortB, 11);
	}else{
		GPIO_PinOutClear(gpioPortB, 11);
	}

	if(dat&(1 << 5)){
		GPIO_PinOutSet(gpioPortC, 8);
	}else{
		GPIO_PinOutClear(gpioPortC, 8);
	}

	if(dat&(1 << 4)){
		GPIO_PinOutSet(gpioPortC, 9);
	}else{
		GPIO_PinOutClear(gpioPortC, 9);
	}

	if(dat&(1 << 3)){
		GPIO_PinOutSet(gpioPortF, 2);
	}else{
		GPIO_PinOutClear(gpioPortF, 2);
	}

	if(dat&(1 << 2)){
		GPIO_PinOutSet(gpioPortC, 11);
	}else{
		GPIO_PinOutClear(gpioPortC, 11);
	}

	if(dat&(1 << 1)){
		GPIO_PinOutSet(gpioPortD, 15);
	}else{
		GPIO_PinOutClear(gpioPortD, 15);
	}

	if(dat&(1 << 0)){
		GPIO_PinOutSet(gpioPortA, 5);
	}else{
		GPIO_PinOutClear(gpioPortA, 5);
	}
}


void _DrvInit_MLR(void){
	CMU_ClockEnable(cmuClock_GPIO, true);
	GPIO_PinModeSet(gpioPortB, 15, gpioModePushPull, 0);
	GPIO_PinModeSet(gpioPortB, 11, gpioModePushPull, 0);
	GPIO_PinModeSet(gpioPortC, 8, gpioModePushPull, 0);
	GPIO_PinModeSet(gpioPortC, 9, gpioModePushPull, 0);
	GPIO_PinModeSet(gpioPortF, 2, gpioModePushPull, 0);
	GPIO_PinModeSet(gpioPortC, 11, gpioModePushPull, 0);
	GPIO_PinModeSet(gpioPortD, 15, gpioModePushPull, 0);
	GPIO_PinModeSet(gpioPortA, 5, gpioModePushPull, 0);

	_Local_DrvWr8bits(0);		// 关闭继电器电流
}


// 7531
// 6420
void _DrvWr_MLR(uint8_t ch, uint8_t newStatus){

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

	// 写入
	if(newStatus){
		_outDat &= ~(1 << _sp_h);
	}
	else{
		_outDat &= ~(1 << _sp_l);
	}
	_Local_DrvWr8bits(~_outDat);

	// 延时拉高驱动电平，消除电流
	extern EmberEventControl _DrvMLR_PinSet_EC;
	emberEventControlSetDelayMS(_DrvMLR_PinSet_EC, _MLR_INTERVAL);

	DEBUG("[Drv]ch %d, dat %d, outDat[0x%X]\n", ch, newStatus, _outDat);
}

// 7531
// 6420
void _DrvWr_MLR_All(uint8_t newStatusMask){

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

	// 延时拉高驱动电平，消除电流
	extern EmberEventControl _DrvMLR_PinSet_EC;
	emberEventControlSetDelayMS(_DrvMLR_PinSet_EC, _MLR_INTERVAL);

	DEBUG("[Drv]all --wr, outDat[0x%X]\n", _outDat);
}

EmberEventControl _DrvMLR_PinSet_EC;
void _DrvMLR_PinSet_EF(void){
	emberEventControlSetInactive(_DrvMLR_PinSet_EC);

	_Local_DrvWr8bits(0);	// 关闭所有继电器电流
	DEBUG("[Drv]all channel off 0xFF");
}




