//  ** serial link relative declarations ******
//****** function prototypes *******
#ifndef __SERIAL__
#define __SERIAL__
#include "mwac.h"

//************** constantes diverses 
#define BYTE_BUFFER_SIZE 200
//************ definition de types liés au lien série
struct { unsigned char *ptRead;
		 unsigned char *ptWrite;
		 unsigned int nbByte;
		 unsigned int bufSize;
		 unsigned int overFlowCnt;
		 unsigned char buffer[BYTE_BUFFER_SIZE];}
         byteFIFO_T;
		 
#ifdef _DEF_SERIAL_
#define PUBLIC_SER
#else
#define PUBLIC_SER extern
#endif 
//**********  prototype des fonctions ************************
PUBLIC_SER void sendAgentAdrSerialPort(unsigned long int agentAdr);
PUBLIC_SER void sendDataLinkFrameSerialPort( unsigned long int senderId, unsigned long int receiverId, appliPDU_T *ptAppliPDU);
PUBLIC_SER void sendCharSerialPort(char c);
PUBLIC_SER void sendShortIntSerialPort(unsigned short int val);
PUBLIC_SER void sendFrameHeaderSerialPort(void);
PUBLIC_SER char calculeBAudRateGenerateur( float freqQuartz, float debitDesire, unsigned char *brgh, unsigned char *spbrg);
PUBLIC  unsigned *advancePointer(unsigned char *pt,byteFIFO_T *fifo);
		 
// **************variables globales ***************************
PUBLIC byteFIFO_T gl_byteFifo;
#endif 