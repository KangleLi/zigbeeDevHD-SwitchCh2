#include "../common.h"



EmberEventControl _HD_HeartBeat_EC;
void _HD_HeartBeat_EF(void){
	emberEventControlSetInactive(_HD_HeartBeat_EC);

	_zcmd_ReadResponse(254, 0, 0x0000, 0x0005, 1, 1, 0x0000);
	DEBUG("__HD HeartBeat !");


	emberEventControlSetDelayMS(_HD_HeartBeat_EC, _HD_HEARTBEAT_PERIOD);

}

void _HD_Heartbeat_Start(void){
	emberEventControlSetDelayMS(_HD_HeartBeat_EC, _HD_HEARTBEAT_PERIOD);
}
