/***
 * �ļ�		��common.c
 * ����		��
 * ��������	��2019/9/9
 * �����	��Carel.Li
 * �޸�		: -
 * �汾		��v1.0
 */
#include "./common.h"
#include <stdlib.h>

// ������־
bool startup = true;

// EndPoint����
LIST(EP_LIST);



/**************************************************************************|
 * ������	_GET_BIT_FROMBITS8
 * ���ͣ�	-
 * ���ܣ�	λ��ṹ�� λȡֵ
 * ���ڣ�	-
 * ������	index-λ������data-����
 * ���أ�	λֵ
 ***/
uint8_t _GET_BIT_FROMBITS8(uint8_t index, mulbits8_t *data){
	switch(index){
	case 1:
		return (uint8_t)((*data).type_bits8.bit_0);
	case 2:
		return (uint8_t)((*data).type_bits8.bit_1);
	case 3:
		return (uint8_t)((*data).type_bits8.bit_2);
	case 4:
		return (uint8_t)((*data).type_bits8.bit_3);
	case 5:
		return (uint8_t)((*data).type_bits8.bit_4);
	case 6:
		return (uint8_t)((*data).type_bits8.bit_5);
	case 7:
		return (uint8_t)((*data).type_bits8.bit_6);
	case 8:
		return (uint8_t)((*data).type_bits8.bit_7);
	default:
		return 0xFF;
	}
}


/**************************************************************************|
 * ������	_SET_BIT_FROMBITS8
 * ���ͣ�	-
 * ���ܣ�	λ��ṹ�� λ��ֵ
 * ���ڣ�	-
 * ������	index-λ������data-����bit-λ��ֵ
 * ���أ�	-
 ***/
void _SET_BIT_FROMBITS8(uint8_t index, mulbits8_t *data, uint8_t bit){
	switch(index){
	case 1:
		(*data).type_bits8.bit_0 = bit==0?0:1;
		break;
	case 2:
		(*data).type_bits8.bit_1 = bit==0?0:1;
		break;
	case 3:
		(*data).type_bits8.bit_2 = bit==0?0:1;
		break;
	case 4:
		(*data).type_bits8.bit_3 = bit==0?0:1;
		break;
	case 5:
		(*data).type_bits8.bit_4 = bit==0?0:1;
		break;
	case 6:
		(*data).type_bits8.bit_5 = bit==0?0:1;
		break;
	case 7:
		(*data).type_bits8.bit_6 = bit==0?0:1;
		break;
	case 8:
		(*data).type_bits8.bit_7 = bit==0?0:1;
		break;
	default:
		break;
	}
}

/**************************************************************************|
 * ������	_epList_Init
 * ���ͣ�	-
 * ���ܣ�	��ʼ��
 * ���ڣ�	-
 * ������	-
 * ���أ�	-
 ***/
void _epList_Init(void){
	list_init(EP_LIST);
}



/**************************************************************************|
 * ������	_getEndpointCount
 * ���ͣ�	-
 * ���ܣ�	��ȡendPoint������
 * ���ڣ�	-
 * ������	-
 * ���أ�	endpoint������
 ***/
uint8_t _getEndpointCount(void){
	return list_length(EP_LIST);
}


/**************************************************************************|
 * ������	_removeEndpointIterm
 * ���ͣ�	-
 * ���ܣ�	ɾ��endPoint����
 * ���ڣ�	-
 * ������	iterm-endPoint����
 * ���أ�	-
 ***/
void _removeEndpointIterm(endPoint_t *iterm){
	list_remove(EP_LIST,iterm);
}


/**************************************************************************|
 * ������	_getEndpointIterm
 * ���ͣ�	-
 * ���ܣ�	����endPoint��������ȡendpoint�������
 * ���ڣ�	-
 * ������	ep-ָ��Endpoint
 * ���أ�	endpoint�������
 ***/
endPoint_t *_getEndpointIterm(uint8_t ep){
	if(*EP_LIST == NULL)
		return NULL;
	for(endPoint_t *buf_ep = *EP_LIST;buf_ep != NULL;buf_ep = buf_ep->next){
		if(buf_ep->endPoint == ep){
			return buf_ep;
		}
	}

	return NULL;
}


/**************************************************************************|
 * ������	addList_EP
 * ���ͣ�	-
 * ���ܣ�	���Endpoint����
 * ���ڣ�	-
 * ������	ep-ָ��Endpoint��mode-ָ��endPoint���͡�data-endPoint����
 * ���أ�	-
 ***/
void addList_EP(uint8_t ep, void *data){

	endPoint_t *bufEp = malloc(sizeof(endPoint_t));

	if(data == NULL)
		return;

	bufEp->endPoint = ep;
	bufEp->data = data;

	list_add(EP_LIST,bufEp);
}













void _epListSw_Add(uint8_t ep, _epListSwData_t *dat){
	_epListSwData_t *buf = malloc(sizeof(_epListSwData_t));
	memcpy(buf, dat, sizeof(_epListSwData_t));
	addList_EP(ep, buf);
}


_epListSwData_t *_epListSw_GetByEp(uint8_t ep){
	return (_epListSwData_t *)(_getEndpointIterm(ep)->data);
}
