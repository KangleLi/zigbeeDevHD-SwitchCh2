
#ifndef CARELSDK_GENERALAPP_KEY_SCAN_H_
#define CARELSDK_GENERALAPP_KEY_SCAN_H_

//////////////////////////////////////////////////////////////////////////
// ������ ���� ������
//////////////////////////////////////////////////////////////////////////
#define PERIOD_KEY_SCAN				((uint32_t)(50))	// Key��������
#define DELAY_KEY_AS				1					// ��������(>0)
//////////////////////////////////////////////////////////////////////////
// ������  ���� ������
//////////////////////////////////////////////////////////////////////////



// Key �����ӿڶ���
typedef uint8_t (*func_get_f)(void);
typedef void (*pressed_callback_f)(void);
typedef void (*released_callback_f)(uint32_t);
typedef void (*pp_callback_f)(uint32_t);

// Key �ṹ��
struct KEY_SCAN_t{
	struct KEY_SCAN_t *next;
	uint8_t num;							// ���
	uint8_t seq;							// ״̬������
	uint8_t state;							// ״̬ [1-����   0-�ͷ�]
	uint16_t comcnt;						// ��������
	uint32_t pcnt;							// ���¼���
	func_get_f func_get; 					// ״̬��ȡ����
	pressed_callback_f pressed_callback;	// ���»ص�
	released_callback_f released_callback;	// �ɿ��ص�
	pp_callback_f pp_callback;				// ��ѹ̬���ڻص�
};

// API
void gKey_Add(uint8_t num, void *func_get, void *pressed_callback, \
		      void *released_callback, void *pp_callback);
void startApp_Keyscan(uint32_t delay);
void stopApp_Keyscan(void);
struct KEY_SCAN_t *getKeyItermPtr(uint8_t keyNum);

#endif /* CARELSDK_GENERALAPP_KEY_SCAN_H_ */
