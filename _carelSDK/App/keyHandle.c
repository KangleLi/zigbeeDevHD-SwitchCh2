/*=======================================================================*\
* ------------------------------------------------------------------------|
* 文件		：_carelSDK/Project/DimSw_CH1_2/keyHandle.c
* 描述		：特定于Dimmer Switch 的按键行为处理，包括下按、抬起、持续按下
* 创建日期		：2018/11/15
* 设计者		：Carel.Li
* 修改日期		：2019/1
* 修改内容		：架构重写
* 版本		：v1.1
*-------------------------------------------------------------------------|
\*=======================================================================*/
#include "../common.h"

//////////////////////////////////////////////////////////////////////////
// ↓↓↓ 配置 ↓↓↓
//////////////////////////////////////////////////////////////////////////
#define TIM_JOIN_NETWORK			3000			// 长按3s加入网络
#define TIM_LEAVE_NETWORK			20000			// 长按10s离开网络
//////////////////////////////////////////////////////////////////////////
// ↑↑↑  配置 ↑↑↑
//////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////
// 1路按键处理
//////////////////////////////////////////////////////////////////////////
void key1Pressed_Callback(void){
	//DEBUG("===Key1 Pressed!");
	if(startup)
		return;

	_switchAttr_OnOff(1);
}

void key1Released_Callback(uint32_t tim){
	//DEBUG("===Key1 Released!");
	if(startup)
		return;

}
// period: 50ms
void key1PersistPressed_Callback(uint32_t tim){
	if(startup)
		return;

	if(tim == TIM_JOIN_NETWORK){
		startCommissioning(1);
	}
	else if(tim == TIM_LEAVE_NETWORK){
		extern EmberEventControl _SteeringNwk_EC;
		// 入网状态获取
		if(!emberEventControlGetActive(_SteeringNwk_EC)){
			_ifClearSceneBindGroup = false;
			emberLeaveNetwork();
			startCommissioning(7000);
		}
	}
}


//////////////////////////////////////////////////////////////////////////
// 2路按键处理
//////////////////////////////////////////////////////////////////////////
void key2Pressed_Callback(void){
	//DEBUG("===Key2 Pressed!");
	if(startup)
		return;

	_switchAttr_OnOff(2);
}

void key2Released_Callback(uint32_t tim){
	//DEBUG("===Key2 Released!");
	if(startup)
		return;

}

void key2PersistPressed_Callback(uint32_t tim){
	if(startup)
		return;
}


//////////////////////////////////////////////////////////////////////////
// 3路按键处理
//////////////////////////////////////////////////////////////////////////
void key3Pressed_Callback(void){
	//DEBUG("===Key3 Pressed!");
	if(startup)
		return;

	_switchAttr_OnOff(3);
}

void key3Released_Callback(uint32_t tim){
	//DEBUG("===Key3 Released!");
	if(startup)
		return;

}

void key3PersistPressed_Callback(uint32_t tim){
	if(startup)
		return;
}


//////////////////////////////////////////////////////////////////////////
// 4路按键处理
//////////////////////////////////////////////////////////////////////////
void key4Pressed_Callback(void){
//	DEBUG("===Key4 Pressed!");
	if(startup)
		return;

	_switchAttr_OnOff(4);
}

void key4Released_Callback(uint32_t tim){
//	DEBUG("===Key4 Released!");
	if(startup)
		return;

}

void key4PersistPressed_Callback(uint32_t tim){
	if(startup)
		return;
}

