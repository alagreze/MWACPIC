//  Definitions relatives au protocole MWAC
// A Lagreze MAi 2013
#ifndef __MWAC__
#define __MWAC__
//********  special addresses  *************
#define BROADCAST_ADDR  0xFFFFFFFF
//********  champs type trame *************
#define INTRODUCTION 1
#define PRESENTATION 2
#define CONFLICT_RESOLUTION  3
#define WHO_ARE_MY_NEIGHBOURS 4
#define DATA 5

//********  codage des rôles  *************
#define NOTHING 0
#define SIMPLE_MEMBER 1
#define LINK 2
#define REPRESENTATIVE 3

// ******* constantes diverses **********
#define HEADER1 0xAA
#define HEADER2 0x55
#define MAX_SIZE_MESSAGE 30  // maxi size of message embedded in application PDU
#define APPLI_HEADER_SIZE 9  // size of header of Application Layer 
#define DATA_LINK_ADDRESS_SIZE 4 // in bytes 
#define APPLICATION_ADDRESS_SIZE 4 // in bytes 
#define APPLI_PDU_LENGTH_SIZE 2 // size of the application PDU (= PDU de l'applicative level)
#define MAX_SIZE_NEIGHBOURS_LIST 10 
#define  NO_GROUP  -1
//**  messages relative constants  ***************
#define INTRODUCTION_PDU_MESSAGE_SIZE  0 
#define WHO_ARE_MY_NEIGHBOURS_PDU_MESSAGE_SIZE_WITHOUTGROUP 1  
#define WHO_ARE_MY_NEIGHBOURS_PDU_MESSAGE_SIZE_WITH_ONE_GROUP 5

//******** definition des type ************
typedef struct { unsigned short int appliPDUlength; // not transmitted in appliPDU but in frameLength fiel of data link frame 
				 unsigned long int appSenderId;  // 4 bytes
				 unsigned long int appReceiverId;// 4 bytes
				 unsigned char type;  // 1 byte
				 unsigned char message[MAX_SIZE_MESSAGE];
				} appliPDU_T;	

typedef struct { unsigned long int senderId;  // 4 bytes
				 unsigned long int receiverId; // 4 bytes
				 unsigned short int appliPDUlength; 
				 unsigned short int frameLength; // 2 bytes
				 appliPDU_T appliPDU; // application PDU
				}				
                dataLinkFrame_T;

typedef struct { unsigned long int agentId;
				 unsigned char agentRole;
				 unsigned long int agentGroupId;
				}neighBour_T;                

typedef struct { unsigned long int myId;
				 unsigned char myRole;
				 unsigned long int myGroup;
				 unsigned long nbNeighbours;
				 neighBour_T myNeighbours[MAX_SIZE_NEIGHBOURS_LIST];
				} me_T;              

#endif 