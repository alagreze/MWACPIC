//  Code initialisation de l' agent MWAC sur microchip
//  A. Lagreze   Mai 2013
#include "mwac.h"
#include <usart.h>
#include "serial.h"
#include "controleur.h"
#include "thisAgent.h"
#include "appli.h"
#include "LCDTools.h"

static char messErr1[]="erreur baudrate";
void init (void)
{
	unsigned char brgh,spbrg;
	// agent initialization 
	gl_me.myId=THIS_AGENT_ID;
	gl_me.myRole=NOTHING;
	gl_me.myGroup=NO_GROUP;
	gl_me.nbNeighbours=0;
    // serial link initialization
	 if (calculeBAudRateGenerateur(OSCI_FREQ,SERIAL_BAUD_RATE,&brgh,&spbrg)!=SUCCESS)
      /** afficher  message d erreur sur LCD*/
	{ afficheLCD(1, 2,messErr1 );
      while (1); /* boucle sur l'erreur */
    }
    /*** mise en place contexte interruption ************/
    INTCON = 0x00;                //disable all interrupts
	// configure USART
	OpenUSART( USART_TX_INT_OFF &
	USART_RX_INT_OFF&  /* interruption reception UART valid?*/
	USART_ASYNCH_MODE &  
	USART_EIGHT_BIT &
    USART_CONT_RX & 
	brgh,spbrg );
	
}