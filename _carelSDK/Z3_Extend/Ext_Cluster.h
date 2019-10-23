#ifndef CARELSDK_Z3_EXTEND_EXT_CLUSTER_H_
#define CARELSDK_Z3_EXTEND_EXT_CLUSTER_H_

EmberAfStatus _switchAttr_OnOff(uint8_t ep);
EmberAfStatus _setAttr_Level(uint8_t ep, uint8_t level);
EmberAfStatus _writeAttr_OnOff(uint8_t ep, uint8_t onoff);
EmberAfStatus _writeAttr_OnTime(uint8_t ep, uint16_t dat);
EmberAfStatus _writeAttr_TUYAIndicator(uint8_t ep, uint8_t dat);
EmberAfStatus _writeAttr_Level(uint8_t ep, uint8_t level);
EmberAfStatus _writeAttr_startupLevel(uint8_t ep, uint8_t level);
EmberAfStatus _writeAttr_onLevel(uint8_t ep, uint8_t level);
EmberAfStatus _writeMfgAttr_Indicator(uint8_t ep, uint8_t newState);
//EmberAfStatus _writeMfgAttr_prMax(uint8_t ep, uint8_t dat);
//EmberAfStatus _writeMfgAttr_prMin(uint8_t ep, uint8_t dat);
//EmberAfStatus _writeMfgAttr_QTSens(uint8_t ep, uint8_t dat);
//EmberAfStatus _writeMfgAttr_DimSpeed(uint8_t ep, uint8_t dat);
//EmberAfStatus _writeMfgAttr_DimPressTim(uint8_t ep, uint8_t dat);
//EmberAfStatus _writeMfgAttr_DimControl(uint8_t ep, uint16_t dat);
// carel_add
EmberAfStatus _writeMfgAttr_FactoryTest(uint8_t ep, uint8_t dat);
// 20190218
EmberAfStatus _writeAttr_LevelMaxMin(uint8_t ep, uint8_t *dat);
EmberAfStatus _writeAttr_LedsPosition(uint8_t ep, uint8_t dat);



uint8_t _readAttr_OnOff(uint8_t ep);
uint16_t _readAttr_OnTime(uint8_t ep);
uint8_t _readAttr_TUYAIndicator(uint8_t ep);
uint16_t _readAttr_OffWaitTime(uint8_t ep);
uint8_t _readAttr_Level(uint8_t ep);
uint8_t _readAttr_startupLevel(uint8_t ep);
uint8_t _readAttr_onLevel(uint8_t ep);
uint8_t _readMfgAttr_Indicator(uint8_t ep);
//uint8_t _readMfgAttr_prMax(uint8_t ep);
//uint8_t _readMfgAttr_prMin(uint8_t ep);
//uint8_t _readMfgAttrQtSens(uint8_t ep);
//uint8_t _readMfgAttr_dimSpeed(uint8_t ep);
//uint8_t _readMfgAttr_dimPressTim(uint8_t ep);
// carel_add
//uint8_t _readMfgAttr_FactoryTest(uint8_t ep);
// 20190218
uint8_t _readAttr_OTAStatus(uint8_t ep);

uint8_t _getLevelMax(uint8_t ep);
uint8_t _getLevelMin(uint8_t ep);

uint8_t _readAttr_LedsPosition(uint8_t ep);


#endif /* CARELSDK_Z3_EXTEND_EXT_CLUSTER_H_ */
