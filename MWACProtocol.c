//  Différent  
//  A. Lagreze   Mai 2013

#include "serial.h"
#include "thisAgent.h"
#include "mwac.h"
#define _DEF_MWACPROTO_
#include "MWACProtocol.h"

void sendIntroductionMessage(void)
{
	appliPDU_T appPDU; // for preparing appli PDU
	appPDU.appliPDUlength= INTRODUCTION_PDU_MESSAGE_SIZE+APPLI_HEADER_SIZE;
	appPDU.appSenderId=gl_me.myId;
	appPDU.appReceiverId=(unsigned long int)BROADCAST_ADDR;
	appPDU.type=INTRODUCTION;  // message type
	sendDataLinkFrameSerialPort(gl_me.myId , BROADCAST_ADDR, &appPDU);
	
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
	// to complete to consider the other cases (LINK   REPRESENTATIVE)
    }	
	sendDataLinkFrameSerialPort(gl_me.myId , BROADCAST_ADDR, &appPDU);
}