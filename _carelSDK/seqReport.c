#include "./common.h"

LIST(_seqReport_list);

EmberEventControl _SEQ_Report_EC;

// 初始化
// 初始化链表、启动event
void _SEQ_Report_Init(void){
	list_init(_seqReport_list);
	emberEventControlSetDelayMS(_SEQ_Report_EC, 1);
}

void _SEQ_Report_Add(uint8_t endpoint, uint16_t clusterId, uint16_t attributeId, bool ifMfg){
	_seq_report_t *_item_ = malloc(sizeof(_seq_report_t));

	_item_->clusterId = clusterId;
	_item_->attributeId = attributeId;
	_item_->ifMfg = ifMfg;
	_item_->endpoint = endpoint;

	// 添加到list尾
	list_add(_seqReport_list, _item_);

	if(!emberEventControlGetActive(_SEQ_Report_EC)){
		emberEventControlSetActive(_SEQ_Report_EC);
	}
}


// 序列式report机制
void _SEQ_Report_EF(void){
	emberEventControlSetInactive(_SEQ_Report_EC);

	// 获取list头
	_seq_report_t *_item_ = (_seq_report_t *)list_head(_seqReport_list);

	if(_item_ != NULL){
		DEBUG("_seq Report, c:  0x%2X, a: 0x%2X", _item_->clusterId, _item_->attributeId);
		if(_item_->ifMfg){
			_reportMfgServerAttribute(0x0000, _item_->endpoint, 1, \
									_item_->clusterId, _item_->attributeId);
		}
		else{
			// ontime 特殊处理
			if((_item_->clusterId == 0x0006) && (_item_->attributeId == 0x4001)){
				uint16_t ontime = _readAttr_OnTime(_item_->endpoint);
				if(ontime == 0){
					_reportStdAttribute(0x0000, _item_->endpoint, 1, \
														_item_->clusterId, _item_->attributeId, 1);
				}

			}
			else{
				_reportStdAttribute(0x0000, _item_->endpoint, 1, \
													_item_->clusterId, _item_->attributeId, 1);
			}
		}

		free(list_pop(_seqReport_list));
	}

	if(list_length(_seqReport_list) > 0){
		emberEventControlSetDelayMS(_SEQ_Report_EC, _PERIOD_SEQ_REPORT);
	}
}


#if 0

void _SEQ_Report_Add(uint8_t endpoint, uint16_t clusterId, uint16_t attributeId, bool ifMfg, uint32_t delayMS, bool ifCyc){
	_seq_report_t *_item_ = malloc(sizeof(_seq_report_t));

	_item_->clusterId = clusterId;
	_item_->attributeId = attributeId;
	_item_->ifMfg = ifMfg;
	_item_->delayMs = delayMS;
	_item_->curCnt = 0;
	_item_->ifCyc = ifCyc;
	_item_->endpoint = endpoint;

	list_add(_seqReport_list, _item_);
}

void _SEQ_Report_EF(void){
	emberEventControlSetInactive(_SEQ_Report_EC);

	for(_seq_report_t *_item_ = *_seqReport_list; _item_ != NULL; _item_ = _item_->next){
		if((_item_->curCnt += _PERIOD_SEQ_REPORT) >= _item_->delayMs){
			// report
			if(_item_->ifMfg){
				_reportMfgServerAttribute(0x0000, _item_->endpoint, 1, \
						_item_->clusterId, _item_->attributeId);
			}
			else{
				_reportStdAttribute(0x0000, _item_->endpoint, 1, \
						_item_->clusterId, _item_->attributeId, 1);
			}
			///////////////////
			if(_item_->ifCyc){
				// Cyclic
				_item_->curCnt = 0;
			}
			else{
				// Delete
				list_remove(_seqReport_list, _item_);
				free(_item_);
			}
		}
	}

	emberEventControlSetDelayMS(_SEQ_Report_EC, _PERIOD_SEQ_REPORT);
}
#endif
