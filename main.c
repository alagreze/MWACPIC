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
//**  read one frame from the byte FIFO et proceed it
void frameProceed(void)
{
    int i;
	unsigned long int appSenderAddr=0;
	unsigned char appPDUType;
	if (gl_byteFifo.nbByte != 0)
	{ // there is a frame in he FIFO
	  // get sender applicative layer address from the FIFO
	  for(i=0;i<APPLICATION_ADDRESS_SIZE;i++)
		appSenderAddr=appSenderAddr+(((unsigned long int)pullCharFromFifo(&gl_byteFifo))<<(i*8));
	  // get type of applicative message
	  appPDUType=pullCharFromFifo(&gl_byteFifo);
	  //get the size of PDU data (message without applicative header 
      
}
  