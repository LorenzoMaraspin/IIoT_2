#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = ON       // Power-up Timer Enable bit (PWRT enabled)
#pragma config BOREN = ON       // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = ON         // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3/PGM pin has PGM function; low-voltage programming enabled)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

#include <xc.h>
#define _XTAL_FREQ 8000000
#define Disp 0x20

void initPic();
void showOnDisplay(void);
void keyPad(void);

//array per collegare il numero alla combinazione di bit adeguata per PORTD
const unsigned char keys[] = {1,4,7,'*',2,5,8,0,3,6,9,'#'};  

//numberi in HEX da visualizzare su PORTD
const char display[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66,0x6D,0x7D,0x07,0x7F,0x6F};

//maschere per controllare se è stata cliccato un pulsante
const unsigned char colMask[3] = 
{
    0b11111110,
    0b11111101,
    0b11111011
};
const unsigned char rowMask[4] = 
{
    0b00001000,
    0b00000100,
    0b00000010,
    0b00000001
};

unsigned char colScan = 0;  //contatore delle colonne
unsigned char rowScan = 0;  //contatore delle righe
unsigned char keypressed = 99; //variabile che conterrà il valore numerico premuto
char keyok = 0; //flag di pulsante premuto

void main(void) {

    //initPic();
    TRISB = 0x00;
    TRISA = 0x00;    
    TRISD = 0xFF;
    while(1)
    {
        keyPad();
        showOnDisplay();
        PORTA = 0x20;
        PORTD = display[keys[keypressed]];
        __delay_ms(20);
        
        PORTA = 0x00;
    }
    return;
}
//funzione che serve per individuare se un pulsante del tastierino è stato premuto
void keyPad()
{
    PORTB = 0x00;
    TRISD |= 0x0F;
    
    for(colScan = 0; colScan < 3; colScan++)
    {
        PORTB = PORTB | 0x07;                       // porto tutte le colonne a 1
        PORTB &= colMask[colScan];                  //porto a zero la colonna interessata
        for( rowScan = 0; rowScan <4; rowScan++)
        {
            __delay_ms(1);
            if(!(PORTD & rowMask[rowScan]))
            {                                       // riga rowScan trovata a massa
                __delay_ms(1);
                if(!(PORTD & rowMask[rowScan]))
                {
                    keypressed = rowScan+(4*colScan);
                    keyok = 1;                      // è stato premuto un pulsante
                }
            }
        }
    }
}
void showOnDisplay(void)
{
    TRISD = 0x00;
    TRISA = 0x00;
    
    PORTA = Disp;                                   //assegnazione del display da accendere
    PORTD = display[keypressed];                    // accensione dei vari led da attivare per mostrare in output un numero
    
}
/*void initPic()
{
    INTCON = 0xA0;
    OPTION_REG = 0x05; 
    TMR0 = 6;  // timer a 8 ms
    TRISB = 0x00;
    TRISA = 0x00;    
    TRISD = 0xFF;
}*/

