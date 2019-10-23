/*=======================================================================*\
* ------------------------------------------------------------------------|
* 文件		：_carelSDK/Z3Extend/Ext_Report.c
* 描述		：通用reporting操作相关API扩展实现
* 创建日期		：2018/12
* 设计者		：Carel.Li
* 修改日期		：
* 修改内容		：
* 版本		：v1.0
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
// report标准属性至指定节点的端点
// c_s.属性类别，0-client，1-server
void _reportStdAttribute(uint16_t dstNodeId, uint8_t srcEp, uint8_t dstEp, uint16_t clusterId, uint16_t attrId, uint8_t c_s){

	uint8_t attrData[ATTRIBUTE_LARGEST];
	EmberAfAttributeType attrType;


	// 入网检查
	if(emberAfNetworkState() != EMBER_JOINED_NETWORK)
		return;

	// 读取属性值
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
	// 组装ZCL帧
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



// report mfg属性至指定节点的端点
// c_s.属性类别，0-client，1-server
void _reportMfgServerAttribute(uint16_t dstNodeId, uint8_t srcEp, uint8_t dstEp, uint16_t clusterId, uint16_t attrId){

	uint8_t attrData[ATTRIBUTE_LARGEST];
	EmberAfAttributeType attrType;


	// 入网检查
	if(emberAfNetworkState() != EMBER_JOINED_NETWORK)
		return;

	// 读取属性值
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
	// 组装ZCL帧
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




