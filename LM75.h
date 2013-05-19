/**************************************************************************************************/
/*Module: LM75.h 
A LAGREZE  5/11/04
* Déclarations associées au thermomètre I2C LM75
* Etat: OK
***************************************************************************************************/
#ifndef __LM75_H
#define __LM75_H
/************* type *************/
typedef enum {NOERROR , ERRCOLLISION , ERRNOTACK} erreurI2C_E;
/******   constantes *********************/
#define DEBIT400 9
#define DEBIT100 39
#define ACCESTEMPE 0
#define ACCESCONFIG 1
#define ACCESHYSTBAS 2
#define ACCESHYSTHAUT 3
#define MASKRETOURLECT 0xFF00
#define BITRWb 1
#define S2 PORTAbits.RA4
#define S3 PORTBbits.RB0
#define DEUX_PUISS_7 0x80
#define HUIT_BIT_POIDSFORT 0xFF00
#define TC74CARTE 0x9A
/************* fonctions *************/
#ifdef _DEF_LM75_
  #define PUBLIC_LM75 
#else
  #define PUBLIC_LM75 extern
#endif 
PUBLIC_LM75  void initI2C(unsigned char debit,unsigned char type, unsigned char slew);
PUBLIC_LM75  erreurI2C_E getTempe(unsigned char adrI2C, float *tempe);
PUBLIC_LM75 void getTempeAsString(char *tempeString, unsigned char addrI2cSensor);
PUBLIC_LM75 void ftoaFaitMaison(float f , char chaineRes[] );

#endif 
