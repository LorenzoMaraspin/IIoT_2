#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = ON       // Power-up Timer Enable bit (PWRT enabled)
#pragma config BOREN = ON       // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = ON         // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3/PGM pin has PGM function; low-voltage programming enabled)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)


#define _XTAL_FREQ 20000000
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
int mapOutput(long);
void SetDisplay(int,int);
void Send_Char(char);
void Check_Fan(void);
void Check_Heater(void);

long valore,valore1,oldValore, oldValore1;
char c,d,u;
char c1,d1,u1;
char statoHeater = 1, statoFan = 1;
int contHeater = 0, contFan = 0, pwmHeater, pwmFan;


void main(void) {
    initADC(); //configura convertitore
    initPic();
    init_LCD();
    Send_Command(L_L1);

    while(1){
        valore = mapOutput(readADC(0));
        valore1 = mapOutput(readADC(1));
        pwmHeater = (int)valore;
        pwmFan = (int)valore1;
        Check_Heater();
        Check_Fan();
        if((oldValore != valore) || (oldValore1 != valore1)){
            SetDisplay(valore, valore1);
            
            oldValore = valore;
            oldValore1 = valore1;
        }
    }
}
void Check_Heater(){
    //TRISC = 0x00;
    //PORTC = 0x00;
    if(statoHeater){
        PORTC = PORTC | 0x20;
    }else{
        PORTC &= ~0x20;

    }
}

void Check_Fan() {
    //TRISC = 0x00;
    //PORTC = 0x00;
    if(statoFan) {
        //accendere la ventola
        PORTC = PORTC | 0x04;
    }else{
        PORTC &= ~0x04;
    }
}

void initADC()
{
    TRISA = 0xFF; //utilizzo TRISA perche le porte di AN corrispondono a quelle di TRISA
    ADCON0 = 0x81;
    /*
     ADCS = 10
     CHS = channel 0 000
     GO = 0
     ADON = 1
     */
    ADCON1 = 0x85;
    /*
     * ADFM = 1 giustificare a destra
     * ADCS = 0 fosc/32
     * 0
     * 0
     * PCFG = 0000
     */
    __delay_us(20); //attesa di 20 microsecondi per far caricare la prima volta il condensatore Chold
    
}
int readADC  (char channel)
{
    ADCON0 = ADCON0 & ~0x38;
    if(!channel){
            ADCON0 = 0x81;
    }
    else  {ADCON0 = 0x89;}
    ADCON0 = ADCON0 | (channel << 3); 
    __delay_us(20); // attendo che il condensatore Chold si carichi
    ADCON0 |= 0x04;
    while(!(ADCON0 & ~0x04));
    ADCON0 &= ~0x80; // Azzerare la frequenza, da chiedere a Mauro il perchè se no da problemi
    return ADRESL + (ADRESH << 8);
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
void Send_Char(char str){
    Send_Data(str);
}
void initPic() {
    TRISB = 0x00;//imposto PORTB come output
    TRISD = 0x00;
    TRISC = 0x00;
    TRISE = 0x00;
    INTCON = 0xA0;
    OPTION_REG = 0x00; //ogni 4ms
    TMR0 = 6;
    
    //PORTC = 0x20;
}
int mapOutput(long valore) {   
  return (valore - 0) * (100 - 0) / (1023 - 0) + 0;
}
void SetDisplay(int num, int num1) {
    c = (num / 100) + '0';
    d = ((num % 100)/10) + '0';
    u = (num % 10) + '0';
    c1 = (num1 / 100) + '0';
    d1 = ((num1 % 100) / 10) + '0';
    u1 = (num1 % 10) + '0';
    
    Send_Command(L_L1);
    Send_Char('V');
    Send_Char(':');
    Send_Char(' ');
    if(c != '0'){
        Send_Char(c);
    }
    Send_Char(d);
    Send_Char(u);
    Send_Char('%');
    
    Send_Command(L_L2);
    Send_Char('T');
    Send_Char(':');
    Send_Char(' ');
    if(c1 != '0'){
        Send_Char(c1);       
    }
    Send_Char(d1);
    Send_Char(u1);
    Send_Char('%');
}
void __interrupt() ISR() {
    if(INTCON & 0x04) {
        contHeater++;
        contFan++;
        
        if(contHeater > 99) {
            contHeater = 0;
            statoHeater = 1;
        }
        if(contFan > 99) {
            contFan = 0;
            statoFan = 1;
        }
        if(contFan == pwmFan){
            statoFan = 0;
        }
        if(contHeater == pwmHeater){
            statoHeater = 0;
        }
        INTCON = INTCON & ~0x04;
    }
    return;
}