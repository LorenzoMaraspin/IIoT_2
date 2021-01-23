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

//dichiarazione funzioni utilizzate
void keyPad(void);
void setDisplay(int);
void initPic(void); 
  
const unsigned char keys[] = {1,4,7,'*',2,5,8,0,3,6,9,'#'}; 
const char display[] = {0x3F/*0*/, 0x06/*1*/, 0x5B/*2*/, 0x4F/*3*/, 0x66/*4*/,0x6D/*5*/,0x7D/*6*/,0x07/*7*/,0x7F/*8*/,0x6F/*9*/};

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
char keyPress = 0;
char stop = 0;

int delayBtn = 0, timer = 200, contDisplay = 0;
int cont = 0, cont1 = 0, contDecimi = 0;

void main(void) {
    initPic();
    while(1)
    {
        setDisplay(0);
        if(cont1 > 20) {     
            keyPad();
            cont1 = 0;
        }
    }
    return;
}
void setDisplay(int num) {
    TRISD = 0x00;
    TRISA = 0x00;
    int c = 0;
    int d = 0;
    int u = 0;
    
    //calcolo le centina, decine e unità per la visualizzazione del display
    c = timer / 100;
    d = (timer % 100) / 10;
    u = (timer % 10);
    
    //assegnazione del display da accendere, i display verranno shiftati in base al contatore di display che si incrementa nell'interrupt
    //questo contatore varia da 0, 1, 2
    PORTA = Display1 << contDisplay;
    //se il timer è maggiore di 100 visualizzo le centinaia e decine del timer
    if(timer > 100) {
        //visualizzo sul display 0 le centinaia
        if(contDisplay == 0) {
            PORTD = display[c]; 
        }
        //visualizzo sul display 1 le decine
        else if(contDisplay == 1) {
            PORTD = display[d]; 
        }
    }else {
        //se il timer è minore di 100 (cioè 10 sec) visualizzo nel timer anche i decimi di secondo
        //visualizzo sul display 0 le decine
        if(contDisplay == 0) {
            PORTD = display[d];
            PORTD = PORTD | 0x80;
        }
        //visualizzo sul display 1 le unità
        else if(contDisplay == 1) {
            PORTD = display[u]; 
        }
    }
}
void initPic() {
    //OUTPUT
    TRISB = 0x00;
    TRISD = 0x00;
    //inizializzo l'interrupt in modo che venga richiamato ogni 4ms
    INTCON = 0xA0;
    OPTION_REG = 0x04;
    TMR0 = 6;
}
void keyPad()
{
    TRISD |= 0x0F;
    PORTA = 0x00;
    PORTD = 0xFF;
    
    for(colScan = 0; colScan < 3; colScan++)
    {
        PORTB = PORTB | 0x07;                       // porto tutte le colonne a 1
        PORTB &= colMask[colScan];                  //porto a zero la colonna interessata
        for( rowScan = 0; rowScan <4; rowScan++)
        {
            if((!(PORTD & rowMask[rowScan])) || keyPress)
            {                                       // riga rowScan trovata a massa
                keyPress = 1;
                if((!(PORTD & rowMask[rowScan])) && delayBtn > 1)
                {
                    keypressed = rowScan+(4*colScan);
                    keyok = 1; // è stato premuto un pulsante
                    //se viene premuto il tasto 1 il timer verrà bloccato
                    if(keys[keypressed] == 1) {
                        stop = !stop;
                    }
                    //se il timer è stato bloccato e clicco il pulsante 2 incremento il timer
                    if(keys[keypressed] == 2 && stop == 1){
                        timer = timer + 10;
                    }
                    //se il timer è stato bloccato e clicco il pulsante 3 decremento il timer
                    if(keys[keypressed] == 3 && stop == 1){
                        timer = timer - 10;
                    }
                }
            }
        }
    }
}
void __interrupt() ISR() {
    if(INTCON & 0x04) {
        cont++;
        cont1++;
        contDecimi++;
        //controllo per shiftare ogni 4ms i vari display permettendo di visualizzare correttamente il numero
        if(cont > 1) { 
            contDisplay++;
            if(contDisplay > 1) {
                contDisplay = 0;
            }
            cont = 0;
        }
        //ogni 25 cicli cioè 100ms cioè un secondo vado a decrementare il timer solamente se il timer non si trova in stato di stallo
        if(contDecimi > 25 && stop != 1) {
            //è passato un decimo di secondo
            timer--;
            if(timer <= 0) {
                timer = 0;
            }
            contDecimi = 0;
        }
        //controllo per il bouncing dei tasti del tastierino numerico
        if(keyPress) {
            delayBtn ++;
        }
        INTCON = INTCON & ~0x04;
    }
    return;
}

