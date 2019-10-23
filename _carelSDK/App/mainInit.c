/*=======================================================================*\
* ------------------------------------------------------------------------|
* �ļ�		��_carelSDK/Project/DimSw_CH1_2/mainInit.c
* ����		���ض���Dimmer Switch ���ϵ�Callbackʵ�֣�����������ʼ��������Task����д���桢
* 			    ʹ�������
* ��������		��2018/11/15
* �����		��Carel.Li
* �޸�����		��2019/1
* �޸�����		���ܹ���д
* �汾		��v1.1
*-------------------------------------------------------------------------|
\*=======================================================================*/
#include "../common.h"

EmberEventControl mainInit_EventControl;



/**************************************************************************|
 * ������	_drvInit_SDK
 * ���ͣ�	-
 * ���ܣ�	Ӳ����ʼ��
 * ���ڣ�	-
 * ������	-
 * ���أ�	-
 ***/

void _drvInit_SDK(void){
	_DrvInitKeyLed();
	_DrvInitLight();
}


/**************************************************************************|
 * ������	mainInit_Start
 * ���ͣ�	-
 * ���ܣ�	Led&Key�����ʼ�������Led����Key����Endpoint
 * ���ڣ�	-
 * ������	-
 * ���أ�	-
 ***/
void mainInit_Start(void){

	// WDR
	halResetWatchdog();

	// ����EndPoint����
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

	// ����Led & key����
	startApp_LedBlink(0);
	startApp_Keyscan(0);

	// ����LED
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
	// ����KEY
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

	// ����EP
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

	// LEDȫ��
	for(uint8_t i=1; i<=CHANNEL_NUM; i++){
		gLed_SetStillState(_epListSw_GetByEp(i)->ledNo, 1);
	}

	// ��ʼ��report����
	_SEQ_Report_Init();

	// ������ʱ������
	emberAfScheduleServerTick(1,
						ZCL_ON_OFF_CLUSTER_ID,
						MILLISECOND_TICKS_PER_SECOND);

	// ��ʱ1s����main init
	emberEventControlSetDelayMS(mainInit_EventControl,1000);

	// WDR
	halResetWatchdog();
}


/**************************************************************************|
 * ������	mainInit_EventFunction
 * ���ͣ�	Task
 * ���ܣ�	��ȡTokens & д��Cluster & ˢ�¿��� & ��������reporting
 * ���ڣ�	-
 * ������	-
 * ���أ�	-
 ***/
void mainInit_EventFunction(void){
	emberEventControlSetInactive(mainInit_EventControl);

	// ��ʼ��Toggle���е�Ԫ
	_ctlToggle_Init();


	// ָʾ��ͬ��
	_ctlIndicatorSyncAll();

	// ����ͬ��
	uint8_t _outMask = 0;
	for(uint8_t i=1; i<=CHANNEL_NUM; i++){
//		_writeAttr_OnOff(i, _readAttr_OnOff(i));
//		_DrvWr_HC595(i, _readAttr_OnOff(i));
		_outMask |= (_readAttr_OnOff(i)?1:0) << i;
	}
	_DrvCtlLightMask(_outMask);

	// ���������ϱ��¼�
	_periodicReportInit();
	_perReportItem_Add(_PERPRIOD_NO_ONOFF, 0x0006, 0x0000, false, 5*60*100, false);
	_perReportItem_Add(_PERPRIOD_NO_INDICATOR, 0x0006, 0x8001, false, 5*60*100, true);
	_perReportItem_Add(_PERPRIOD_NO_INDICATOR, 0x0006, 0x4001, false, 5*60*100, false); // �����⴦��
	_perReportCmd(0, _PerReport_Cmd_ShortTemp); // ��ʱ�������ϱ�

	// �������
#if HD_ENABLE
	_HD_Heartbeat_Start();
#endif

	// ������־
	startup = false;

	DEBUG("Init Done!\n");
}











