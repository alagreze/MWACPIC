//  miscellaneous declaration
#ifndef __APPLI__
#define __APPLI__

//#define  LCD_DISPLAY  // to uncomment when we use LCD Display
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

#endif
