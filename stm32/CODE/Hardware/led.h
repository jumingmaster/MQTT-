#ifndef _LED_H
#define _LED_H
#include "sys.h"
#include <stm32f4xx.h>
#include "Communication.h"

#define AlarmLED            PFout(9)
#define RuntimeLED          PFout(10)
#define FigurePrintLED      PEout(13)
#define DataLED             PEout(14)
#define Bowl                PAout(6)

#define AlarmLED_ON         (AlarmLED = 0)
#define AlarmLED_OFF        (AlarmLED = 1)
#define RuntimeLED_ON       (RuntimeLED = 0)
#define RuntimeLED_OFF      (RuntimeLED = 1)
#define FigurePrintLED_ON   (FigurePrintLED = 0)
#define FigurePrintLED_OFF  (FigurePrintLED = 1)
#define DataLED_ON          (DataLED = 0)
#define DataLED_OFF         (DataLED = 1)

void Init_LED(void);
void Bowl_ON(void);
void Bowl_OFF(void);
#endif
