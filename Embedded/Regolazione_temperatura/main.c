#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = ON       // Power-up Timer Enable bit (PWRT enabled)
#pragma config BOREN = ON       // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = ON         // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3/PGM pin has PGM function; low-voltage programming enabled)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)


#define _XTAL_FREQ 8000000
#include <xc.h>
#define L_ON  0x0F
#define L_OFF 0x08
#define L_CLR 0x01
#define L_L1  0x80
#define L_L2  0xC0
#define L_CR  0x0F
#define L_NCR 0x0C
#define L_CFG 0x38
#define L_CUR 0x0E

void initADC();
void initPic();
int readADC(char channel);
void init_LCD();
void Send_Command(char);
void Send_Data(char);
void send_char(char);
void send_string(char *str);
void checkTemp();



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
int i;
char m;
char c;
char d;
char u;
int valore= -1, vecchio = -1,temp1, tempDes = 35;
int cont1 = 0;
double volt,temp;

void main(void) {
    TRISE = 0x00;
    TRISD = 0x00;
    TRISB = 0x00;//imposto PORTB come output
    
    
    char str[] = "Des:    C";
    initPic();
    init_LCD();
    initADC();
    Send_Command(L_L1);
    while(1)
    {
        valore = readADC(2);
        /*
        if(cont1 > 10) {     
            keyPad();
            cont1 = 0;
        }*/
        if(vecchio != valore){
            Send_Command(L_CLR);
            Send_Command(L_L1);
            volt = valore * 4.88281;
            temp = volt / 10;
            temp1 = temp * 100;
            
            m = (temp1 / 1000) + '0';
            c = ((temp1 % 1000) / 100) + '0';
            send_string("Temp: ");
            send_char(m);
            send_char(c);            
            send_char(' ');
            send_char('C');            

            
            Send_Command(L_L2);
            
            d = (tempDes / 10);
            c = (tempDes % 10);
            
            str[5] = d + '0';
            str[6] = c + '0';
            
            send_string(str);
            
            checkTemp();
            //keyPad();
            
            vecchio = valore;
        }
    }
    return;
}
void keyPad()
{
    //PORTB = 0x00;
    //TRISA = 0x00;
    TRISD |= 0x0F;
    PORTA = 0x00;
    PORTD = 0xFF;
    
    for(colScan = 0; colScan < 3; colScan++)
    {
        PORTB = PORTB | 0x07;                       // porto tutte le colonne a 1
        PORTB &= colMask[colScan];                  //porto a zero la colonna interessata
        for( rowScan = 0; rowScan <4; rowScan++)
        {
            if(!(PORTD & rowMask[rowScan]))
            {
                __delay_ms(1);
                if((!(PORTD & rowMask[rowScan])))
                {
                    keypressed = rowScan+(4*colScan);
                    keyok = 1;
                    // è stato premuto un pulsante
/*
                    if(keys[keypressed] == 1) {
                        tempDes++;
                    }
                    if(keys[keypressed] == 2) {
                        tempDes--;
                    }*/
                }
            }
        }
    }
}
void checkTemp() {
    int tempCopy = (int)temp;

    if(tempCopy < tempDes) {
        PORTC = 0x20;
    }
    if(tempCopy >= tempDes) {
        PORTC = 0x00;
    }
}
void initADC()
{
    TRISA = 0xFF; //utilizzo TRISA perche le porte di AN corrispondono a quelle di TRISA
    ADCON0 = 0x91;
    /*
     ADCS = 10
     CHS = channel 2 010
     GO = 0
     ADON = 1
     */
    ADCON1 = 0x8A;
    /*
     * ADFM = 1 giustificare a destra
     * ADCS = 0 fosc/32
     * 0
     * 0
     * PCFG = 0000
     */
    __delay_us(20); //attesa di 20 microsecondi per far caricare la prima volta il condensatore Chold
    
}
int readADC(char channel)
{
    TRISA = TRISA | 0x04;
    ADCON0 = ADCON0 | (channel << 3);
    /*
     ADCON0 viene settato cosi perche tramite or mi permette di non troncare gli altri bit
     di suo la port AN viene settata sui primi 3 bit ma siccome ADCON setta le porte AN sui bit 5,4,3
     tramite or e lo shift di 3 bit a sinistra li sposto nel loro spazio*/
    //__delay_us(20);
    ADCON0 = ADCON0 | 0x04; //avvio la conversione imponendo GO/DONE
    while(!(ADCON0 & ~0x04)); // --> 0xFB = 0b11111011 oppure si può scrivere ~0x04 --> finche il GO/DONE non torna a 0
    
    return ADRESL +(ADRESH << 8);
}
void init_LCD()
{
    PORTE = PORTE & 0x02;
    PORTE = PORTE & 0x04;
    __delay_ms(20);
    PORTE = PORTE | 0x02;
    Send_Command(L_CFG);
    __delay_ms(5);
    Send_Command(L_CFG);
    __delay_ms(1);
    Send_Command(L_CFG);
    Send_Command(L_ON);
    Send_Command(L_CLR);
    Send_Command(L_CUR);
    Send_Command(L_L1);
}

void Send_Command(char command)
{
    PORTE = PORTE | 0x02; //RE1 0b00000010
    PORTD = command;
    PORTE = PORTE & ~0x04; 
    __delay_ms(3);
    PORTE = PORTE & ~0x02; 
    __delay_ms(3);
    PORTE = PORTE | 0x02;
}

void Send_Data(char data)
{
    PORTE = PORTE | 0x02;
    PORTD = data;
    PORTE  = PORTE | 0x04;
    __delay_ms(3);
     PORTE = PORTE & ~0x02;
    __delay_ms(3);
    PORTE = PORTE | 0x02;    
}
void send_char(char str){
    Send_Data(str);
}
void initPic() {
    TRISB = 0x00;//imposto PORTB come output
    TRISD = 0x00;
    TRISC = 0x00;
    TRISE = 0x00;
    INTCON = 0xA0;
    OPTION_REG = 0x04;          //ogni 4ms
    TMR0 = 6;
    
    //PORTC = 0x20;
}
void send_string(char *str){
    for(int i = 0; str[i] != '\0'; i++) {
        Send_Data(str[i]);
    }
}
void __interrupt() ISR() {
    if(INTCON & 0x04) {
        cont1++;
        INTCON = INTCON & ~0x04;
    }
    return;
}
