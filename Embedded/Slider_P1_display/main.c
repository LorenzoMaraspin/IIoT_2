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

const char display [] = {0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39};

void initADC();
int readADC(char channel);
void init_LCD();
void Send_Command(char);
void Send_Data(char);
void send_string(char *str);
int mapOutput(long);

int i;
int valore, vecchio = -1;

void main(void) {
    TRISE = 0x00;
    TRISD = 0x00;
    TRISB = 0x00;//imposto PORTB come output
    
    
    char str[] = " ";
    
    init_LCD();
    initADC(); //configura convertitore
    Send_Command(L_L1);
    while(1)
    {
        valore = mapOutput(readADC(0));
        if(valore != vecchio) {
            Send_Command(L_CLR);  //pulisco
            str[0] = valore + '0'; //aggiorno
            send_string(str); //mando
            vecchio = valore; //salvo valore
        }

    }
    return;
}

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
    ADCON0 = ADCON0 | (channel << 3);
    /*
     ADCON0 viene settato cosi perche tramite or mi permette di non troncare gli altri bit
     di suo la port AN viene settata sui primi 3 bit ma siccome ADCON setta le porte AN sui bit 5,4,3
     tramite or e lo shift di 3 bit a sinistra li sposto nel loro spazio*/
    __delay_us(20);
    ADCON0 = ADCON0 | 0x04; //avvio la conversione imponendo GO/DONE
    while(!(ADCON0 & 0xFB)){} // --> 0xFB = 0b11111011 oppure si può scrivere ~0x04 --> finche il GO/DONE non torna a 0
    
    return valore = ADRESL +(ADRESH << 8);
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
void send_string(char *str){
    for(int i = 0; str[i] != '\0'; i++) {
        Send_Data(str[i]);
    }
}
//funzione che mappa i valori ricevuti dal potenziometro in modo che visualizzo dei numeri da 0 a 9
int mapOutput(long valore) {   
  return (valore - 0) * (9 - 0) / (1023 - 0) + 0;
}