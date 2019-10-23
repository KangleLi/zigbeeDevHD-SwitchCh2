/*=======================================================================*\
* ------------------------------------------------------------------------|
* �ļ�		��_carelSDK/Z3Extend/Ext_Bind.c
* ����		��ͨ��Binding���API��չʵ��
* ��������		��2018/12
* �����		��Carel.Li
* �޸�����		��
* �޸�����		��
* �汾		��v1.0
*-------------------------------------------------------------------------|
\*=======================================================================*/
#include "../common.h"


uint8_t getBindingInfoFromEp(uint8_t ep, uint16_t clusterId){

	EmberBindingTableEntry etyBinding;

	for(uint8_t i = 0;i < EMBER_BINDING_TABLE_SIZE;i ++){
		// ��ȡʧ��
		if(emberGetBinding(i,&etyBinding) != EMBER_SUCCESS){
			return 0xFF;
		}
		// ��ȡbinding��Ŀ
		if((etyBinding.local == ep) &&
		(etyBinding.type == EMBER_UNICAST_BINDING) &&
		(etyBinding.clusterId == clusterId)){
			return i;
		}
	}
	// δ������ƥ����Ŀ
	return 0xFF;
}

