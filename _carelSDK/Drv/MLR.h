
#ifndef CARELSDK_DRV_MLR_H_
#define CARELSDK_DRV_MLR_H_

#define _MLR_INTERVAL					250

// Çý¶¯Ó³Éä
#define _DrvInitLight()					_DrvInit_MLR()
#define _DrvCtlLight(ep, newStatus)		_DrvWr_MLR(ep, newStatus)
#define _DrvCtlLightMask(newStatusMask)	_DrvWr_MLR_All(newStatusMask)






void _DrvInit_MLR(void);
void _DrvWr_MLR(uint8_t ch, uint8_t newStatus);
void _DrvWr_MLR_All(uint8_t newStatusMask);



#endif /* CARELSDK_DRV_MLR_H_ */
