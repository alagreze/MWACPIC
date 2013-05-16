//  Code principal agent MWAC sur microchip
//  A. Lagreze   Mai 2013
//  version 1.0

#include "controleur.h"
#include "mwac.h"
#include "MWACprotocol.h"
#define _DEF_THIS_AGENT_ 
#include "thisAgent.h"
#include "MWACProtocol.h"
#include "serial.h"
#define _DEF_APPLI_
#include "appli.h"
me_T gl_me;

// ********* prototype des fonctions *******
void init (void);

// ********* programme principal *******
void main(void)
{
  init();
  sendIntroductionMessage(); 
  sendWhoAreMyNeighboursMessage();
  while(1)
  {
    frameProceed();  //  polling the FIFO and proceeds one frame if there is. If no, give the hand back 
  }
}
  