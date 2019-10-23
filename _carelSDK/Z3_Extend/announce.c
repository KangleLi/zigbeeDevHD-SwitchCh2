#include "../common.h"



uint8_t _zdp_buffer_for_announce[12];

extern EmberApsFrame globalApsFrame;

extern EmberStatus _send_plus(EmberOutgoingMessageType type,
        uint16_t indexOrDestination,
        EmberApsFrame *apsFrame,
        uint16_t messageLength,
        uint8_t *message,
        bool broadcast,
        EmberNodeId alias,
        uint8_t sequence);

int8_t _carelDeviceAnnounce(void){

	if(emberAfNetworkState() != EMBER_JOINED_NETWORK){
		return -1;
	}

	globalApsFrame.clusterId = _ANNOUNCE_CLUSTER_ID;
	globalApsFrame.destinationEndpoint = _ANNOUNCE_DESTINATION_EP;
	globalApsFrame.options = EMBER_APS_OPTION_SOURCE_EUI64;
	globalApsFrame.profileId = _ANNOUNCE_PROFILE_ID;
	globalApsFrame.sourceEndpoint = _ANNOUNCE_SOURCE_EP;

	uint8_t * _ptr_ = _zdp_buffer_for_announce;
	// [1]ZDP Transaction Sequence Number
	*(_ptr_++) = _ANNOUNCE_SEQNUM;
	// [2]NWK Address of Local Device
	uint16_t _nodeId = emberAfGetNodeId();
	*(_ptr_++) = (uint8_t)(_nodeId & 0xFF);
	*(_ptr_++) = (uint8_t)((_nodeId >> 8) & 0xFF);
	// [8]IEEE of Local Device
	EmberEUI64 _eui64_;
	emberAfGetEui64(_eui64_);
	MEMCOPY(_ptr_, _eui64_, 8);
	_ptr_ += 8;
	// [1]Capability Information
	*_ptr_ = _ANNOUNCE_CAPAINFO;

	EmberStatus ret = _send_plus(EMBER_OUTGOING_BROADCAST,
			_ANNOUNCE_DESTINATION_NODEID,
	        &globalApsFrame,
	        12,
			_zdp_buffer_for_announce,
	        true,
	        0,
	        0);
	if(EMBER_SUCCESS != ret){
		DEBUG("___errcode: %d", ret);
		return -2;
	}
	return 0;
}




























