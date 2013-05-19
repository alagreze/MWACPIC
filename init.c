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


// CONFIG1H
#pragma config OSC = ECIO       // Oscillator Selection bits (EC oscillator w/ OSC2 configured as RA6)
#pragma config OSCS = OFF       // Oscillator System Clock Switch Enable bit (Oscillator system clock switch option is disabled (main oscillator is source))

// CONFIG2L
#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOR = OFF        // Brown-out Reset Enable bit (Brown-out Reset disabled)
//#pragma config BORV = 25        // Brown-out Reset Voltage bits (VBOR set to 2.5V)

// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config WDTPS = 128      // Watchdog Timer Postscale Select bits (1:128)

// CONFIG4L
#pragma config STVR = ON        // Stack Full/Underflow Reset Enable bit (Stack Full/Underflow will cause Reset)
#pragma config LVP = OFF        // Low-Voltage ICSP Enable bit (Low-Voltage ICSP disabled)

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000200-001FFFh) not code protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (002000-003FFFh) not code protected)
#pragma config CP2 = OFF        // Code Protection bit (Block 2 (004000-005FFFh) not code protected)
#pragma config CP3 = OFF        // Code Protection bit (Block 3 (006000-007FFFh) not code protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot Block (000000-0001FFh) not code protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM not code protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000200-001FFFh) not write protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (002000-003FFFh) not write protected)
#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (004000-005FFFh) not write protected)
#pragma config WRT3 = OFF       // Write Protection bit (Block 3 (006000-007FFFh) not write protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) not write protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot Block (000000-0001FFh) not write protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000200-001FFFh) not protected from Table Reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (002000-003FFFh) not protected from Table Reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (004000-005FFFh) not protected from Table Reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection bit (Block 3 (006000-007FFFh) not protected from Table Reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot Block (000000-0001FFh) not protected from Table Reads executed in other blocks)


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