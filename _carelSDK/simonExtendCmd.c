/*=======================================================================*\
* ------------------------------------------------------------------------|
* �ļ�		��_carelSDK/DimSw_CH1_2/mainCallback.c
* ����		���ض���Dimmer Swtich��callbackʵ��
* ��������		��2018/12
* �����		��Carel.Li
* �޸�����		��2019
* �޸�����		������
* �汾		��v1.1
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
				// carel + (ÿ��1min�ϱ�һ��)
//				if(smTimerCHx[i].timeCnt % 60 == 0){
//					// �Ƕ����ϱ�
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
	// ȡ����ʱ
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

