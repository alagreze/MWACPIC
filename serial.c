//**********serial link relative functions *****
//  A Lagreze   Mai 2013

#include "controleur.h"
#include <usart.h>
#include "mwac.h"
#include "appli.h"
#define _DEF_SERIAL_ 
#include "serial.h"



//***** send on the serial port a data link level frame *****
void sendDataLinkFrameSerialPort( unsigned long int senderId, unsigned long int receiverId, appliPDU_T *ptAppliPDU)
{
    int i;
    sendFrameHeaderSerialPort();
	sendAgentAdrSerialPort(senderId);
	sendAgentAdrSerialPort(receiverId);
	sendShortIntSerialPort(ptAppliPDU->appliPDUlength);	
	sendAgentAdrSerialPort(ptAppliPDU->appSenderId);
	sendAgentAdrSerialPort(ptAppliPDU->appReceiverId);
	sendCharSerialPort(ptAppliPDU->type);
    for (i=0;i<ptAppliPDU->appliPDUlength-APPLI_HEADER_SIZE;i++)
      	sendCharSerialPort(ptAppliPDU->message[i]);
}



//******  send one character on serial link **/
void sendCharSerialPort(char c)
{  while(!PIR1bits.TXIF); // wait for USART free
   TXREG=c; //character emission
}
	
//******  send data link frame header on serial port **/
 void sendFrameHeaderSerialPort()
 {
	sendCharSerialPort((unsigned char)HEADER1);
	sendCharSerialPort((unsigned char)HEADER2);
 }
//******  send a short int (16 bits) in big endian on serial port  **/
 void sendShortIntSerialPort(unsigned short int val)
 {
	sendCharSerialPort((val>>8)&0xFF);  
	sendCharSerialPort(val&0xFF);
 }
 
 //******  send an agent address  (long  int (32 bits)) in big endian on serial port  **/
  void sendAgentAdrSerialPort(unsigned long int agentAdr)
 {
	sendCharSerialPort((agentAdr>>24)&0xFF);  
	sendCharSerialPort((agentAdr>>16)&0xFF);  
	sendCharSerialPort((agentAdr>>8)&0xFF);
	sendCharSerialPort(agentAdr&0xFF);
 }
 
 
 /*****************************************************************************
  Fonction de calcul des registres du générateur d'horloge
  entree: frequence quartz de la carte en MHz, frequence bit desiree en bits par secondes
  programme pour le maximum de clarte mais on pourrait optimiser
******************************************************************************/
char calculeBAudRateGenerateur( float freqQuartz, float debitDesire, unsigned char *brgh, unsigned char *spbrg)
{
	float divis;
	divis= (freqQuartz*1.E6/ debitDesire/16.0 -1.0);
    if (divis >255) 
    {  	
		divis=(divis+1)/4.0-1.0;
        if (divis >255) return (FAIL);
		else
		{
			*brgh = USART_BRGH_LOW;
			*spbrg = (char)divis;
			return(SUCCESS);
		}
	}
	else
	{
		*brgh = USART_BRGH_HIGH;
		*spbrg = (char)divis;	
		return(SUCCESS);
	}
		
}

/***********************************************************************
 avance du pointeur de lecture ou d'ecriture  et
 simulation de la structure circulaire d'une FIFO
 en entrée: ptCar est le pointeur à avancer
            ptFifo pointe sur la FIFO concernée par l'avance du pointeur
 en sortie: nouvelle valeur du pointeur apres avance d'un cran
 ***********************************************************************/
unsigned char *advancePointeur(unsigned char *ptCar,byteFIFO_T *ptFifo)
{
    ptCar++;
	if (ptCar==(ptFifo->buffer+ptFifo->bufSize))
		ptCar=ptFifo->buffer;
	return ptCar;

}
