#include "./common.h"

void _zcmd_ReadResponse(uint8_t seqNum, uint8_t isMfg, uint16_t cId, \
		uint16_t aId, uint8_t sEp, uint8_t dEp, uint16_t dNid){
	// Clear out the response buffer by setting its length to zero
	appResponseLength = 0;
	uint8_t frameControl;

//	emAfApsFrameClusterIdSetup(cId);
	emberAfResponseApsFrame.clusterId = cId;
	emberAfResponseApsFrame.options = EMBER_AF_DEFAULT_APS_OPTIONS;

	// Make the ZCL header for the response
	// note: cmd byte is set below
	frameControl = (ZCL_GLOBAL_COMMAND
				  | (ZCL_FRAME_CONTROL_SERVER_TO_CLIENT
					 | EMBER_AF_DEFAULT_RESPONSE_POLICY_RESPONSES));

	if(isMfg){
		frameControl |= ZCL_MANUFACTURER_SPECIFIC_MASK;
	}

	emberAfPutInt8uInResp(frameControl);

	if(isMfg){
		emberAfPutInt16uInResp(EMBER_AF_MANUFACTURER_CODE);
	}

	emberAfPutInt8uInResp(seqNum);

	// ---- common ---- ¡ü¡ü¡ü¡ü¡ü¡ü¡ü¡ü¡ü¡ü¡ü¡ü¡ü¡ü¡ü¡ü¡ü¡ü


	emberAfPutInt8uInResp(ZCL_READ_ATTRIBUTES_RESPONSE_COMMAND_ID);

	emberAfRetrieveAttributeAndCraftResponse(sEp, cId, aId, CLUSTER_MASK_SERVER, isMfg?EMBER_AF_MANUFACTURER_CODE:0,
	                                                   (EMBER_AF_RESPONSE_BUFFER_LEN
	                                                    - appResponseLength));

	// unicast
//	emAfApsFrameEndpointSetup(sEp, dEp);
	emberAfResponseApsFrame.sourceEndpoint = sEp;
	emberAfResponseApsFrame.destinationEndpoint = dEp;

	emberAfSendUnicast(EMBER_OUTGOING_DIRECT,
					   dNid,
					   &emberAfResponseApsFrame,
					   appResponseLength,
					   appResponseData);
}



