/*convertit un float en chaine de caracteres avec nombre de décimales configurable*/
/** la longueur de la chaine sur laquelel on passe un pointeur en entree devra etre
    correctement defini en fonction de l'ordre de grandeur du reel. Aucun verification de longueur
     n est faite **/
/** OK teste mais encore pb d'arrondi dus certainement a la pietre
    qualite de la bibliotheque flottante **/
#define NBREDEC 2 /* nombre de decimales mesurees */
#include <string.h>
#include <stdlib.h>
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
