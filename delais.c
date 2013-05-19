/**********************************************************************************
  Fonction delai basee sur la valeur du cycle d'horloge
  On utilise la fonction de la bibliotheque fournie: delai1KTCYx
  Le nombre de secondes désirées est fourni en paramètre. Le valeur retournée
  immédiatement ou au bout du delai demande vaut 1 si tout s'est bien passe ou 0
  si le delai demande n'a pu etre obtenu (trop grand).
  Utilise le predefini OSCI_FREQ qui donne la frequence de microcontroleur
  en MHZ (flottant) et  pourra etre place dans controleur.h qui parametre le type de 
  microcontroleur. Si on veut prendre en compte le mode PLL (multiplication par 4
  en interne de la fréquence d'oscillateur), il faudra definir la constante MODE_PLL 
************************************************************************************/
#include "controleur.h"
#include "delays.h" 
#include "appli.h"
#define _DEF_DELAIS_
#include "delais.h"
 char delaiEnSec(unsigned int nbSec)
 {
   #ifdef MODE_PLL 
     int pll=4;
   #else
     int pll=1;
   #endif
   float nbreTours;
   unsigned int i,fin;
   nbreTours= (float)nbSec*OSCI_FREQ*1.0E3*pll/4.0/100.;  
   /* division par 4 due au fait que un cycle d'instruction=4 périodes d'oscillateur*/ 
   if (nbreTours > 0xFFFF)
       return(FAIL);
   else
   {   fin=(unsigned int)(nbreTours+0.0001); /* problème sur les arrondis
                                                manque 1 tour */
       for(i=0; i<fin;i++)
            Delay1KTCYx(100);
   }
   return SUCCESS;
 }

/**********************************************************************************
  Fonction delai basee sur la valeur du cycle d'horloge
  On utilise la fonction de la bibliotheque fournie: delay1TCY
  Le nombre de millisecondes désirées est fourni en paramètre. 
  Utilise le predefini OSCI_FREQ qui donne la frequence de microcontroleur
  en MHZ (flottant) et  pourra etre place dans controleur.h qui parametre le type de 
  microcontroleur. Si on veut prendre en compte le mode PLL (multiplication par 4
  en interne de la fréquence d'oscillateur), il faudra definir la constante MODE_PLL 
  Le nombre de cycle correspondant à 1 milliseconde est calculé une seule fois lors du 
  appel de la fonction car le temps de calcul en flottant est long et dépasse la milliseconde !!
************************************************************************************/
 void delaiEnMilliSec(unsigned int nbMilliSec)
 {
   static char premierPassage=0;
   static unsigned  int nbToursParMilliSec=0;
   unsigned  int nbTours,i;
   #ifdef MODE_PLL 
     int pll=4;
   #else
     int pll=1;
   #endif
   if (premierPassage==0)
	{
		premierPassage=1;
		nbToursParMilliSec= (unsigned  int)(OSCI_FREQ*2.5*pll); 
    } 
   nbTours= nbToursParMilliSec* nbMilliSec;
   for(i=0;i<nbTours;i++)
     Delay10TCYx(7);
   	

 }
