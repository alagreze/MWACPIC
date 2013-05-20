/**************************************************************************************************/
/*Module: LM75.c 
A LAGREZE  5/11/04
* Routines associees au thermomètre I2C LM75 ou TC74 
* Etat: OK testé
***************************************************************************************************/
#include <i2c.h>
#include "controleur.h"
#define _DEF_LM75_
#include "LM75.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
/************* variables globales  **********************/
static char gl_partDec[4]= ".5";
static char gl_messErr[]="ERR SENSOR";
/**************************************************************************************************/
/*** Initialisation du bus I2C   
     Utilise la routine OpenI2C fourni en rajoutant le debit et la config des lignes SCL et SDA ***/
/**************************************************************************************************/
void initI2C(unsigned char debit,unsigned char type, unsigned char slew)
{
  OpenI2C(type, slew);// Initialize I2C module
  SSPADD = debit ; //400Khz Baud clock(9) @16MHz
                   //100khz Baud clock(39) @16MHz
}

/***********************************************************************************************************/
/*** lecture d'un des trois registres  (Temp,TOS, THYS)  du LM75  ***/
/*** entree: adrI2C: adresse I2C du LM75 est passée sur 8 bits (calée à gauche)
             reg: registre qui doit etre lu ,  vaut ACCESTEMPE ACCESHYSTBAS(Thyst) ACCESHYSTHAUT(Tos)
     sortie: entier 16 bits contReg contient les 9 bits du registre lu recalés sur les poids faibles          
     valeur renvoyée :  valeur de l'erraur pour signaler une erreur bus avec les prédéfinis enumérés dans LM75.h
/**************************************************************************************************************/
 erreurI2C_E getRegistreLM75(unsigned char adrI2C, unsigned char reg,  unsigned int * contReg)
{ 
   IdleI2C();                      // ensure module is idle
   StartI2C();                     // initiate START condition
//*********************************************************************
   /*****  emet octet de controle (adresse+ RW)   ***/
//*********************************************************************
   while ( SSPCON2bits.SEN );      // wait until start condition is over 
   if ( PIR2bits.BCLIF )           // test for bus collision
   {
      PIR2bits.BCLIF=0; /* rajoute car bug. voir doc sur BCLIF */
      return (ERRCOLLISION );                // return with Bus Collision error 
   }
   else
   {
      if ( WriteI2C( adrI2C&(~BITRWb)))    // le bit R/W est mis à 0(Write)
      {
        return (ERRCOLLISION );              // return with write collision error
      }

      IdleI2C();                    // ensure module is idle
      //*********************************************************************
      /**  emet l'octet de pointeur pour accéder au registre souhaite ***/ 
      //*********************************************************************
      if ( !SSPCON2bits.ACKSTAT )   // test for ACK condition, if received
      {
        if ( WriteI2C(reg ))  // ecrit pointeur de registre 
        {
          return (ERRCOLLISION );            // return with write collision error
        }
        IdleI2C();                  // ensure module is idle
        //*********************************************************************
        /**  emet Repeated Start puis adresse avec R/W à 1 (read)       ***/ 
        //*********************************************************************
        if ( !SSPCON2bits.ACKSTAT ) // test for ACK condition, if received
        {
          RestartI2C();             // generate I2C bus restart condition
          while ( SSPCON2bits.RSEN );// wait until re-start condition is over 
          if ( PIR2bits.BCLIF )     // test for bus collision
          {
             PIR2bits.BCLIF=0; /* rajoute car bug. voir doc sur BCLIF */
             return (ERRCOLLISION);          // return with Bus Collision error 
           }
                          
           if ( WriteI2C(adrI2C|BITRWb) )// mise à 1 du bit R/W (Read)
           {
            return (ERRCOLLISION);          // return with write collision error
           }
           IdleI2C();                // ensure module is idle
           if ( !SSPCON2bits.ACKSTAT )// test for ACK condition, if received
           {
             //*********************************************************************
             /**  recoit octet de poids fort contenu registre                  ***/ 
             //*********************************************************************
             SSPCON2bits.RCEN = 1;       // enable master for 1 byte reception
             while ( SSPCON2bits.RCEN ); // check that receive sequence is over
             *contReg=SSPBUF;
             *contReg=((*contReg)<<8); // recupere l'octet poids fort registre 
             AckI2C();              // send ACK condition
             while ( SSPCON2bits.ACKEN ); // wait until ACK sequence is over 
            //*********************************************************************
            /**  recoit octet de poids faible contenu registre                  ***/ 
            //*********************************************************************
            SSPCON2bits.RCEN = 1; // enable master for 1 byte reception
            while ( SSPCON2bits.RCEN ); // check that receive sequence is over
            *contReg=(*contReg | SSPBUF)>>7; // recupere poids faible et remet en position de base
            NotAckI2C();              // send no ACK condition
            while ( SSPCON2bits.ACKEN ); // wait until no ACK sequence is over 
	        StopI2C();              // send STOP condition
            while ( SSPCON2bits.PEN ); // wait until stop condition is over 
            if ( PIR2bits.BCLIF )   // test for bus collision
            {
              PIR2bits.BCLIF=0; /* rajoute car bug. voir doc sur BCLIF */
              return (ERRCOLLISION);         // return with Bus Collision error 
            }
           }
           else
           {
             return (ERRNOTACK );          // return with Not Ack error
           }
      }
      else
      {
         return (ERRNOTACK );            // return with Not Ack error
      }
     }
     else
     {
        return (ERRNOTACK);              // return with Not Ack error
     }
  }
  return ( NOERROR );   
}

/***********************************************************************************************************/
/*** lecture du LM75 et retourne de la temperature sous forme d'un float
/*** entree: adrI2C: adresse I2C du LM75 est passée sur 8 bits (7 bits adresses calés à gauche)
     paramètre résultat:  température sous forme d'un float            
     valeur renvoyée :  valeur de l'erreur pour signaler une erreur bus avec les prédéfinis enumérés dans LM75.h
/**************************************************************************************************************/
erreurI2C_E getTempe(unsigned char adrI2C, float *tempe)
{
 
  signed int regTempe; 
  erreurI2C_E err;
  char dec;
  
  /* lecture registre temperature */
  if  ((err=getRegistreLM75(adrI2C, ACCESTEMPE, &regTempe))== NOERROR) 
  {  /* si pas d'erreur bus I2C*/
      dec=regTempe&1; /* recupere le chiffre decimal */
      regTempe=regTempe>>1; /* recupere partie entière de la température */
	   if (regTempe >= DEUX_PUISS_7 ) 
      {
         regTempe=regTempe|HUIT_BIT_POIDSFORT; /*extension de signe sur 16 bits*/ 
      }
      *tempe=(float)regTempe;
	  if (dec!=0)/* si partie décimale, on rajoute ".5" à la chaine */
        *tempe=*tempe+0.5;   
      return (NOERROR);
  }
else
   {
     return (err);
   }   
} 
// ***************************************************************************************
// ** Renvoie sous forme de chaine la température  lue sur le capteur de température 
// ** dont l'adresse I2C est apssée en paramètre. renvoie "ERR SENSOR" si pb lecture I2C 
// ***************************************************************************************
void getTempeAsString(char *tempeString, unsigned char addrI2cSensor)
{
  static char premPassage=0;
  float tempe;
  erreurI2C_E ret;
  if (premPassage==0)
  { premPassage=1;
    initI2C(DEBIT400,MASTER,SLEW_ON); /* initialise le bus I2C au premier passage */
  }
  ret=getTempe(addrI2cSensor,&tempe);
  if (ret!=NOERROR)
	strcpy(tempeString,gl_messErr);
  else 
  {
	ftoaFaitMaison(tempe,tempeString);
	sprintf(tempeString,"%s°C",tempeString);
  }
 }
  
  
// ***************************************************************************************  
// *convertit un float en chaine de caracteres avec nombre de décimales configurable                      
// ***************************************************************************************
// la longueur de la chaine sur laquelel on passe un pointeur en entree devra etre
//  correctement defini en fonction de l'ordre de grandeur du reel. Aucun verification de longueur
//     n est faite **/
//  OK teste mais encore pb d'arrondi dus certainement a la pietre
//    qualite de la bibliotheque flottante **/
// ***************************************************************************************
#define NBREDEC 1 /* nombre de decimales mesurees */

void ftoaFaitMaison(float f , char chaineRes[] )
{
  char i,fin;
  int iF;
  int chifDec; /* chiffre decimal */
  iF=(int)f;
  itoa(iF,chaineRes);
  fin=strlen(chaineRes);
  chaineRes[fin++]='.';
  if (f >0)  f=f+0.00005; 
  else f=f-0.0005; /* pour problèmes d'arrondi */
 
  for (i=0;i<NBREDEC;i++)
  { 
     f=f*10.0;
     chifDec=(int)(f)%10;
     if (chifDec<0) chifDec=-chifDec;
     chaineRes[fin++]=chifDec+'0';
  }
  chaineRes[fin]='\0'; /* fin de chaine */
   
}
	
	
	