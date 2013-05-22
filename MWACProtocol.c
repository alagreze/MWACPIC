//  Différent  
//  A. Lagreze   Mai 2013

#include "serial.h"
#include "thisAgent.h"
#include "mwac.h"
#define _DEF_MWACPROTO_
#include "MWACProtocol.h"
#include "appli.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "LCDTools.h"
static char gl_frameColor[]={01,0xFF,00,0x9,0xF0,04,00,0xFF,00,0xFF,00,0xFF,00,03,0xF7,02,0xFF} ;
void sendIntroductionMessage(void)
{
	appliPDU_T appPDU; // for preparing appli PDU
	appPDU.appliPDUlength= INTRODUCTION_PDU_MESSAGE_SIZE+APPLI_HEADER_SIZE;
	appPDU.appSenderId=gl_me.myId;
	appPDU.appReceiverId=(unsigned long int)BROADCAST_ADDR;
	appPDU.type=INTRODUCTION;  // message type
	sendDataLinkFrameSerialPort(gl_me.myId , BROADCAST_ADDR, &appPDU);
	
}
// ************************************************************************
// **  send a message  (char string terminated by 0 but this 0 is not sent)
//  to the agent receiveAg
// ************************************************************************
void sendDataMessage(unsigned long int receiveAg, char *message)
{
	appliPDU_T appPDU; // for preparing appli PDU
	appPDU.appliPDUlength= APPLI_HEADER_SIZE+strlen(message);
	appPDU.appSenderId=gl_me.myId;
	appPDU.appReceiverId=(unsigned long int)receiveAg;
	appPDU.type=DATA;  // message type
    strcpy(appPDU.message,message); // copy message into appPDU message
	sendDataLinkFrameSerialPort(gl_me.myId ,gl_me.myGroup, &appPDU);
	
}
void sendWhoAreMyNeighboursMessage(void)
{
	appliPDU_T appPDU; // for preparing appli PDU
	appPDU.appSenderId=gl_me.myId;
	appPDU.appReceiverId=(unsigned long int)BROADCAST_ADDR;
	appPDU.type=WHO_ARE_MY_NEIGHBOURS;  // message type
	appPDU.message[0]=gl_me.myRole;
    switch ( gl_me.myRole)
    {	
	 case NOTHING :
		appPDU.appliPDUlength= WHO_ARE_MY_NEIGHBOURS_PDU_MESSAGE_SIZE_WITHOUTGROUP+APPLI_HEADER_SIZE;
		break;
	 case SIMPLE_MEMBER :
		appPDU.appliPDUlength= WHO_ARE_MY_NEIGHBOURS_PDU_MESSAGE_SIZE_WITH_ONE_GROUP+APPLI_HEADER_SIZE;
		appPDU.message[1]=(gl_me.myGroup>>24)&0xFF;  // put the group appartenance in the message 
		appPDU.message[2]=(gl_me.myGroup>>16)&0xFF;
		appPDU.message[3]=(gl_me.myGroup>>8)&0xFF;
		appPDU.message[4]=gl_me.myGroup&0xFF;
		break;
	// to compleete to consider the other cases (LINK   REPRESENTATIVE)
    }	
	sendDataLinkFrameSerialPort(gl_me.myId , BROADCAST_ADDR, &appPDU);
}

void sendPresentationMessage(void)
{
	appliPDU_T appPDU; // for preparing appli PDU
	appPDU.appSenderId=gl_me.myId;
	appPDU.appReceiverId=(unsigned long int)BROADCAST_ADDR;
	appPDU.type=PRESENTATION;  // message type
	appPDU.message[0]=gl_me.myRole;
    switch ( gl_me.myRole)
    {	
	 case NOTHING :
		appPDU.appliPDUlength= WHO_ARE_MY_NEIGHBOURS_PDU_MESSAGE_SIZE_WITHOUTGROUP+APPLI_HEADER_SIZE;
		break;
	 case SIMPLE_MEMBER :
		appPDU.appliPDUlength= WHO_ARE_MY_NEIGHBOURS_PDU_MESSAGE_SIZE_WITH_ONE_GROUP+APPLI_HEADER_SIZE;
		appPDU.message[1]=(gl_me.myGroup>>24)&0xFF;  // put the group appartenance in the message 
		appPDU.message[2]=(gl_me.myGroup>>16)&0xFF;
		appPDU.message[3]=(gl_me.myGroup>>8)&0xFF;
		appPDU.message[4]=gl_me.myGroup&0xFF;
		break;
	// to compleete to consider the other cases (LINK   REPRESENTATIVE)
    }	
	sendDataLinkFrameSerialPort(gl_me.myId , BROADCAST_ADDR, &appPDU);
}
// ****************************************************
// **  read one frame from the byte FIFO et proceed it
// ****************************************************
void frameProceed(void)
{
    int i,j;
	unsigned long int appSenderAddr=0,groupAd;
	unsigned char appPDUType;
	unsigned char appPDUMessageSize=0;  // size of the message in applicative PDU 
	unsigned char neighbFound=0,posNeighb=0,nbGroup;
	char messRec[20];
	if (gl_byteFifo.framesCnt != 0)
	{ // there is a frame in he FIFO
	  // get sender applicative layer address from the FIFO
	  for(i=0;i<APPLICATION_ADDRESS_SIZE;i++)
		appSenderAddr=appSenderAddr+(((unsigned long int)pullCharFromFifo(&gl_byteFifo))<<(i*8));
	  // get type of applicative message
	  appPDUType=pullCharFromFifo(&gl_byteFifo);
	  //get the size of PDU data (message without applicative header )
	  appPDUMessageSize=pullCharFromFifo(&gl_byteFifo);;
      switch (appPDUType){
	  case PRESENTATION :
	  case WHO_ARE_MY_NEIGHBOURS:
		// update neighbours array
		for (i=0;i<gl_me.nbNeighbours;i++)
		{
		    // fetching for an already recorded neighbour
			if(gl_me.myNeighbours[i].agentId==appSenderAddr)
			{ neighbFound=TRUE;
			  posNeighb=i;
			}
		}
		if (!neighbFound )
		{ 	// we must add a new neighbour if not in the neighbours list 
			posNeighb=gl_me.nbNeighbours;
			if (gl_me.nbNeighbours<MAX_NB_NEIGHBOURS_IN_LIST)
				gl_me.nbNeighbours++;
		}
		//update neighbour list if there is still place in the list 
		if (posNeighb<MAX_NB_NEIGHBOURS_IN_LIST)
		{
			gl_me.myNeighbours[posNeighb].agentId=appSenderAddr;
			gl_me.myNeighbours[posNeighb].agentRole=pullCharFromFifo(&gl_byteFifo); // update the role of agent
			nbGroup=(appPDUMessageSize-1)/4;
			gl_me.myNeighbours[posNeighb].nbGroup=nbGroup;// update nb groups of the neighbour
			for (i=0;i<nbGroup;i++)
				{ groupAd=0;
				  for (j=0;j<APPLICATION_ADDRESS_SIZE;j++)
				     groupAd=(groupAd<<8)+((unsigned long int)pullCharFromFifo(&gl_byteFifo));
				  if(i < MAX_NB_GROUPS)
					// we record until MAX_NB_GROUPS groups per neighbour
					gl_me.myNeighbours[posNeighb].agentGroupId[i]=groupAd;
				}
			// if the neibghbour is  a representative agent, this agent becomes simple member and  his group is up_dated	
			if (gl_me.myNeighbours[posNeighb].agentRole==REPRESENTATIVE)
			{
			gl_me.myRole=SIMPLE_MEMBER;	
			gl_me.myGroup=gl_me.myNeighbours[posNeighb].agentGroupId[0]; // this agent gets the group of the representative agent 
			sprintf (gl_ligne1,"Ag#%ld S %ld",gl_me.myId,gl_me.myGroup);
			colorViewModificationForSimpleMemberSystemFrameSend();
			#ifdef LCD_DISPLAY 
			afficheLCD(1,1,gl_blancs);
			afficheLCD(1,1,gl_ligne1);
			#endif	
			}
			else;
	
		}
		else
		{  // no place in neighbours list  we must pull the end of frame from the FIFO
			for (i=0;i<appPDUMessageSize;i++)
				pullCharFromFifo(&gl_byteFifo);
	  
        }
	    break;
	case DATA :	
		for(i=0;i<appPDUMessageSize;i++)
			messRec[i]=pullCharFromFifo(&gl_byteFifo);
		sprintf (gl_ligne2,"MAg#%ld ",appSenderAddr);
		strcat(gl_ligne2,messRec);
		#ifdef LCD_DISPLAY 
		afficheLCD(1,2,gl_blancs);
		afficheLCD(1,2,gl_ligne2);
		#endif 
		break;
			
	}
	INTCONbits.GIEH = 0; // interrupts masked
    gl_byteFifo.framesCnt--;  // one frame less
	INTCONbits.GIEH = 1; // interrupts unmasked
	if(appPDUType== WHO_ARE_MY_NEIGHBOURS)
			sendPresentationMessage();


}
}

// ********************************************************
// **  send frame for coloration for a simple member agent
// *********************************************************

void colorViewModificationForSimpleMemberSystemFrameSend (void)
{
  int i;
  for (i=0;i<COLORATION_SIMPLE_MEMBER_FRAME_SIZE;i++)
		sendCharSerialPort(gl_frameColor[i]);
}