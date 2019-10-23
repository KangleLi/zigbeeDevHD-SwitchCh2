/*=======================================================================*\
* ------------------------------------------------------------------------|
* �ļ�		��_carelSDK/Z3Extend/Ext_Report.c
* ����		��ͨ��reporting�������API��չʵ��
* ��������		��2018/12
* �����		��Carel.Li
* �޸�����		��
* �޸�����		��
* �汾		��v1.0
*-------------------------------------------------------------------------|
\*=======================================================================*/
#include "../common.h"



extern uint8_t appZclBuffer[];
extern uint16_t appZclBufferLen;
extern EmberApsFrame globalApsFrame;
extern uint8_t disableDefaultResponse;
extern uint16_t mfgSpecificId;
extern bool zclCmdIsBuilt;
extern uint8_t disableDefaultResponse;
extern void zclBufferSetup(uint8_t frameType, uint16_t clusterId, uint8_t commandId);
extern void zclBufferAddWord(uint16_t word);
extern void zclBufferAddByte(uint8_t byte);
extern void emAfApsFrameEndpointSetup(uint8_t srcEndpoint, uint8_t dstEndpoint);
// report��׼������ָ���ڵ�Ķ˵�
// c_s.�������0-client��1-server
void _reportStdAttribute(uint16_t dstNodeId, uint8_t srcEp, uint8_t dstEp, uint16_t clusterId, uint16_t attrId, uint8_t c_s){

	uint8_t attrData[ATTRIBUTE_LARGEST];
	EmberAfAttributeType attrType;


	// �������
	if(emberAfNetworkState() != EMBER_JOINED_NETWORK)
		return;

	// ��ȡ����ֵ
	if(emberAfReadAttribute(srcEp,
						 clusterId,
						 attrId,
						 (c_s==0?CLUSTER_MASK_CLIENT:CLUSTER_MASK_SERVER),
						 attrData,
						 sizeof(attrData),
						 &attrType) != EMBER_ZCL_STATUS_SUCCESS){
		return;
	}

	//
	// ��װZCL֡
	//
	// Frame Control Field
	mfgSpecificId = EMBER_AF_NULL_MANUFACTURER_CODE;
	zclCmdIsBuilt = false; // ??
	disableDefaultResponse = 0;

	zclBufferSetup(ZCL_GLOBAL_COMMAND | \
				   (c_s==0?ZCL_FRAME_CONTROL_CLIENT_TO_SERVER:ZCL_FRAME_CONTROL_SERVER_TO_CLIENT), \
				   clusterId, \
				   ZCL_REPORT_ATTRIBUTES_COMMAND_ID);
	// Frame Payload
	zclBufferAddWord(attrId);
	zclBufferAddByte(attrType);

	uint16_t dataLen = emberAfIsThisDataTypeAStringType(attrType)?\
					   emberAfStringLength(attrData): \
					   emberAfGetDataSize(attrType);
	MEMCOPY(appZclBuffer + appZclBufferLen, attrData, dataLen);
	appZclBufferLen += dataLen;

	// unicast
	emAfApsFrameEndpointSetup(srcEp, dstEp);

	emberAfSendUnicast(EMBER_OUTGOING_DIRECT,
					   dstNodeId,
	                   &globalApsFrame,
					   appZclBufferLen,
					   appZclBuffer);
}



// report mfg������ָ���ڵ�Ķ˵�
// c_s.�������0-client��1-server
void _reportMfgServerAttribute(uint16_t dstNodeId, uint8_t srcEp, uint8_t dstEp, uint16_t clusterId, uint16_t attrId){

	uint8_t attrData[ATTRIBUTE_LARGEST];
	EmberAfAttributeType attrType;


	// �������
	if(emberAfNetworkState() != EMBER_JOINED_NETWORK)
		return;

	// ��ȡ����ֵ
	extern EmberAfStatus emAfReadAttribute(uint8_t endpoint,
            EmberAfClusterId cluster,
            EmberAfAttributeId attributeID,
            uint8_t mask,
            uint16_t manufacturerCode,
            uint8_t *dataPtr,
            uint16_t readLength,
            EmberAfAttributeType *dataType);
	if(emAfReadAttribute(srcEp,
			clusterId,
			attrId,
	        CLUSTER_MASK_SERVER,
			EMBER_AF_MANUFACTURER_CODE,
			attrData,
			sizeof(attrData),
	        &attrType) != EMBER_ZCL_STATUS_SUCCESS){
		return;
	}

	//
	// ��װZCL֡
	//
	// Frame Control Field
	mfgSpecificId = EMBER_AF_MANUFACTURER_CODE;
	zclCmdIsBuilt = false; // ??
	disableDefaultResponse = 0;

	zclBufferSetup(ZCL_GLOBAL_COMMAND | \
				   ZCL_FRAME_CONTROL_SERVER_TO_CLIENT, \
				   clusterId, \
				   ZCL_REPORT_ATTRIBUTES_COMMAND_ID);

	// Frame Payload
	zclBufferAddWord(attrId);
	zclBufferAddByte(attrType);

	uint16_t dataLen = emberAfIsThisDataTypeAStringType(attrType)?\
					   emberAfStringLength(attrData): \
					   emberAfGetDataSize(attrType);
	MEMCOPY(appZclBuffer + appZclBufferLen, attrData, dataLen);
	appZclBufferLen += dataLen;

	// unicast
	emAfApsFrameEndpointSetup(srcEp, dstEp);

	emberAfSendUnicast(EMBER_OUTGOING_DIRECT,
					   dstNodeId,
	                   &globalApsFrame,
					   appZclBufferLen,
					   appZclBuffer);
}




