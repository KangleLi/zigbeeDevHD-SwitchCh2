/***
 * 文件		：common.c
 * 描述		：
 * 创建日期	：2019/9/9
 * 设计者	：Carel.Li
 * 修改		: -
 * 版本		：v1.0
 */
#include "./common.h"
#include <stdlib.h>

// 启动标志
bool startup = true;

// EndPoint链表
LIST(EP_LIST);



/**************************************************************************|
 * 函数：	_GET_BIT_FROMBITS8
 * 类型：	-
 * 功能：	位域结构体 位取值
 * 周期：	-
 * 参数：	index-位索引、data-对象
 * 返回：	位值
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
 * 函数：	_SET_BIT_FROMBITS8
 * 类型：	-
 * 功能：	位域结构体 位赋值
 * 周期：	-
 * 参数：	index-位索引、data-对象、bit-位新值
 * 返回：	-
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
 * 函数：	_epList_Init
 * 类型：	-
 * 功能：	初始化
 * 周期：	-
 * 参数：	-
 * 返回：	-
 ***/
void _epList_Init(void){
	list_init(EP_LIST);
}



/**************************************************************************|
 * 函数：	_getEndpointCount
 * 类型：	-
 * 功能：	获取endPoint链表长度
 * 周期：	-
 * 参数：	-
 * 返回：	endpoint链表长度
 ***/
uint8_t _getEndpointCount(void){
	return list_length(EP_LIST);
}


/**************************************************************************|
 * 函数：	_removeEndpointIterm
 * 类型：	-
 * 功能：	删除endPoint对象
 * 周期：	-
 * 参数：	iterm-endPoint对象
 * 返回：	-
 ***/
void _removeEndpointIterm(endPoint_t *iterm){
	list_remove(EP_LIST,iterm);
}


/**************************************************************************|
 * 函数：	_getEndpointIterm
 * 类型：	-
 * 功能：	根据endPoint索引，获取endpoint链表对象
 * 周期：	-
 * 参数：	ep-指定Endpoint
 * 返回：	endpoint链表对象
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
 * 函数：	addList_EP
 * 类型：	-
 * 功能：	添加Endpoint对象
 * 周期：	-
 * 参数：	ep-指定Endpoint、mode-指定endPoint类型、data-endPoint数据
 * 返回：	-
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
