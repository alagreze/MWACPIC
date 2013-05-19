//  Definitions relatives aux délais fait maison
// A Lagreze MAi 2013
#ifndef __DELAISH__
#define _DELAISH_
#include "controleur.h"
#ifdef _DEF_DELAIS_
  #define PUBLIC_DELAIS
#else
  #define PUBLIC_DELAIS extern
#endif
PUBLIC_DELAIS  char delaiEnSec(unsigned int nbSec);
PUBLIC_DELAIS void delaiEnMilliSec(unsigned int nbMilliSec);
#endif 