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
#include "../common.h"

//extern bool startup;

//////////////////////////////////////////////////////////////////////////
// Main Start
//////////////////////////////////////////////////////////////////////////
extern void _drvInit_SDK(void);
bool emberAfMainStartCallback(int* returnCode,
                              int argc,
                              char** argv){
	// 硬件初始化
	_drvInit_SDK();

	return false;
}


//////////////////////////////////////////////////////////////////////////
// Main Init
//////////////////////////////////////////////////////////////////////////
void emberAfMainInitCallback(void){
#if 0
	// Disable JTAG
	BUS_RegBitWrite(&(GPIO->ROUTEPEN), _GPIO_ROUTEPEN_TDIPEN_SHIFT, false);	// TDIPEN (JTAG Test Debug Input Pin)
	BUS_RegBitWrite(&(GPIO->ROUTEPEN), _GPIO_ROUTEPEN_TDOPEN_SHIFT, false);	// TDOPEN (JTAG Test Debug Output Pin)
#endif
}


//////////////////////////////////////////////////////////////////////////
// Main Tick
//////////////////////////////////////////////////////////////////////////
extern void mainInit_Start(void);
void emberAfMainTickCallback(void){
	static bool F1_Tick = true;
	if(F1_Tick){
		F1_Tick = false;
		mainInit_Start();
	}
}


//////////////////////////////////////////////////////////////////////////
// 进入识别模式
//////////////////////////////////////////////////////////////////////////
void emberAfPluginIdentifyStartFeedbackCallback(uint8_t endpoint,
                                                uint16_t identifyTime){
	if(startup)
		return;

	// 网络指示LED 2Hz闪烁5s后熄灭
	gLed_Active(1, 10, 200);
}


//////////////////////////////////////////////////////////////////////////
// 退出识别模式
//////////////////////////////////////////////////////////////////////////
void emberAfPluginIdentifyStopFeedbackCallback(uint8_t endpoint){
	if(startup)
		return;
}




//////////////////////////////////////////////////////////////////////////
// OnOff Mfg-AttrCng
//////////////////////////////////////////////////////////////////////////
void emberAfOnOffClusterServerManufacturerSpecificAttributeChangedCallback(int8u endpoint,
                                                                           EmberAfAttributeId attributeId,
                                                                           int16u manufacturerCode){

}


//////////////////////////////////////////////////////////////////////////
// Basic AttrCng
//////////////////////////////////////////////////////////////////////////
void emberAfBasicClusterServerAttributeChangedCallback(int8u endpoint,
                                                       EmberAfAttributeId attributeId){
#if 0
	uint8_t buf_auth[41];
//	uint8_t oem_select;

	// 授权码
	if(attributeId == 0xFC01){
		if(emberAfReadServerAttribute(1, \
								 0x0000, \
								 0xFC01, \
								 buf_auth,
								 41) == 0x00){

			// 将授权码写入授权Token
			// 返回0代表写入成功
			if(_setMfgToken_AUTHInfo(buf_auth) == 0){
				DEBUG("_____write token auth success....");
				// token -> attribute
				_auth_token_to_FFE1();
			}
			else{
				DEBUG("_____write token auth failure....");
			}
		}
	}
#endif
}


//////////////////////////////////////////////////////////////////////////
// NWK Write
//////////////////////////////////////////////////////////////////////////
EmberAfAttributeWritePermission emberAfAllowNetworkWriteAttributeCallback(int8u endpoint,
                                                                          EmberAfClusterId clusterId,
                                                                          EmberAfAttributeId attributeId,
                                                                          int8u mask,
                                                                          int16u manufacturerCode,
                                                                          int8u* value,
                                                                          int8u type){
	DEBUG("____WNK Writitng! ep: %d, c: 0x%2X, a: 0x%2X, type: 0x%X, val[0]: 0x%X", endpoint, clusterId, attributeId, type, value[0]);
	switch(clusterId){
#if TUYA_ENABLE
	case 0x0000:
		// 写触发复位（涂鸦机制）
		if(attributeId == 0x8002){
			if(type == 0x10){
				if(value[0] == 1){
					emberAfPluginBasicResetToFactoryDefaultsCallback(1);
				}
				return EMBER_ZCL_ATTRIBUTE_WRITE_PERMISSION_DENY_WRITE;
			}
		}

		break;
#endif
	default:
		break;
	}
#if 0
	uint8_t buf_dat = 0;

	/* --- 扩展 --- */
	// 指示灯模式
//	if(clusterId == 0xFC56){
	if(clusterId == 0x0006){
		switch(attributeId){
//		case 0x0000:
		case 0x8001:
			buf_dat = value[0];
			if(buf_dat > 2)
				break;

			if(_readMfgAttr_Indicator(1) != buf_dat){
				_writeMfgAttr_Indicator(endpoint, buf_dat);
				for(uint8_t i=1;i<=_getEndpointCount();i++){
					_ctlIndicatorSync();
				}
			}
			else{
				_writeMfgAttr_Indicator(endpoint, buf_dat);
			}

			break;
		default:
			break;
		}
		return EMBER_ZCL_ATTRIBUTE_WRITE_PERMISSION_DENY_WRITE;
	}
	else if(clusterId == 0x0000){
		// 厂商配置
		if(attributeId == 0xFC01){
			// !0 - 不允许写
			if(_readMfgAttr_FactoryTest(1)){
				DEBUG("___not allow write 0xFC01...");
				return EMBER_ZCL_ATTRIBUTE_WRITE_PERMISSION_DENY_WRITE;
			}
			// 0 - 厂测允许写
			else{
				DEBUG("___allow write 0xFC01...");
				return EMBER_ZCL_ATTRIBUTE_WRITE_PERMISSION_ALLOW_WRITE_NORMAL;
			}
		}

	}
	else if(clusterId == 0x0008){
		// 输出区间配置
		if(attributeId == 0xFC00){
			if(type == ZCL_INT16U_ATTRIBUTE_TYPE){
				uint8_t buf_max = value[0];
				uint8_t buf_min = value[1];
				if(buf_max > buf_min + LIMIET_LEVEL_DIFF){
					return EMBER_ZCL_ATTRIBUTE_WRITE_PERMISSION_ALLOW_WRITE_NORMAL;
				}
			}
			return EMBER_ZCL_ATTRIBUTE_WRITE_PERMISSION_DENY_WRITE;
		}
	}
#endif
	return EMBER_ZCL_ATTRIBUTE_WRITE_PERMISSION_ALLOW_WRITE_NORMAL;
}



//////////////////////////////////////////////////////////////////////////
// ResetToFactory
//////////////////////////////////////////////////////////////////////////
void emberAfPluginBasicResetToFactoryDefaultsCallback(uint8_t endpoint){
	DEBUG("RESETing .............");
	for(uint8_t ep = 1; ep <= CHANNEL_NUM; ep++){
//		emberAfResetAttributes(emberAfCurrentEndpoint());
		_writeAttr_OnOff(ep, 0);										// onOff
		_writeAttr_OnTime(ep, 0);										// onTime
		_writeAttr_TUYAIndicator(ep, 1);								// indicator
	}

	emberLeaveNetwork();
}




//////////////////////////////////////////////////////////////////////////
// Tuya_ext_cmd callback - reset
//////////////////////////////////////////////////////////////////////////
//extern volatile uint32_t _cnt_I2cErr;
//extern void emberAfResetAttributes(uint8_t endpoint);
boolean emberAfBasicClusterTuya_ext_resetCallback(void){
#if 0
	for(uint8_t ep = 1;ep <= _getEndpointCount();ep ++){
		emberAfResetAttributes(ep);
	}

	RESET();

	// TUYA 复位机制
	_reportStdAttribute(0x0000, 1, 1, 0x0000, 0xFFFE, 1);
#endif
	return true;
}






