#ifndef __BOUTONSH__
#define __BOUTONSH__
#include "appli.h"
/**** module boutons.h *******************************************/
/*******************declaration des constantes *******************/
/*** pour le module boutons.c ************************************/
/****************************************************************/

#define S2 PORTAbits.RA4
#define S3 PORTBbits.RB0
#define BoutonS2 2
#define BoutonS3 3
#define MASK_S2 0x10
#define MASK_S3 0x1
#ifdef _DEF_BOUTONS_
  #define PUBLIC_BOUTONS
#else
  #define PUBLIC_BOUTONS extern
#endif
PUBLIC_BOUTONS char bouton(char numBouton);
#endif
