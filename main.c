//  Code principal agent MWAC sur microchip
//  A. Lagreze   Mai 2013
//  version 1.0

#include "controleur.h"
#include "mwac.h"
#include "MWACprotocol.h"
#define _DEF_THIS_AGENT_ 
#include "thisAgent.h"
#include "MWACProtocol.h"

// ********* prototype des fonctions *******
void init (void);
void main(void)
{
  init();
  //sendIntroductionMessage(); 
  gl_me.myRole=SIMPLE_MEMBER;
  gl_me.myGroup=0x11223344;
  sendWhoAreMyNeighboursMessage();
  while(1);
}

