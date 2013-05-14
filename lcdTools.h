//Pilote pour afficheur LCD type Hitachi HD44780
//pour PICDEM2PLUS 
//
//  (R Toquebeuf le 08-01-03 ; modifications de F Bouchy septembre 2004)
//
//  Fonctions exploitables par le programme utilisateur :
//
//  initLCD(): initialise l'affichage
//	clearLCD() : efface l'affichage
//  afficheLCD(col,lig,message): affiche une ligne sur le LCD
//	lcd_data(unsigned char c) : écriture d'un caractère
//	lcd_puts(unsigned char *s): écriture d'une chaîne de caractères 
//	lcd_goto(unsigned char x, unsigned char y) : positionne le curseur en cl( 1 à 16) et l (1 ou 2)

#ifndef __LCDTOOLS__
#define __LCDTOOLS__
#include	"controleur.h"


// Données : 4 bits de poids faible du port D 
#define LCD_DATA	LATD
#define LCD_DATA_TRIS	TRISD

// Définition des lignes de contrôle
#define LCD_RW		LATAbits.LATA2   //  RW  
#define LCD_RS		LATAbits.LATA3   //  RS 
#define LCD_EN		LATAbits.LATA1   //  E 

#define LCD_RW_TRIS	DDRAbits.RA2    // Direction RW 
#define LCD_RS_TRIS	DDRAbits.RA3    // Direction RS 
#define LCD_EN_TRIS	DDRAbits.RA1    // Direction E  

#define OUTPUT          0x0
#define INPUT           0x1


//  Affichage ligne
// colonne de 1 a 16 , ligne de 1 à 2
// retourne 0 si OK, 1 si message tronqué,  2 si paramètre out of range
int afficheLCD (char col, char lig ,char *message); 
    
//Temporisations :

// Delai 1 µs sur clock 4MHZ 
void DelayMUs(unsigned char x);

// Delai 1 Ms sur clock 4MHZ 
void DelayMs(unsigned char cnt);


//LCD :

// Validation lecture/écriture
void LCD_STROBE(void); 

// Ecriture commande
void lcd_cmd(char c);

// Ecriture donnée
void lcd_data(char c);

// Ecriture d'une chaîne de caractères 
void lcd_puts(char * s);

// Ecriture de n caractères d'une chaîne de caractères 
void lcd_nputs(unsigned char n, char * s);

// efface l'afficheur
void clearLCD(void);

// Positionne le curseur : x position sur la ligne ( 1 à 16), y ligne (1 ou 2)
void lcd_goto(unsigned char cl, char l);

// Initialisation de l'afficheur
void initLCD(void);

#endif
/*******************************************************************/
