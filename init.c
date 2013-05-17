//  Code initialisation de l' agent MWAC sur microchip
//  A. Lagreze   Mai 2013
#include "mwac.h"
#include <usart.h>
#include "serial.h"
#include "controleur.h"
#include "thisAgent.h"
#include "appli.h"
#include "LCDTools.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
static char messErr1[]="erreur baudrate";
char blancs[]="                ";
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
	FIFOinit(&gl_byteFifo,50);
    INTCON = 0x00;                //disable all interrupts
	// configure USART
	OpenUSART( USART_TX_INT_OFF &
	USART_RX_INT_ON&  /* interruption reception UART valid?*/
	USART_ASYNCH_MODE &  
	USART_EIGHT_BIT &
    USART_CONT_RX & 
	brgh,spbrg );
	 /*** mise en place contexte interruption ************/
    IPR1bits.RCIP=1 ;              //reception USART high priority
    RCONbits.IPEN = 1;            //enable priority levels
    INTCONbits.GIEH = 1;          //enable interrupts
	// ****  init LCD display
	strcpy(gl_blancs,blancs);
	sprintf (gl_ligne1,"Ag#%ld N %ld",gl_me.myId,gl_me.myGroup);
	#ifdef LCD_DISPLAY
	initLCD();
	clearLCD();
	afficheLCD(1,1,gl_ligne1);
	#endif
	
	
}