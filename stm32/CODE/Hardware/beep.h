#ifndef _BEEP_H
#define _BEEP_H
#include <stm32f4xx.h>
#include "sys.h"

#define beep            PFout(8)    
#define BeepRing        (PFout(8) = 1)
#define BeepCalm        (PFout(8) = 0)

void Init_Beep(void);

#endif
