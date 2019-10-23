/*=======================================================================*\
* ------------------------------------------------------------------------|
* �ļ�		��_carelSDK/Project/DimSw_CH1_2/networkHandle.c
* ����		���ض���Dimmer Switch ��������Ϊ����
* ��������		��2018/11/15
* �����		��Carel.Li
* �޸�����		��20190214
* �޸�����		����������ʧ����������Ϊ3��
* �汾		��v1.1
*-------------------------------------------------------------------------|
\*=======================================================================*/
#include "../common.h"
#include EMBER_AF_API_NETWORK_STEERING
#include EMBER_AF_API_FIND_AND_BIND_TARGET

bool _ifClearSceneBindGroup = true;

static uint8_t _SteeringNwk_End = 2;	// 0-δ����	1-�û�����	2-ʱ�����
EmberEventControl _SteeringNwk_EC;
void _SteeringNwk_EF(void){
	emberEventControlSetInactive(_SteeringNwk_EC);
	_SteeringNwk_End = 2;
}

// commissioning �¼�
EmberEventControl commissioning_EventControl;
void commissioning_EventFunction(void){
	emberEventControlSetInactive(commissioning_EventControl);
	_ifClearSceneBindGroup = true;	// carel + >20190925
	if(emberAfNetworkState() == EMBER_JOINED_NETWORK){
		// nothing
	}
	else{
		// ����steering
		emberAfPluginNetworkSteeringStart();
		// ����������ʱ��
		if((!emberEventControlGetActive(_SteeringNwk_EC)) && (_SteeringNwk_End > 0)){
			_SteeringNwk_End = 0;
			emberEventControlSetDelayMS(_SteeringNwk_EC, _LIMIT_STEERING_NWK);
			// ����ָʾLED��˸Ƶ��4Hz ("ʧ��״̬������������˸4min")
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
	// steeringʧ������commissioning�¼�
	else{
//		DEBUG("______________NWK steering failure!!!!!!!!!!!!!!!!!!!");
		if(emberEventControlGetActive(_SteeringNwk_EC)){
			// ������
			if(_SteeringNwk_End == 0){
				startCommissioning(40);
//				DEBUG("______restart steering!");
			}
			// �û�����
			else if(_SteeringNwk_End == 1){
//				DEBUG("______abort steering!");
				emberEventControlSetInactive(_SteeringNwk_EC);
			}
			// ������ִ��
			else{
				_SteeringNwk_End = 1;
				emberEventControlSetInactive(_SteeringNwk_EC);
			}
		}
		//
		else{
			if(_SteeringNwk_End == 2){
				// ����LED 1Hz��˸10s
				gLed_Active(NETLED, 20, 400);
				_SteeringNwk_End = true;
			}
			else{
				_SteeringNwk_End = 1;
			}
		}
	}
}

// FindingAndBinding �¼�
EmberEventControl findingandbinding_EventControl;
void findingandbinding_EventFunction(void){
	emberEventControlSetInactive(findingandbinding_EventControl);

	// ����finding&binding
	if(emberAfNetworkState() == EMBER_JOINED_NETWORK){
		emberAfPluginFindAndBindTargetStart(1);
	}
}

void clearNWK(void){
	// ���binding
	emberClearBindingTable();

	// ���reporting
	emberAfClearReportTableCallback();

	// ���groups��scenes
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

		// HD ���Announce
		_carelDeviceAnnounce();
		_carelDeviceAnnounce();
		_carelDeviceAnnounce();
		// 20191021

		// ����LED 2Hz��˸2s
		gLed_Active(NETLED, 10, 80);

		// ����finding binding
//		startFindingAndBinding(1);

#if (TUYA_ENABLE)
		// TUYA ��λ����
		_reportStdAttribute(0x0000, 1, 1, 0x0000, 0xFFFE, 1);
#endif

		// ͬ���ϱ�
		_perReportCmd(0, _PerReport_Cmd_ShortTemp);

	}
	else if(status == EMBER_NETWORK_DOWN && \
			(emberAfNetworkState() == EMBER_NO_NETWORK || \
					emberAfNetworkState() == EMBER_JOINED_NETWORK_NO_PARENT)){
//		DEBUG("______________NWK DOWN!!!!!!!!!!!!!!!!!!!");
		// ����������
		if(_ifClearSceneBindGroup)
			clearNWK();

		// �������ʱ
		for(uint8_t ep = 1; ep <= CHANNEL_NUM; ep++){
			if(simonExtendCmd_Timer_GetEnable(ep))
				simonExtendCmd_Timer_CTL(0, 0, ep);
		}

		// ����LED 2Hz��˸2s
		gLed_Active(NETLED, 10, 80);

		// HD
		emberEventControlSetInactive(_SteeringNwk_EC);
		_SteeringNwk_End = 2;
		startCommissioning(3000);
		// 20191018
	}

	return false;
}


// ����Commissioning event
void startCommissioning(uint32_t delay){
	emberEventControlSetDelayMS(commissioning_EventControl,delay);
}


// ����findingandbinding event
void startFindingAndBinding(uint32_t delay){
	emberEventControlSetDelayMS(findingandbinding_EventControl,delay);
}
