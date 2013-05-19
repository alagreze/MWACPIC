/***********************************************************************************/
/** Programmeur(s): A. LAGREZE
    Date cr�ation: MAi 2005
	Date derni�re modif: Mai 2013
    Version: 2.0
    Teste de l'appui des boutons de la carte PICDemplus et de la
    carte fournie aux �tudiants
    Etat: en cours
*********************************************************************************/
/************* fichiers inclus ******************************/
/****************************************************************/
#include <p18f458.h>
#define _DEF_BOUTONS_
#include "boutons.h"
#include "delais.h"

/***********************************************************
 fonction renvoie TRUE(1) ou FALSE(0)  suivant que le bouton
 dont le num�ro est pass� en param�tre (2 ou 3) est enfonc� ou pas
 si le bouton est d�tect� appuy�, on attendra 40 ms avant de 
 sortir de la fonction pour laisser s'amortir les rebonds
***************************************************************/

char bouton(char numBouton)
{ char premPassage=0;
  if ( premPassage== 0)
  {
    /*on initialise les ports associ�s aux boutons en entree */
    TRISA= TRISA|MASK_S2;
	TRISB= TRISB|MASK_S3;
    premPassage=1;
  }
  switch(numBouton)
  {
    case BoutonS2: 
       	if (S2==0)
          {
            delaiEnMilliSec(40);
			return (TRUE);
          }
        else return(FALSE);
        break;
	 case BoutonS3: 
       	if (S3==0)
          {
            delaiEnMilliSec(40);
			return (TRUE);
          }
         else return(FALSE);
        break;
     default:
        return(FALSE); 
   }
}
