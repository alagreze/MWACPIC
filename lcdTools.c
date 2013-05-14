// fichier code source du pilote lcdTools.c pour le LCD de la carte PICDEM2+
//  Programmeurs: Florent Bouchy � partir code r�cup�r� sur Internet Sep 2004
//                A. Lagreze. Rajout d'un fonction permettant directement l'affichage d'une chaine
//                sans positionnement de curseur pr�alable
// Etat: test�. Si probleme revoir tempo qui ont �t� modifi�es
#include <string.h>
#include "lcdTools.h" 

/***********************************************************************************
 Fonction d'affichage d'une ligne.
 Entr�e:  col: colonne de d�but chaine (1 � 16)
          lig: ligne d�but de chaine  (1 ou 2)
          mes:  pointeur sur la chaine � afficher.
 Retour:  0 si affichage s'est bien pass�, 1 si chaine tronqu�e, 2 si param�tres non valides
 ************************************************************************************/
#define LONGLIG 16
#define NBLIG 2
int afficheLCD(char col, char lig, char *mes)
{ int n;
  if ((col > LONGLIG)|| (col<0) || (lig > NBLIG) || (lig <0))
    return(2);
  else
  {
     if (strlen(mes)+col > (LONGLIG+1))
        n= LONGLIG-col+1;
     else
        n=strlen(mes);
    lcd_goto( col,lig);
    lcd_nputs(n,mes);
    if (n==(LONGLIG-col+1)) return (1);
    else return (0);
  }
}
 

//Temporisations 

// Delai 1 �s sur clock 4MHZ 
void DelayMUs(unsigned char x)	
{  
	while(x--)
	{			
	Nop();
	};		
}


// Delai 1 Ms sur clock 4MHZ 
void DelayMs(unsigned char cnt)
{
	unsigned char i;
	while (cnt--)
	 {
		i=4;
		while(i--)
		 {
			DelayMUs(250);	
	 	 } ;
	} ;
} 


// Validation lecture/�criture

void LCD_STROBE(void)    	
{
		LCD_EN = 1; 
		Nop(); 
		Nop(); 
		LCD_EN = 0;
}


// Ecriture commande

void lcd_cmd( char c)
{
	    DelayMs(1); /** 10 avant modif par AL */
	    LCD_DATA = 0; 
		LCD_DATA |= ( ( c >> 4 ) & 0x0F );
		LCD_STROBE();
		LCD_DATA &= 0xF0;
    	LCD_DATA |= ( c & 0x0F );
		LCD_STROBE();   
}


// Ecriture donn�e

void lcd_data(char c)
{
	DelayMs(1); /* 20 avant modif par AL */
	LCD_DATA = 0; 
	
	LCD_RS = 1; 						// mode donn�e
	DelayMUs(1);
	LCD_DATA |= ( ( c >> 4 ) & 0x0F );	//Ecriture quartet de poids fort     
	LCD_STROBE();
	
	LCD_DATA &= 0xF0;					//Ecriture quartet de poids faible
	LCD_DATA |= ( c & 0x0F ); 
	LCD_STROBE();
	
	LCD_RS = 0; 						// retour en mode commande

}


// Ecriture d'une cha�ne de caract�res 

void lcd_puts(char * s)
{
	while(*s)
		lcd_data(*s++);
}
// Ecriture de n caract�re d'une  cha�ne de caract�res sur le LCD

void lcd_nputs(unsigned char n, char * s)
{
	while((*s) && (n!=0))
    {
	  n--;	
      lcd_data(*s++);
    }
}

// efface l'afficheur
void clearLCD(void)
{
    lcd_cmd(0x1);		
}


// Positionne le curseur : cl position sur la ligne ( 1 � 16), l ligne (1 ou 2)

void lcd_goto(unsigned char cl, char l)	
{
	if (l==2) 
	lcd_cmd((cl+0x40+0x80));
	else if (l==1)
	lcd_cmd((cl + 0x80));	
}


// Initialisation de l'afficheur

void initLCD(void)
{
	// S�quence de d�marrage de l'afficheur (cf doc constructeur)

	LCD_RW_TRIS	  = OUTPUT;	
	LCD_RS_TRIS	  = OUTPUT;
	LCD_EN_TRIS	  = OUTPUT;
	LCD_DATA_TRIS = OUTPUT;
	
	LCD_RW = 0;
	
	DelayMs(15);		
	
	LCD_DATA	 = 0x3;
	LCD_STROBE();
	DelayMs(5);		
	
	LCD_DATA	 = 0x3;
	LCD_STROBE();
    DelayMUs(250);	
    
	LCD_DATA	 = 0x3;
	LCD_STROBE();
	DelayMUs(250);	

	LCD_DATA	 = 0x2; 
	LCD_STROBE();
	DelayMUs(250);
	// Fin de la s�quence de d�marrage
						
						
						// Initialisation
						
	lcd_cmd(0x28);		// Fonctionnement : 4 bits ,2 lignes	
	lcd_cmd(0xF);		// Affichage en fonction, Curseur visible, Clignotement du curseur
	clearLCD();		    // efface l'affichage
	lcd_cmd(0x6);		// D�placement du curseur vers la droite
	lcd_goto(0,1);		// Initialise DDRAM addresse � z�ro
}


