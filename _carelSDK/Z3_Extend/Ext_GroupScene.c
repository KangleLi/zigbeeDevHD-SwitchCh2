/*=======================================================================*\
* ------------------------------------------------------------------------|
* �ļ�		��_carelSDK/Z3Extend/Ext_GroupScene.c
* ����		��ͨ���顢�����������API��չʵ��
* ��������		��2018/12
* �����		��Carel.Li
* �޸�����		��
* �޸�����		��
* �汾		��v1.0
*-------------------------------------------------------------------------|
\*=======================================================================*/
#include "../common.h"
#include "plugin/scenes/scenes.h"

// ��ȡ��������ƥ��˵������
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
