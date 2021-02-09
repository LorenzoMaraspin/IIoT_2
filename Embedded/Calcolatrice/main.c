#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = ON       // Power-up Timer Enable bit (PWRT enabled)
#pragma config BOREN = ON       // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = ON         // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3/PGM pin has PGM function; low-voltage programming enabled)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

#define _XTAL_FREQ 16000000

#define L_ON  0x0F
#define L_OFF 0x08
#define L_CLR 0x01
#define L_L1  0x80
#define L_L2  0xC0
#define L_CR  0x0F
#define L_NCR 0x0C
#define L_CFG 0x38
#define L_CUR 0x0E
#define EN 0x01

#define DEBUG TRISB &= ~0x80; PORTB |= 0x80; __delay_ms(1); PORTB &= ~0x80; __delay_ms(1); 
#include <xc.h>

void init_LCD(void);
void send_command(char);
void send_data(char);
void send_string(char *str);
void initADC();
char keyPad();
int readADC(char channel);
void initPic();
void Check_Key(int);
void Check();
int Map(long result);
void outPutDisplay(char key);

const unsigned char keys[] = {1,4,7,'*',2,5,8,0,3,6,9,'#'};
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

int delayBtn = 0,contDelayButton = 0;
unsigned char colScan = 0;  //contatore delle colonne
unsigned char rowScan = 0;  //contatore delle righe
unsigned char keypressed = 99; //variabile che conterrà il valore numerico premuto
char index;
char fronte[12] = "";
char oldFronte[12] = "";
char lcd[16] = "";
char lcd1[16] = "                ";
char array_oper[5];
char keyok = 0; //flag di pulsante premuto
char keyPress = 0;
char key, oldKey;
int index_oper = 0;
char array_num[5];
int index_num = 0;

void main(void) {
    initPic();
    init_LCD();
    while(1) {
        Check();
    }
    return;
}

void Check() {
    key = keyPad();
    if(key != 99){
        //DEBUG;
        
        outPutDisplay(key);
    }
}
//output del display 
void outPutDisplay(char key){
    DEBUG;
    /*switch(key){
        case '0':
            DEBUG;
            array_num[index_num] = '1';
            index_num++;
            break;
        case '1':
            array_num[index_num] = '4';
            index_num++;
            break;
        case '2':
            array_num[index_num] = '7';
            index_num++;
            break;
        case '3':
            array_oper[index_oper] = '+';
            index_oper++;
            break;
        case '4':
            array_num[index_num] = '2';
            index_num++;
            break;
        case '5':
            array_num[index_num] = '5';
            index_num++;
            break;
        case '6':
            array_num[index_num] = '8';
            index_num++;
            break;
        case '7':
            array_num[index_num] = '0';
            index_num++;
            break;
        case '8':
            array_num[index_num] = '3';
            index_num++;
            break;
        case '9':
            array_num[index_num] = '6';
            index_num++;
            break;
        case '10':
            array_num[index_num] = '9';
            index_num++;
            break;     
        case '11':
            array_oper[index_oper] = '-';
            index_oper++;
            break;
    }*/
        switch(key){
        case 0:
            DEBUG;
            lcd[index_num] = '1';
            index_num++;
            break;
        case '1':
            lcd[index_num] = '4';
            index_num++;
            break;
        case '2':
            lcd[index_num] = '7';
            index_num++;
            break;
        case '3':
            lcd[index_num] = '+';
            index_num++;
            break;
        case '4':
            lcd[index_num] = '2';
            index_num++;
            break;
        case '5':
            lcd[index_num] = '5';
            index_num++;
            break;
        case '6':
            lcd[index_num] = '8';
            index_num++;
            break;
        case '7':
            lcd[index_num] = '0';
            index_num++;
            break;
        case '8':
            lcd[index_num] = '3';
            index_num++;
            break;
        case '9':
            lcd[index_num] = '6';
            index_num++;
            break;
        case '10':
            lcd[index_num] = '9';
            index_num++;
            break;     
        case '11':
            lcd[index_num] = '-';
            index_num++;
            break;
    }
    //send_command(L_L1);
    //send_string(lcd);
}
char keyPad()
{
    TRISD |= 0x0F;
    //PORTA = 0x00;
    //PORTD = 0xFF;
    keypressed = 99;

    for(colScan = 0; colScan < 3; colScan++)
    {
        PORTB = PORTB | 0x07;                       // porto tutte le colonne a 1
        PORTB &= colMask[colScan];                  //porto a zero la colonna interessata
        for( rowScan = 0; rowScan <4; rowScan++)
        {
            index = (rowScan + (4*colScan));
            fronte[index] = !(PORTD & rowMask[rowScan]);
            if(((fronte[index])&&(!(oldFronte[index]))) || keyPress)
            {                                       // riga rowScan trovata a massa
                keyPress = 1;
                if(((fronte[index])&&(!(oldFronte[index]))) && delayBtn > 1)
                {
                    //DEBUG;
                    keypressed = index;
                    delayBtn = 0;
                    keyPress = 0;
                    oldFronte[index] = fronte[index];
                }
                
            }
            if((!(fronte[index]))&&((oldFronte[index]))){
                for (int i = 0; i < 12; i++)
                {
                    oldFronte[i] = 0;
                    fronte[i] = 0; 
                } 
             }
        }
    }
    return keypressed;
}
int Map(long result) {
    if(result < 512){
        return 0;
    }
    if(result > 511){
        return 1;
    }
}
void initPic() {
    //TRISA = 0xFF;
    TRISB = 0x00;
    TRISD = 0x00;
    TRISE = 0x00;
    //inizializzo l'interrupt in modo che venga richiamato ogni 4ms
    INTCON = 0xA0;
    OPTION_REG = 0x04;
    TMR0 = 6;
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
void init_LCD()
{
    PORTE = PORTE & 0x02; //enable flag = 1
    PORTE = PORTE & 0x04; //rs flag = 0
    __delay_ms(20);
    PORTE = PORTE | 0x02;
    send_command(L_CFG);
    __delay_ms(5);
    send_command(L_CFG);
    __delay_ms(1);
    send_command(L_CFG);
    send_command(L_ON);
    send_command(L_CLR);
    send_command(L_CUR);
    send_command(L_L1);
}
void send_command(char command) {
    TRISD = 0x00;
    PORTE = PORTE | 0x02; //RE1 0b00000010
    PORTD = command;
    PORTE = PORTE & ~0x04; 
    __delay_ms(3);
    PORTE = PORTE & ~0x02; 
    __delay_ms(3);
    PORTE = PORTE | 0x02;
}
void send_data(char data)
{
    TRISD = 0x00;
    PORTE = PORTE | 0x02;
    PORTD = data;
    PORTE  = PORTE | 0x04;
    __delay_ms(3);
    PORTE = PORTE & ~0x02;
    __delay_ms(3);
    PORTE = PORTE | 0x02;
}
void send_string(char *str){
    TRISD = 0x00;
    for(int i = 0; str[i] != '\0'; i++) {
        send_data(str[i]);
    }
}
void __interrupt() ISR() {
    if(INTCON & 0x04) {
        //contDelayButton++;
        if(keyPress) {
            delayBtn ++;
        }
        INTCON = INTCON & ~0x04;
    }
    return;
}