
#ifndef CARELSDK_DRV_HC595_H_
#define CARELSDK_DRV_HC595_H_
#if 0

#define _HC595_INTERVAL					250

// Çý¶¯Ó³Éä
#define _DrvInitLight()					_DrvInit_HC595()
#define _DrvCtlLight(ep, newStatus)		_DrvWr_HC595(ep, newStatus)
#define _DrvCtlLightMask(newStatusMask)	_DrvWr_HC595_All(newStatusMask)

void _DrvInit_HC595(void);
void _DrvWr_HC595(uint8_t ch, uint8_t newStatus);
void _DrvWr_HC595_All(uint8_t newStatusMask);
#endif

#endif /* CARELSDK_DRV_HC595_H_ */
