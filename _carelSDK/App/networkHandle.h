
#ifndef CARELSDK_PROJECT_DIMSW_CH1_2_NETWORKHANDLE_H_
#define CARELSDK_PROJECT_DIMSW_CH1_2_NETWORKHANDLE_H_


#define _LIMIT_STEERING_NWK				(1000*60*3-1000*15)	// 入网限制时间
#define	NETLED							1

extern bool _ifClearSceneBindGroup;

void startCommissioning(uint32_t delay);
void startFindingAndBinding(uint32_t delay);
void clearNWK(void);



#endif /* CARELSDK_PROJECT_DIMSW_CH1_2_NETWORKHANDLE_H_ */
