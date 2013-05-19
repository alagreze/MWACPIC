//  miscellaneous declaration
#ifndef __APPLI__
#define __APPLI__
#include "mwac.h"
#define  LCD_DISPLAY  // to uncomment when we use LCD Display
#define FAIL 0
#define SUCCESS 1
#define TRUE 1
#define FALSE 0

#ifdef  _DEF_APPLI_
#define PUBLIC_APPLI 
#else
#define PUBLIC_APPLI extern
#endif
// *** variables globales **
PUBLIC_APPLI char gl_ligne1[20];
PUBLIC_APPLI char gl_ligne2[20];
PUBLIC_APPLI char gl_blancs[17];
PUBLIC_APPLI me_T gl_me;
#endif
