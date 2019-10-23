/*=======================================================================*\
* ------------------------------------------------------------------------|
* 文件		：_carelSDK/Z3Extend/Ext_Bind.c
* 描述		：通用Cluster操作相关API扩展实现
* 创建日期		：2018/12
* 设计者		：Carel.Li
* 修改日期		：2019/1
* 修改内容		：新增Dimmer相关属性读写
* 版本		：v1.1
*-------------------------------------------------------------------------|
\*=======================================================================*/
#include "../common.h"


EmberAfStatus _switchAttr_OnOff(uint8_t ep){
	return _writeAttr_OnOff(ep, _readAttr_OnOff(ep)==0?1:0);
}

EmberAfStatus _setAttr_Level(uint8_t ep, uint8_t level){
	return _writeAttr_Level(ep, level);
}


EmberAfStatus _writeAttr_OnOff(uint8_t ep, uint8_t onoff){
	uint8_t buf_onoff = onoff;
	return emberAfWriteServerAttribute(ep, \
	                         0x0006, \
	                         0x0000, \
	                         &buf_onoff,
	                         0x10);
}

EmberAfStatus _writeAttr_OnTime(uint8_t ep, uint16_t dat){
	uint16_t buf_dat = dat;
	return emberAfWriteServerAttribute(ep, \
	                         0x0006, \
	                         0x4001, \
	                         (uint8_t *)(&buf_dat),
	                         0x21);
}

EmberAfStatus _writeAttr_TUYAIndicator(uint8_t ep, uint8_t dat){
	uint8_t buf_dat = dat;
	return emberAfWriteServerAttribute(ep, \
							 0x0006, \
							 0x8001, \
							 (uint8_t *)(&buf_dat),
							 0x30);
}

uint8_t _readAttr_OnOff(uint8_t ep){
	uint8_t buf_onoff;
	if(emberAfReadServerAttribute(ep, \
	                         0x0006, \
	                         0x0000, \
	                         &buf_onoff,
	                         1) == 0){
		return buf_onoff;
	}
	else
		return 0xFF;
}

uint16_t _readAttr_OnTime(uint8_t ep){
	uint16_t buf_ontime;
	if(emberAfReadServerAttribute(ep, \
	                         0x0006, \
	                         0x4001, \
	                         (uint8_t *)&buf_ontime,
	                         2) == 0){
		return buf_ontime;
	}
	else
		return 0xFFFF;
}

uint8_t _readAttr_TUYAIndicator(uint8_t ep){
	uint8_t buf_dat;
	if(emberAfReadServerAttribute(ep, \
							 0x0006, \
							 0x8001, \
							 (uint8_t *)&buf_dat,
							 1) == 0){
		return buf_dat;
	}
	else
		return 0xFF;
}

uint16_t _readAttr_OffWaitTime(uint8_t ep){
	uint16_t buf_offwaittime;
	if(emberAfReadServerAttribute(ep, \
	                         0x0006, \
	                         0x4002, \
	                         (uint8_t *)&buf_offwaittime,
	                         2) == 0){
		return buf_offwaittime;
	}
	else
		return 0xFFFF;
}

EmberAfStatus _writeAttr_Level(uint8_t ep, uint8_t level){
	uint8_t buf_level = level;
	return emberAfWriteServerAttribute(ep, \
	                         0x0008, \
	                         0x0000, \
	                         &buf_level,
	                         0x20);
}

EmberAfStatus _writeAttr_LevelMaxMin(uint8_t ep, uint8_t *dat){
	return emberAfWriteServerAttribute(ep, \
							 0x0008, \
							 0xFC00, \
							 dat,
							 ZCL_INT16U_ATTRIBUTE_TYPE);
}

EmberAfStatus _writeAttr_startupLevel(uint8_t ep, uint8_t level){
	uint8_t buf_level = level;
	return emberAfWriteServerAttribute(ep, \
	                         0x0008, \
	                         0x4000, \
	                         &buf_level,
	                         0x20);
}

EmberAfStatus _writeAttr_onLevel(uint8_t ep, uint8_t level){
	uint8_t buf_level = level;
	return emberAfWriteServerAttribute(ep, \
	                         0x0008, \
	                         0x0011, \
	                         &buf_level,
	                         0x20);
}

uint8_t _readAttr_Level(uint8_t ep){
	uint8_t buf_level;
	emberAfReadServerAttribute(ep, \
	                         0x0008, \
	                         0x0000, \
	                         &buf_level,
	                         1);
	return buf_level;
}

uint8_t _readAttr_startupLevel(uint8_t ep){
	uint8_t buf_level;
	emberAfReadServerAttribute(ep, \
							 0x0008, \
							 0x4000, \
							 &buf_level,
							 1);
	return buf_level;
}

uint8_t _readAttr_onLevel(uint8_t ep){
	uint8_t buf_level;
	emberAfReadServerAttribute(ep, \
							 0x0008, \
							 0x0011, \
							 &buf_level,
							 1);
	return buf_level;
}

EmberAfStatus _writeMfgAttr_Indicator(uint8_t ep, uint8_t newState){
//	uint8_t bufNewState = newState;
	uint8_t bufNewState[2];
	bufNewState[0] = newState;
	bufNewState[1] = 0;
#if 0
	return emberAfWriteManufacturerSpecificServerAttribute(ep,
            0xFC56,
            0x0000,
			EMBER_AF_MANUFACTURER_CODE,
			&bufNewState,
            0x20);
#else
	return emberAfWriteServerAttribute(ep, \
		                         0x0006, \
		                         0x8001, \
		                         bufNewState,
		                         0x30);
#endif
}

uint8_t _readMfgAttr_Indicator(uint8_t ep){
	uint8_t bufNewState[2];
#if 0
	emberAfReadManufacturerSpecificServerAttribute(ep,
	        0xFC56,
			0x0000,
			EMBER_AF_MANUFACTURER_CODE,
			&bufNewState,
			1);
#else
	emberAfReadServerAttribute(ep, \
			 0x0006, \
			 0x8001, \
			 bufNewState,
			 2);
#endif
	return bufNewState[0];
}

EmberAfStatus _writeMfgAttr_prMax(uint8_t ep, uint8_t dat){
	uint8_t bufdat = dat;
	return emberAfWriteManufacturerSpecificServerAttribute(ep,
			0xFC80,
			0x0001,
			EMBER_AF_MANUFACTURER_CODE,
			&bufdat,
			0x20);
}

EmberAfStatus _writeMfgAttr_prMin(uint8_t ep, uint8_t dat){
	uint8_t bufdat = dat;
	return emberAfWriteManufacturerSpecificServerAttribute(ep,
			0xFC80,
			0x0002,
			EMBER_AF_MANUFACTURER_CODE,
			&bufdat,
			0x20);
}

uint8_t _readMfgAttr_prMax(uint8_t ep){
	uint8_t bufdat;
	emberAfReadManufacturerSpecificServerAttribute(ep,
			0xFC80,
			0x0001,
			EMBER_AF_MANUFACTURER_CODE,
			&bufdat,
			1);
	return bufdat;
}

uint8_t _readMfgAttr_prMin(uint8_t ep){
	uint8_t bufdat;
	emberAfReadManufacturerSpecificServerAttribute(ep,
			0xFC80,
			0x0002,
			EMBER_AF_MANUFACTURER_CODE,
			&bufdat,
			1);
	return bufdat;
}

EmberAfStatus _writeMfgAttr_QTSens(uint8_t ep, uint8_t dat){
	uint8_t bufdat = dat;
	return emberAfWriteManufacturerSpecificServerAttribute(ep,
			0xFC80,
			0x0000,
			EMBER_AF_MANUFACTURER_CODE,
			&bufdat,
			0x20);
}

EmberAfStatus _writeMfgAttr_DimSpeed(uint8_t ep, uint8_t dat){
	uint8_t bufdat = dat;
	return emberAfWriteManufacturerSpecificServerAttribute(ep,
			0xFC80,
			0x0003,
			EMBER_AF_MANUFACTURER_CODE,
			&bufdat,
			0x20);
}
EmberAfStatus _writeMfgAttr_DimPressTim(uint8_t ep, uint8_t dat){
	uint8_t bufdat = dat;
	return emberAfWriteManufacturerSpecificServerAttribute(ep,
			0xFC80,
			0x0004,
			EMBER_AF_MANUFACTURER_CODE,
			&bufdat,
			0x20);
}

EmberAfStatus _writeMfgAttr_DimControl(uint8_t ep, uint16_t dat){
	uint16_t bufdat = dat;
	return emberAfWriteManufacturerSpecificServerAttribute(ep,
			0xFC80,
			0x000C,
			EMBER_AF_MANUFACTURER_CODE,
			(uint8_t *)&bufdat,
			ZCL_INT16U_ATTRIBUTE_TYPE);
}

// carel_add
EmberAfStatus _writeMfgAttr_FactoryTest(uint8_t ep, uint8_t dat){
	uint8_t bufdat = dat;
	return emberAfWriteManufacturerSpecificServerAttribute(ep,
			0xFC55,
			0x0002,
			EMBER_AF_MANUFACTURER_CODE,
			&bufdat,
			0x20);
}

// 20190218

EmberAfStatus _writeAttr_LedsPosition(uint8_t ep, uint8_t dat){
	uint8_t buf_dat = dat>7?7:dat;

	return emberAfWriteServerAttribute(ep, \
							 0x0008, \
							 0xFC01, \
							 &buf_dat,
							 0x20);
}


uint8_t _readAttr_LedsPosition(uint8_t ep){
	uint8_t buf_dat;

	emberAfReadServerAttribute(ep, \
				 0x0008, \
				 0xFC01, \
				 &buf_dat,
				 1);

	return buf_dat;
}




uint8_t _readMfgAttrQtSens(uint8_t ep){
	uint8_t bufdat;
	emberAfReadManufacturerSpecificServerAttribute(ep,
			0xFC80,
			0x0000,
			EMBER_AF_MANUFACTURER_CODE,
			&bufdat,
			1);
	return bufdat;
}

uint8_t _readMfgAttr_dimSpeed(uint8_t ep){
	uint8_t bufdat;
	emberAfReadManufacturerSpecificServerAttribute(ep,
			0xFC80,
			0x0003,
			EMBER_AF_MANUFACTURER_CODE,
			&bufdat,
			1);
	return bufdat;
}

uint8_t _readMfgAttr_dimPressTim(uint8_t ep){
	uint8_t bufdat;
	emberAfReadManufacturerSpecificServerAttribute(ep,
			0xFC80,
			0x0004,
			EMBER_AF_MANUFACTURER_CODE,
			&bufdat,
			1);
	return bufdat;
}



// carel_add
uint8_t _readMfgAttr_FactoryTest(uint8_t ep){
	uint8_t bufdat;
	emberAfReadManufacturerSpecificServerAttribute(ep,
			0xFC55,
			0x0002,
			EMBER_AF_MANUFACTURER_CODE,
			&bufdat,
			1);
	return bufdat;
}
// 20190218


uint8_t _readAttr_OTAStatus(uint8_t ep){
	uint8_t buf_status;
	emberAfReadClientAttribute(1, 0x0019, 0x0006, &buf_status, 1);
	return buf_status;
}



uint8_t _getLevelMax(uint8_t ep){
	uint8_t bufdat[2];
	emberAfReadServerAttribute(ep, \
			 0x0008, \
			 0xFC00, \
			 &bufdat,
			 2);
	return bufdat[0];
}

uint8_t _getLevelMin(uint8_t ep){
	uint8_t bufdat[2];
	emberAfReadServerAttribute(ep, \
			 0x0008, \
			 0xFC00, \
			 &bufdat,
			 2);
	return bufdat[1];
}
