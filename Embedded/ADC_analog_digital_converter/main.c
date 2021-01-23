#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = ON       // Power-up Timer Enable bit (PWRT enabled)
#pragma config BOREN = ON       // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = ON         // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3/PGM pin has PGM function; low-voltage programming enabled)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

#define _XTAL_FREQ 8000000
#define Display1 0x04
#include <xc.h>

const char display[] = {0x3F/*0*/, 0x06/*1*/, 0x5B/*2*/, 0x4F/*3*/, 0x66/*4*/,0x6D/*5*/,0x7D/*6*/,0x07/*7*/,0x7F/*8*/,0x6F/*9*/};

void initADC();
int readADC(char channel);
void setDisplay(int);
void initPic(void);
int mapOutput(long);


long valore;
int contDisplay = 0;
int cont = 0, cont1 = 0;

void main(void) {
    
    initADC(); //configura convertitore
    initPic();
    while(1)
    {
        if(cont1 > 15){
            valore = mapOutput(readADC(0));//partiamo dal canale AN0
            cont1 = 0;
        }
        //valore = mapOutput(readADC(0));
        setDisplay(valore);
        //PORTB = valore; // visualizzo il valore su portB
        //PORTD = valore << 8;
    }
    return;
}
//variabile = variabile | 0x01; cosi facendo io seleziono solo il bit 0 
//evitando di troncare gli altri bit che potranno esssere riutlizzati


//variabile = variabile & 0x01 mi permette di resettare il bit 0
void initADC()
{
    TRISA = 0xFF; //utilizzo TRISA perche le porte di AN corrispondono a quelle di TRISA
    ADCON0 = 0x81;
    /*
     ADCS = 10
     CHS = channel
     GO = 0
     ADON = 1
     */
    ADCON1 = 0x8E;
    /*
     * ADFM = 1 giustificare a destra
     * ADCS = 0 fosc/32
     * 0
     * 0
     * PCFG = 1110
     */
    __delay_us(20); //attesa di 20 microsecondi per far caricare la prima volta il condensatore Chold
    
}
int readADC(char channel)
{
    TRISA = 0xFF;
    ADCON0 = ADCON0 | (channel << 3);
    /*
     ADCON0 viene settato cosi perche tramite or mi permette di non troncare gli altri bit
     di suo la port AN viene settata sui primi 3 bit ma siccome ADCON setta le porte AN sui bit 5,4,3
     tramite or e lo shift di 3 bit a sinistra li sposto nel loro spazio*/
    __delay_us(20);
    ADCON0 = ADCON0 | 0x04; //avvio la conversione imponendo GO/DONE
    while(!(ADCON0 & 0xFB)); // --> 0xFB = 0b11111011 oppure si può scrivere ~0x04 --> finche il GO/DONE non torna a 0
    
    return ADRESL +(ADRESH << 8);
}
void setDisplay(int num) {
    TRISD = 0x00;
    TRISA = 0x00;
    int m = 0;
    int c = 0;
    int d = 0;
    int u = 0;
    
    m = num / 1000;
    c = (num % 1000) / 100;
    d = ((num % 1000)%100) /10;
    u = (num % 10);
    
    PORTA = Display1 << contDisplay;
    
    if(contDisplay == 0) {
        PORTD = display[m]; 
    }else if(contDisplay == 1) {
        PORTD = display[c]; 
    }else if(contDisplay == 2) {
        PORTD = display[d];
    }else if(contDisplay == 3) {
        PORTD = display[u];
    }
}
void initPic() {
    TRISB = 0x00;//imposto PORTB come output
    TRISD = 0x00;
    INTCON = 0xA0;
    OPTION_REG = 0x04; 
    TMR0 = 6;
}
void __interrupt() ISR() {
    if(INTCON & 0x04) {
        cont++;
        cont1++;
        if(cont > 1) { 
            contDisplay++;
            if(contDisplay > 3) {
                contDisplay = 0;
            }
            cont = 0;
        }
        INTCON = INTCON & ~0x04;
    }
    return;
}
int mapOutput(long valore) {   
  return (valore - 0) * (100 - 0) / (1023 - 0) + 0;
}