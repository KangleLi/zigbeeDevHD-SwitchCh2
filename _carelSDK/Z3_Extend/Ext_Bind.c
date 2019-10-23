/*=======================================================================*\
* ------------------------------------------------------------------------|
* 文件		：_carelSDK/Z3Extend/Ext_Bind.c
* 描述		：通用Binding相关API扩展实现
* 创建日期		：2018/12
* 设计者		：Carel.Li
* 修改日期		：
* 修改内容		：
* 版本		：v1.0
*-------------------------------------------------------------------------|
\*=======================================================================*/
#include "../common.h"


uint8_t getBindingInfoFromEp(uint8_t ep, uint16_t clusterId){

	EmberBindingTableEntry etyBinding;

	for(uint8_t i = 0;i < EMBER_BINDING_TABLE_SIZE;i ++){
		// 获取失败
		if(emberGetBinding(i,&etyBinding) != EMBER_SUCCESS){
			return 0xFF;
		}
		// 获取binding条目
		if((etyBinding.local == ep) &&
		(etyBinding.type == EMBER_UNICAST_BINDING) &&
		(etyBinding.clusterId == clusterId)){
			return i;
		}
	}
	// 未检索到匹配条目
	return 0xFF;
}

