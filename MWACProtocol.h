//  ** MWAC protocol  relative declarations ******
//****** function prototypes *******
#ifndef __MWACPROTO__
#define __MWACPROTO__
#include "mwac.h"
#ifdef _DEF_MWACPROTO_
 #define PUBLIC_MWAC 
#else
 #define PUBLIC_MWAC extern
#endif 
PUBLIC_MWAC void sendIntroductionMessage(void);
PUBLIC_MWAC void sendWhoAreMyNeighboursMessage(void);

#endif 