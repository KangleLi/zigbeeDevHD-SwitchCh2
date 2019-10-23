
#ifndef CARELSDK_PROJECT_DIMSW_CH1_2_PORTS_H_
#define CARELSDK_PROJECT_DIMSW_CH1_2_PORTS_H_

// Çý¶¯Ó³Éä
#define _DrvInitKeyLed()		{ \
	extern void KEY1_HalInit(void);	\
	extern void KEY2_HalInit(void);	\
	extern void LED1_HalInit(void);	\
	extern void LED2_HalInit(void);	\
	KEY1_HalInit();	\
	KEY2_HalInit();	\
	LED1_HalInit();	\
	LED2_HalInit();	\
}


void LED1_Handler(uint8_t newState);
void LED2_Handler(uint8_t newState);
uint8_t KEY1_Get(void);
void KEY1_pdCB(void);
void KEY1_rdCB(uint32_t tim);
void KEY1_ppCB(uint32_t tim);
uint8_t KEY2_Get(void);
void KEY2_pdCB(void);
void KEY2_rdCB(uint32_t tim);
void KEY2_ppCB(uint32_t tim);


#endif /* CARELSDK_PROJECT_DIMSW_CH1_2_PORTS_H_ */
