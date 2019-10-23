#include "../common.h"


LIST(_tglList);

static void _ctlIndicatorSyncCh(uint8_t ch, uint8_t newStatus);
static void _ctlToggle_Add(uint8_t ch, uint8_t newStatus);



void emberAfOnOffClusterServerAttributeChangedCallback(int8u endpoint,
                                                       EmberAfAttributeId attributeId){
	if(startup)
		return;

//	DEBUG("__onoff attr cng: %2X", attributeId);

	// �ϱ�
	if(attributeId == 0x4001){
		// ontime ÿ��60s�ϱ�(ֱ���ϱ�)
		uint16_t ontime = _readAttr_OnTime(endpoint);
		if((ontime % 60) == 0){
//			_SEQ_Report_Add(endpoint, 0x0006, attributeId, false);
			_reportStdAttribute(0, endpoint, 1, 0x0006, attributeId, 1);
		}
	}
	else if(attributeId == 0x4002){
		// offwaittime ���ϱ�
	}
	else{
		_SEQ_Report_Add(endpoint, 0x0006, attributeId, false);
	}

	// �����ϱ�����
	_perReportCmd(_PERPRIOD_NO_ONOFF, _PerReport_Cmd_RefleshPeriod);


	if(attributeId == 0x0000){
		// ��ֵ
		uint8_t buf_onoff = _readAttr_OnOff(endpoint)?1:0;
		// ���д�������
		_ctlToggle_Add(endpoint, buf_onoff);
		// ������ʱ
		if(simonExtendCmd_Timer_GetEnable(endpoint)){
			if((uint8_t)simonExtendCmd_Timer_GetGoalState(endpoint) == buf_onoff){
				simonExtendCmd_Timer_CTL(0, 0, endpoint);
			}
		}
	}
	else if(attributeId == 0x8001){
		_ctlIndicatorSyncAll();
	}
}


// ���ش������������¼�
EmberEventControl _toggleHandler_EC;
void _toggleHandler_EF(void){
	emberEventControlSetInactive(_toggleHandler_EC);

	_toggle_t *item = list_pop(_tglList);

	if(item == NULL){
		return;
	}
	else{
		DEBUG("_toggle: ch %d, newStatus: %d", item->ch, item->cmd?1:0);

		// ����ͬ��
		_DrvCtlLight(item->ch, item->cmd?1:0);

		// ָʾ��ͬ��
		_ctlIndicatorSyncCh(item->ch, item->cmd?1:0);

		free(item);
	}


	emberEventControlSetDelayMS(_toggleHandler_EC, _TOGGLE_INTERVAL);
}


// ��ʼ��
void _ctlToggle_Init(void){
	list_init(_tglList);
}


// �������
static void _ctlToggle_Add(uint8_t ch, uint8_t newStatus){

	if(!ch)
		return;

	if(ch > CHANNEL_NUM)
		return;

	// ɾ��ͬ��·�ɿ��Ƶ�Ԫ(��֤���ߴ粻����CHANNEL_NUM)
	for(_toggle_t *item = *_tglList; item != NULL; item = item->next){
		if(item->ch == ch){
			list_remove(_tglList, item);
			free(item);
			break;
		}
	}

	// ��ӽ���
	_toggle_t *item = malloc(sizeof(_toggle_t));
	item->ch = ch;
	item->cmd = newStatus?1:0;
	list_add(_tglList, item);

	// ���������¼�
	if(!emberEventControlGetActive(_toggleHandler_EC)){
		emberEventControlSetDelayMS(_toggleHandler_EC, _TOGGLE_INTERVAL);
	}
}

static void _ctlIndicatorSyncCh(uint8_t ch, uint8_t newStatus){
	if(ch == 0 || ch > CHANNEL_NUM)
		return;

	// ��ģʽ
	uint8_t _bufMode;
	if(emberAfReadServerAttribute(ch,
								 0x0006,
								 0x8001,
								 &_bufMode,
								 1) != EMBER_ZCL_STATUS_SUCCESS){
		return;
	}

	// ״ָ̬ʾ
	if(_bufMode == 1){
		gLed_SetStillState(_epListSw_GetByEp(ch)->ledNo, newStatus?1:0);
	}
	// λ��ָʾ
	else if(_bufMode == 2){
		gLed_SetStillState(_epListSw_GetByEp(ch)->ledNo, newStatus?0:1);
	}
	// ����
	else if(_bufMode == 0){
		gLed_SetStillState(_epListSw_GetByEp(ch)->ledNo, 0);
	}
	else{
		return;
	}
}

void _ctlIndicatorSyncAll(void){
	// ��ģʽ
	uint8_t _bufMode;
	if(emberAfReadServerAttribute(1,
								 0x0006,
								 0x8001,
								 &_bufMode,
								 1) != EMBER_ZCL_STATUS_SUCCESS){
		return;
	}

	for(uint8_t i=1; i<=CHANNEL_NUM; i++){
		uint8_t buf_onoff = _readAttr_OnOff(i)?1:0;
		if(_bufMode == 2){
			gLed_SetStillState(_epListSw_GetByEp(i)->ledNo, buf_onoff?0:1);
		}
		else if(_bufMode == 0){
			gLed_SetStillState(_epListSw_GetByEp(i)->ledNo, 0);
		}
		else if(_bufMode == 1){
			gLed_SetStillState(_epListSw_GetByEp(i)->ledNo, buf_onoff?1:0);
		}
	}
}

// <ontimedoff>������Զ���ʵ��
bool _carel_emberAfOnOffClusterOnWithTimedOffCallback(uint8_t onOffControl,
                                               uint16_t onTime,
                                               uint16_t offWaitTime){
	EmberAfStatus status;
	uint16_t onTimeAttribute;//, offWaitTimeAttribute;
	uint8_t onOffAttribute;
	uint8_t endpoint = emberAfCurrentEndpoint();

	// The valid range of the OnTime and OffWaitTime fields is 0x0000 to 0xFFFF.
	if (onTime == 0xFFFF || offWaitTime == 0xFFFF) {
		status = EMBER_ZCL_STATUS_INVALID_FIELD;
		goto kickout;
	}


	if((onOffAttribute = _readAttr_OnOff(endpoint)) == 0xFF){
		status = EMBER_ZCL_STATUS_FAILURE;
		goto kickout;
	}

	onTimeAttribute = onTime;
	if((status = emberAfWriteServerAttribute(endpoint, \
							 0x0006, \
							 0x4001, \
							 (uint8_t *)&onTimeAttribute,
							 0x21)) != EMBER_ZCL_STATUS_SUCCESS){
		goto kickout;
	}

	(void)offWaitTime;
#if 0
	offWaitTimeAttribute = offWaitTime;
	if((status = emberAfWriteServerAttribute(endpoint, \
							 0x0006, \
							 0x4002, \
							 (uint8_t *)&offWaitTimeAttribute,
							 0x21)) != EMBER_ZCL_STATUS_SUCCESS){
		goto kickout;
	}
#endif
	// ts
	DEBUG("ep:%d ___________________crttent onoff: %d",endpoint, onOffAttribute);
	//

	simonExtendCmd_Timer_CTL(onOffAttribute?0:1, onTime, endpoint);

	// ȷ��ontime�ϱ�
	if(((onTime % 60) != 0) && (onTime != 0)){
		_reportStdAttribute(0x0000, endpoint, 1, 0x0006, 0x4001, 1);
	}

kickout:
	emberAfSendImmediateDefaultResponse(status);
	return true;
}

