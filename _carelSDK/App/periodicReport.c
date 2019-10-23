#include "../common.h"


LIST(_perReportList);

static _perReport_Cmd_t _perReport_Cmd = 0;
static uint8_t _perReport_ActNo = 0;

static uint32_t _shortCnt = 0;
static uint32_t _shortTotal = 0;

// period:100ms
EmberEventControl _periodicReport_EC;
void _periodicReport_EF(void){
	emberEventControlSetInactive(_periodicReport_EC);

	static uint8_t _seq_ = 0;

	// TS
	static uint8_t _befSeq = 0;
	if(_befSeq != _seq_){
		DEBUG("_perReport: %d", _befSeq=_seq_);
	}
	if(_perReport_Cmd){
		DEBUG("_perReport Cmd: %d", _perReport_Cmd);
	}
	//

	// 重置周期
	if(_perReport_Cmd == _PerReport_Cmd_RefleshPeriod){
		for(_perReport_t *item = *_perReportList; item != NULL; item = item->next){
			if(item->no == _perReport_ActNo){
				item->cnt = item->period;
				break;
			}
		}
	}

	switch(_seq_){
	// 等待启动
	case 0:
		// Evt: ENABLE
		if(_perReport_Cmd == _PerReport_Cmd_Enable){
			_seq_++;
			break;
		}
		// Evt: SHORT
		else if(_perReport_Cmd == _PerReport_Cmd_ShortTemp){
			_seq_ = 2;
			_shortTotal = 0;
			_shortCnt = 0;
			break;
		}
		break;

	// 常规上报
	case 1:
		// Evt: DISABLE
		if(_perReport_Cmd == _PerReport_Cmd_Disable){
			_seq_--;
			break;
		}
		// Evt: SHORT
		else if(_perReport_Cmd == _PerReport_Cmd_ShortTemp){
			_seq_ = 2;
			_shortTotal = 0;
			_shortCnt = 0;
			break;
		}

		for(_perReport_t *item = *_perReportList; item != NULL; item = item->next){
			if(item->cnt > 1){
				item->cnt --;
			}
			else{
				DEBUG("_per long report!");
				item->cnt = item->period;
				// report
				if(item->oneEp){
					_SEQ_Report_Add(1, item->clusterId, item->attributeId, item->isMfg);
				}
				else{
					for(uint8_t ep = 1; ep <= CHANNEL_NUM; ep++){
						_SEQ_Report_Add(ep, item->clusterId, item->attributeId, item->isMfg);
					}
				}
			}
		}

		break;

	case 2:
		// Evt: DISABLE
		if(_perReport_Cmd == _PerReport_Cmd_Disable){
			_seq_ = 0;
			break;
		}

		if(++_shortCnt >= _SHORTTEMP_PERIOD){
			_shortCnt = 0;
			for(_perReport_t *item = *_perReportList; item != NULL; item = item->next){
				DEBUG("_per short report!");
				item->cnt = item->period;
				// report
				if(item->oneEp){
					_SEQ_Report_Add(1, item->clusterId, item->attributeId, item->isMfg);
				}
				else{
					for(uint8_t ep = 1; ep <= CHANNEL_NUM; ep++){
						_SEQ_Report_Add(ep, item->clusterId, item->attributeId, item->isMfg);
					}
				}
			}

			if(++_shortTotal >= _SHORTTEMP_COUNT){
				_shortTotal = 0;
				_seq_ = 1;
				break;
			}
		}

		break;

	default:
		break;
	}

	_perReport_Cmd = 0;
	emberEventControlSetDelayMS(_periodicReport_EC, 100);
}


void _perReportCmd(uint8_t no, _perReport_Cmd_t cmd){
	_perReport_Cmd = cmd>_PerReport_Cmd_ShortTemp?0:cmd;
	if(_perReport_Cmd == _PerReport_Cmd_RefleshPeriod){
		_perReport_ActNo = no;
		_shortCnt = 0;
		_shortTotal = 0;
	}
	(void)no;
}

// period单位0.1s
void _perReportItem_Add(uint8_t no, uint16_t cId, uint16_t aId, bool isMfg, uint32_t period, bool oneEp){
	_perReport_t *_item = malloc(sizeof(_perReport_t));
	_item->no = no;
	_item->clusterId = cId;
	_item->attributeId = aId;
	_item->isMfg = isMfg;
	_item->period = period;
	_item->cnt = period;
	_item->oneEp = oneEp;
	list_add(_perReportList, _item);
}

void _perReportItem_Del(uint8_t no){
	for(_perReport_t *_item = *_perReportList; _item != NULL; _item = _item->next){
		if(_item->no == no){
			list_remove(_perReportList, _item);
			free(_item);
		}
	}
}

void _periodicReportInit(void){
	list_init(_perReportList);
	emberEventControlSetDelayMS(_periodicReport_EC, 100);
}
