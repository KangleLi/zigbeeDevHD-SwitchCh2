/*=======================================================================*\
* ------------------------------------------------------------------------|
* 文件		：_carelSDK/Project/DimSw_CH1_2/mainInit.c
* 描述		：特定于Dimmer Switch 的上电Callback实现，包括驱动初始化、启动Task、读写闪存、
* 			    使能外设等
* 创建日期		：2018/11/15
* 设计者		：Carel.Li
* 修改日期		：2019/1
* 修改内容		：架构重写
* 版本		：v1.1
*-------------------------------------------------------------------------|
\*=======================================================================*/
#include "../common.h"

EmberEventControl mainInit_EventControl;



/**************************************************************************|
 * 函数：	_drvInit_SDK
 * 类型：	-
 * 功能：	硬件初始化
 * 周期：	-
 * 参数：	-
 * 返回：	-
 ***/

void _drvInit_SDK(void){
	_DrvInitKeyLed();
	_DrvInitLight();
}


/**************************************************************************|
 * 函数：	mainInit_Start
 * 类型：	-
 * 功能：	Led&Key任务初始化，添加Led对象、Key对象、Endpoint
 * 周期：	-
 * 参数：	-
 * 返回：	-
 ***/
void mainInit_Start(void){

	// WDR
	halResetWatchdog();

	// 无用EndPoint屏蔽
#if (CHANNEL_NUM < 4)
	emberAfEndpointEnableDisable(4, false);
#if (CHANNEL_NUM < 3)
	emberAfEndpointEnableDisable(3, false);
#if (CHANNEL_NUM < 2)
	emberAfEndpointEnableDisable(2, false);
#endif
#endif
#endif


	_epList_Init();

	// 启动Led & key任务
	startApp_LedBlink(0);
	startApp_Keyscan(0);

	// 创建LED
	gLed_Add(1, LED1_Handler, 0);
#if (CHANNEL_NUM > 1)
	gLed_Add(2, LED2_Handler, 0);
#if (CHANNEL_NUM > 2)
	gLed_Add(3, LED3_Handler, 0);
#if (CHANNEL_NUM > 3)
	gLed_Add(4, LED4_Handler, 0);
#endif
#endif
#endif
	// 创建KEY
	gKey_Add(1, KEY1_Get, KEY1_pdCB, KEY1_rdCB, KEY1_ppCB);
#if (CHANNEL_NUM > 1)
	gKey_Add(2, KEY2_Get, KEY2_pdCB, KEY2_rdCB, KEY2_ppCB);
#if (CHANNEL_NUM > 2)
	gKey_Add(3, KEY3_Get, KEY3_pdCB, KEY3_rdCB, KEY3_ppCB);
#if (CHANNEL_NUM > 3)
	gKey_Add(4, KEY4_Get, KEY4_pdCB, KEY4_rdCB, KEY4_ppCB);
#endif
#endif
#endif

	// 创建EP
	_epListSwData_t _epItem1 = {1, 1}; _epListSw_Add(1, &_epItem1);
#if (CHANNEL_NUM > 1)
	_epListSwData_t _epItem2 = {2, 2}; _epListSw_Add(2, &_epItem2);
#if (CHANNEL_NUM > 2)
	_epListSwData_t _epItem3 = {3, 3}; _epListSw_Add(3, &_epItem3);
#if (CHANNEL_NUM > 3)
	_epListSwData_t _epItem4 = {4, 4}; _epListSw_Add(4, &_epItem4);
#endif
#endif
#endif

	// LED全亮
	for(uint8_t i=1; i<=CHANNEL_NUM; i++){
		gLed_SetStillState(_epListSw_GetByEp(i)->ledNo, 1);
	}

	// 初始化report序列
	_SEQ_Report_Init();

	// 启动定时调度器
	emberAfScheduleServerTick(1,
						ZCL_ON_OFF_CLUSTER_ID,
						MILLISECOND_TICKS_PER_SECOND);

	// 延时1s启动main init
	emberEventControlSetDelayMS(mainInit_EventControl,1000);

	// WDR
	halResetWatchdog();
}


/**************************************************************************|
 * 函数：	mainInit_EventFunction
 * 类型：	Task
 * 功能：	获取Tokens & 写入Cluster & 刷新控制 & 启动周期reporting
 * 周期：	-
 * 参数：	-
 * 返回：	-
 ***/
void mainInit_EventFunction(void){
	emberEventControlSetInactive(mainInit_EventControl);

	// 初始化Toggle序列单元
	_ctlToggle_Init();


	// 指示灯同步
	_ctlIndicatorSyncAll();

	// 驱动同步
	uint8_t _outMask = 0;
	for(uint8_t i=1; i<=CHANNEL_NUM; i++){
//		_writeAttr_OnOff(i, _readAttr_OnOff(i));
//		_DrvWr_HC595(i, _readAttr_OnOff(i));
		_outMask |= (_readAttr_OnOff(i)?1:0) << i;
	}
	_DrvCtlLightMask(_outMask);

	// 启动周期上报事件
	_periodicReportInit();
	_perReportItem_Add(_PERPRIOD_NO_ONOFF, 0x0006, 0x0000, false, 5*60*100, false);
	_perReportItem_Add(_PERPRIOD_NO_INDICATOR, 0x0006, 0x8001, false, 5*60*100, true);
	_perReportItem_Add(_PERPRIOD_NO_INDICATOR, 0x0006, 0x4001, false, 5*60*100, false); // 有特殊处理
	_perReportCmd(0, _PerReport_Cmd_ShortTemp); // 短时快周期上报

	// 恒大心跳
#if HD_ENABLE
	_HD_Heartbeat_Start();
#endif

	// 启动标志
	startup = false;

	DEBUG("Init Done!\n");
}











