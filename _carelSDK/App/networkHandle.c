/*=======================================================================*\
* ------------------------------------------------------------------------|
* 文件		：_carelSDK/Project/DimSw_CH1_2/networkHandle.c
* 描述		：特定于Dimmer Switch 的网络行为处理
* 创建日期		：2018/11/15
* 设计者		：Carel.Li
* 修改日期		：20190214
* 修改内容		：增加入网失败重启次数为3次
* 版本		：v1.1
*-------------------------------------------------------------------------|
\*=======================================================================*/
#include "../common.h"
#include EMBER_AF_API_NETWORK_STEERING
#include EMBER_AF_API_FIND_AND_BIND_TARGET

bool _ifClearSceneBindGroup = true;

static uint8_t _SteeringNwk_End = 2;	// 0-未结束	1-用户结束	2-时间结束
EmberEventControl _SteeringNwk_EC;
void _SteeringNwk_EF(void){
	emberEventControlSetInactive(_SteeringNwk_EC);
	_SteeringNwk_End = 2;
}

// commissioning 事件
EmberEventControl commissioning_EventControl;
void commissioning_EventFunction(void){
	emberEventControlSetInactive(commissioning_EventControl);
	_ifClearSceneBindGroup = true;	// carel + >20190925
	if(emberAfNetworkState() == EMBER_JOINED_NETWORK){
		// nothing
	}
	else{
		// 启动steering
		emberAfPluginNetworkSteeringStart();
		// 启动入网计时器
		if((!emberEventControlGetActive(_SteeringNwk_EC)) && (_SteeringNwk_End > 0)){
			_SteeringNwk_End = 0;
			emberEventControlSetDelayMS(_SteeringNwk_EC, _LIMIT_STEERING_NWK);
			// 网络指示LED闪烁频率4Hz ("失控状态下限制入网闪烁4min")
			gLed_Active(NETLED, 5, 40*60*4);
		}
	}
}


// callback
// Network Steering Complete
void emberAfPluginNetworkSteeringCompleteCallback(EmberStatus status,
                                                  uint8_t totalBeacons,
                                                  uint8_t joinAttempts,
                                                  uint8_t finalState){
	if(status == EMBER_SUCCESS){
		_SteeringNwk_End = 2;
		emberEventControlSetInactive(_SteeringNwk_EC);
//		DEBUG("______________NWK steering success!!!!!!!!!!!!!!!!!!!");
	}
	// steering失败重启commissioning事件
	else{
//		DEBUG("______________NWK steering failure!!!!!!!!!!!!!!!!!!!");
		if(emberEventControlGetActive(_SteeringNwk_EC)){
			// 过程中
			if(_SteeringNwk_End == 0){
				startCommissioning(40);
//				DEBUG("______restart steering!");
			}
			// 用户结束
			else if(_SteeringNwk_End == 1){
//				DEBUG("______abort steering!");
				emberEventControlSetInactive(_SteeringNwk_EC);
			}
			// 不可能执行
			else{
				_SteeringNwk_End = 1;
				emberEventControlSetInactive(_SteeringNwk_EC);
			}
		}
		//
		else{
			if(_SteeringNwk_End == 2){
				// 网络LED 1Hz闪烁10s
				gLed_Active(NETLED, 20, 400);
				_SteeringNwk_End = true;
			}
			else{
				_SteeringNwk_End = 1;
			}
		}
	}
}

// FindingAndBinding 事件
EmberEventControl findingandbinding_EventControl;
void findingandbinding_EventFunction(void){
	emberEventControlSetInactive(findingandbinding_EventControl);

	// 启动finding&binding
	if(emberAfNetworkState() == EMBER_JOINED_NETWORK){
		emberAfPluginFindAndBindTargetStart(1);
	}
}

void clearNWK(void){
	// 清除binding
	emberClearBindingTable();

	// 清除reporting
	emberAfClearReportTableCallback();

	// 清除groups、scenes
	for(uint8_t ep = 1;ep <= _getEndpointCount();ep ++){
		emberAfGroupsClusterClearGroupTableCallback(ep);
		emberAfScenesClusterClearSceneTableCallback(ep);
	}
}


// callback
// stack status
bool emberAfStackStatusCallback(EmberStatus status)
{
	if(startup)
		return false;

	if(status == EMBER_NETWORK_UP){
//		DEBUG("______________NWK UP!!!!!!!!!!!!!!!!!!!");
		_SteeringNwk_End = 1;	// abort

		// HD 多次Announce
		_carelDeviceAnnounce();
		_carelDeviceAnnounce();
		_carelDeviceAnnounce();
		// 20191021

		// 网络LED 2Hz闪烁2s
		gLed_Active(NETLED, 10, 80);

		// 启动finding binding
//		startFindingAndBinding(1);

#if (TUYA_ENABLE)
		// TUYA 复位机制
		_reportStdAttribute(0x0000, 1, 1, 0x0000, 0xFFFE, 1);
#endif

		// 同步上报
		_perReportCmd(0, _PerReport_Cmd_ShortTemp);

	}
	else if(status == EMBER_NETWORK_DOWN && \
			(emberAfNetworkState() == EMBER_NO_NETWORK || \
					emberAfNetworkState() == EMBER_JOINED_NETWORK_NO_PARENT)){
//		DEBUG("______________NWK DOWN!!!!!!!!!!!!!!!!!!!");
		// 清除网络参数
		if(_ifClearSceneBindGroup)
			clearNWK();

		// 清除倒计时
		for(uint8_t ep = 1; ep <= CHANNEL_NUM; ep++){
			if(simonExtendCmd_Timer_GetEnable(ep))
				simonExtendCmd_Timer_CTL(0, 0, ep);
		}

		// 网络LED 2Hz闪烁2s
		gLed_Active(NETLED, 10, 80);

		// HD
		emberEventControlSetInactive(_SteeringNwk_EC);
		_SteeringNwk_End = 2;
		startCommissioning(3000);
		// 20191018
	}

	return false;
}


// 激活Commissioning event
void startCommissioning(uint32_t delay){
	emberEventControlSetDelayMS(commissioning_EventControl,delay);
}


// 激活findingandbinding event
void startFindingAndBinding(uint32_t delay){
	emberEventControlSetDelayMS(findingandbinding_EventControl,delay);
}
