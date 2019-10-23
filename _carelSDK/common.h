
#ifndef CARELSDK_COMMON_H_
#define CARELSDK_COMMON_H_

//////////////////////////////////////////////////////////////////////////
// Includes
//////////////////////////////////////////////////////////////////////////
#include "stdlib.h"
#include "./CLib/List/list.h"
#include "app/framework/include/af.h"

#include "./Z3_Extend/Ext_Bind.h"
#include "./Z3_Extend/Ext_GroupScene.h"
#include "./Z3_Extend/Ext_Cluster.h"
#include "./Z3_Extend/Ext_Report.h"
#include "./Z3_Extend/Ext_zcmd.h"
#include "./Z3_Extend/announce.h"

#include "seqReport.h"

#include "./GeneralApp/led_blink.h"
#include "./GeneralApp/key_scan.h"

#include "./App/networkHandle.h"
#include "./App/onoff.h"
#include "./App/periodicReport.h"
#include "./App/HD_Port.h"

//#include "./Drv/HC595.h"
#include "./Drv/MLR.h"
#include "./Drv/keyLed.h"

#include "./simonExtendCmd.h"

//////////////////////////////////////////////////////////////////////////
// Macros
//////////////////////////////////////////////////////////////////////////
//#define DEBUG(...)			{ \
//	emberAfDebugPrint("## F[%s],L[%d] ##\n", __FILE__, __LINE__); \
//	emberAfDebugPrintln(__VA_ARGS__); \
//}

#define DEBUG			emberAfDebugPrintln

//////////////////////////////////////////////////////////////////////////
// 位域结构体
//////////////////////////////////////////////////////////////////////////
typedef struct{
	uint8_t bit_0:1;
	uint8_t bit_1:1;
	uint8_t bit_2:1;
	uint8_t bit_3:1;
	uint8_t bit_4:1;
	uint8_t bit_5:1;
	uint8_t bit_6:1;
	uint8_t bit_7:1;
}bits8_t;

typedef union{
	uint8_t type_uint8;
	bits8_t type_bits8;
}mulbits8_t;

uint8_t _GET_BIT_FROMBITS8(uint8_t index, mulbits8_t *data);				// 位域结构体 位取值
void _SET_BIT_FROMBITS8(uint8_t index, mulbits8_t *data, uint8_t bit);		// 位域结构体 位赋值


//////////////////////////////////////////////////////////////////////////
// EndPoints
//////////////////////////////////////////////////////////////////////////

typedef struct ENDPOINT {
	struct ENDPOINT *next;
	uint8_t endPoint;
	void *data;
} endPoint_t;

void _epList_Init(void);
void addList_EP(uint8_t ep, void *data);									// 添加Endpoint对象
endPoint_t *_getEndpointIterm(uint8_t ep);									// 获取Endpoint对象指针
uint8_t _getEndpointCount(void);											// 获取Endpoint对象数量
void _removeEndpointIterm(endPoint_t *iterm);								// 删除Endpoint对象

extern bool startup;

//////////////////////////////////////////////////////////////////////////
// EndPoints --- Switch
//////////////////////////////////////////////////////////////////////////
#include "./App/config.h"



typedef struct _EPLISTSWDATA{
	uint8_t ledNo;
	uint8_t keyNo;

}_epListSwData_t;



void _epListSw_Add(uint8_t ep, _epListSwData_t *dat);
_epListSwData_t *_epListSw_GetByEp(uint8_t ep);

#endif /* CARELSDK_COMMON_H_ */
