/*=======================================================================*\
* ------------------------------------------------------------------------|
* 文件		：_carelSDK/Z3Extend/Ext_GroupScene.c
* 描述		：通用组、场景操作相关API扩展实现
* 创建日期		：2018/12
* 设计者		：Carel.Li
* 修改日期		：
* 修改内容		：
* 版本		：v1.0
*-------------------------------------------------------------------------|
\*=======================================================================*/
#include "../common.h"
#include "plugin/scenes/scenes.h"

// 获取场景表中匹配端点的索引
uint8_t getSceneInfoFromEp(uint8_t ep){

	EmberAfSceneTableEntry entry;
	for (uint8_t i = 0; i < EMBER_AF_PLUGIN_SCENES_TABLE_SIZE; i++) {
		emberAfPluginScenesServerRetrieveSceneEntry(entry, i);
		if (entry.endpoint == ep) {
			return i;
		}
	}
	return 0xFF;
}
