#include "controleur.h"
#include "usart.h"
#include "serial.h"
#include "thisAgent.h"

void serveurUART (void);

//----------------------------------------------------------------------------
// High priority interrupt vector

#pragma code InterruptVectorHigh = 0x08
void
InterruptVectorHigh (void)
{
  _asm
    goto serveurUART //jump to interrupt routine
  _endasm
}


//----------------------------------------------------------------------------
// High priority interrupt routine

#pragma code
#pragma interrupt serveurUART
/*********************************************************************
serveur d'interruption de l'UART
On prétraite les trames de niveau data link en supprimant le header
et en stockant dans une FIFO uniquement  les trames de type  PRESENTATION
ou DATA en broadcast ou qui nous est destinée
**********************************************************************/

void serveurUART()
{
  static unsigned long int s_appSenderAddr, s_appReceiverAddr,s_appPDUSize;
  static unsigned short int s_appPDUType;
  static unsigned char s_state=0;
  static unsigned short int s_charCnt;
  unsigned char recChar;
  int i;
  if (PIR1bits.RCIF)
  {
    recChar=RCREG;
	switch(s_state) 
	{
	   case 0:  // attend HEADER1
			if (recChar==HEADER1)
				s_state=1;
				s_appSenderAddr=0;
				s_appReceiverAddr=0;
				s_appPDUSize=0;
				s_appPDUType=0;
				s_charCnt=0;
			break;
	   case 1: // attend HEADER2
			if (recChar==HEADER2)
				s_state=2;
			break;
	   case 2: // on recoit les adresses niveau datalink sans les memoriser
			s_charCnt++;
			if (s_charCnt== (2*DATA_LINK_ADDRESS_SIZE))
			{
				s_state=3;
				s_charCnt=0;
			}
			break;
		 case 3: // on recoit la taille du Application PDU
			s_charCnt++;
			s_appPDUSize=(s_appPDUSize<<8)+recChar;
			if (s_charCnt== APPLI_PDU_LENGTH_SIZE)
			{
				s_state=4;
				s_charCnt=0;
			}
			break;
		case 4: // build applicative address of the sender
		    s_charCnt++;
			s_appSenderAddr=(s_appSenderAddr<<8)+recChar;
			if ( s_charCnt==APPLICATION_ADDRESS_SIZE)
			{
				s_state=5;
				s_charCnt=0;
			}
			break;
		case 5: // build applicative address of the receiver
			s_charCnt++;
			s_appReceiverAddr=(s_appReceiverAddr<<8)+recChar;
			if ( s_charCnt==APPLICATION_ADDRESS_SIZE)
			{
				if ((s_appReceiverAddr==BROADCAST_ADDR)|| (s_appReceiverAddr==gl_me.myId))
				{
				 // broadcast or for me
				  s_state=6;
				  s_charCnt=0;
				}
				else 
				{ // we do not consider the frames not for me and not in broadcast
				  s_state=0;
				}
			}
			break;
		case 6:  // receive  type of message
			s_appPDUType=recChar;
			if ((s_appPDUType==PRESENTATION)||(s_appPDUType==DATA)||(s_appPDUType==WHO_ARE_MY_NEIGHBOURS))
			{
				// if type is accepted 
				
				for (i=0;i<APPLICATION_ADDRESS_SIZE;i++)
				{
					// push the sender application address into the FIFO en little endian
					pushCharIntoFifo(s_appSenderAddr&0xFF,&gl_byteFifo);
					s_appSenderAddr=s_appSenderAddr>>8;
				}
				pushCharIntoFifo(s_appPDUType,&gl_byteFifo);  // push type of message into the FIFO
				s_charCnt=s_appPDUSize-APPLI_HEADER_SIZE;  // size of applicative layer data  (applicative message)
				pushCharIntoFifo(s_charCnt,&gl_byteFifo);  
				if (s_charCnt >0)
				    s_state=7; // this frame must be continued
				else
					 s_state=0;  // this frame is over 
			}
			else
				s_state=0; // no interest for this frame
			break;  
		case 7 : // put into the FIFO the data of applicative message
			pushCharIntoFifo(recChar,&gl_byteFifo);
			s_charCnt--;
			if (s_charCnt==0)
					s_state=0; // frame is complete
			break;
				
  }

}
}
