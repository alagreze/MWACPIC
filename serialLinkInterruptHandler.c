#include "controleur.h"
#include "usart.h"
#include "serial.h"


void serveurUART (void);




//----------------------------------------------------------------------------
// High priority interrupt vector

#pragma code InterruptVectorHigh = 0x08
void
InterruptVectorHigh (void)
{
  _asm
    goto serveurUART //jump to interrupt routine
  _endasm
}


//----------------------------------------------------------------------------
// High priority interrupt routine

#pragma code
#pragma interrupt serveurUART
/**********************************************************
serveur d'interruption de l'UART
place le caractère reçu dans la file FIFO
gere le debordement en remplacant la donnee la plus ancienne
par la plus recente 
**********************************************************/

void serveurUART()
{
  if (PIR1bits.RCIF)
  {
    *gl_fifo.ptEcr=RCREG; /* stocke le caractere dans FIFO */
    gl_fifo.ptEcr=avancePointeur(gl_fifo.ptEcr,&gl_fifo);
    if (gl_fifo.nbCar==TAILLE_BUFFER)
    { /** debordement **/
      gl_fifo.ptLect=avancePointeur(gl_fifo.ptLect,&gl_fifo); /* on avance le pointeur de lecture pour pointer sur
                          le caractere le plus ancien */
      gl_fifo.nbDebord++; // on incremente le compteur de debordement
    }
    else
    {
      gl_fifo.nbCar++;
    }
   }
   /* qui remet  a zero RCIF ??*/
}
