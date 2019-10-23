/*=======================================================================*\
* ------------------------------------------------------------------------|
* 文件		：_carelSDK/DimSw_CH1_2/mainCallback.c
* 描述		：特定于Dimmer Swtich的callback实现
* 创建日期		：2018/12
* 设计者		：Carel.Li
* 修改日期		：2019
* 修改内容		：完善
* 版本		：v1.1
*-------------------------------------------------------------------------|
\*=======================================================================*/
#include "common.h"
#include<stdlib.h>
#if 1
//EmberEventControl simonExtendCmd_Timer_EC;

typedef struct {
	bool timerEnable;
	bool goalState;
	uint16_t timeCnt;
}smExtTimedOnOff_t;

static smExtTimedOnOff_t smTimerCHx[] = {
	{false,false,0},
#if CHANNEL_NUM > 1
	{false,false,0},
#if CHANNEL_NUM > 2
	{false,false,0},
#if CHANNEL_NUM > 3
	{false,false,0},
#endif
#endif
#endif
};

void simonExtendCmd_Timer_EF(void){
	uint16_t onTimeAttribute;

//	emberEventControlSetInactive(simonExtendCmd_Timer_EC);

	for(uint8_t i=0;i<CHANNEL_NUM;i++){
		if(smTimerCHx[i].timerEnable){
			if(smTimerCHx[i].timeCnt > 0){
				smTimerCHx[i].timeCnt --;
				onTimeAttribute = smTimerCHx[i].timeCnt;
				emberAfWriteServerAttribute(i+1, \
				  	                         0x0006, \
				  	                         0x4001, \
				  	                         (uint8_t *)&onTimeAttribute,
				  	                         0x21);
				// carel + (每隔1min上报一次)
//				if(smTimerCHx[i].timeCnt % 60 == 0){
//					// 非队列上报
//					_reportStdAttribute(0x0000, i + 1, 1, 0x0006, 0x4001, 1);
//				}
				//
			}
			else{
				smTimerCHx[i].timerEnable = false;
				_reportStdAttribute(0x0000, i+1, 1, 0x0006, 0x4001, 1);
				_writeAttr_OnOff(i+1, smTimerCHx[i].goalState?1:0);
			}
		}
	}
}

void simonExtendCmd_Timer_CTL(uint8_t onoff, uint16_t time, uint8_t ch){
	// 取消定时
	if(time == 0){
		smTimerCHx[ch-1].timerEnable = false;
		smTimerCHx[ch-1].timeCnt = 0;
		smTimerCHx[ch-1].goalState = false;
		_writeAttr_OnTime(ch, 0);
	}
	else{
		smTimerCHx[ch-1].timeCnt = time;
		smTimerCHx[ch-1].goalState = onoff?1:0;
		smTimerCHx[ch-1].timerEnable = true;
	}


}


bool simonExtendCmd_Timer_GetEnable(uint8_t ch){
	return smTimerCHx[ch-1].timerEnable;
}


bool simonExtendCmd_Timer_GetGoalState(uint8_t ch){
	return smTimerCHx[ch-1].goalState;
}

#endif

