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
#include "boutons.h"
#include <string.h>
#include <stdio.h>
#include "LM75.h"

// ********* prototype des fonctions *******
void init (void);
// ********* globales *******
char tempe[10];
char message[MAX_SIZE_MESSAGE];
// ********* programme principal *******
void main(void)
{
  unsigned long int tempeReceiverAgent=1;
  init();
  sendIntroductionMessage(); 
  sendWhoAreMyNeighboursMessage();
  while(1)
  {
    if (bouton(BoutonS2))
    {
	  getTempeAsString(tempe, TC74CARTE);
	  tempeReceiverAgent++;
	  sprintf(message,"Mag#%ld ",tempeReceiverAgent);
	  strcat(message,tempe);	
      sendDataMessage(tempeReceiverAgent, message);
		
    }
    frameProceed();  //  polling the FIFO and proceeds one frame if there is. If no, give the hand back 
  }
}
  