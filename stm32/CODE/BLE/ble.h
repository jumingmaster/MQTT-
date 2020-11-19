#ifndef __BLE_H
#define __BLE_H
#include "stm32f4xx.h"

#define     UNLOCK      "bleunlock"
#define     LOCK        "blelock"

extern int BLE_RX_FLAG;

void Init_BLE(void);


#endif
