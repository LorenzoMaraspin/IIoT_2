#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = ON       // Power-up Timer Enable bit (PWRT enabled)
#pragma config BOREN = ON       // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = ON         // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3/PGM pin has PGM function; low-voltage programming enabled)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)


#define _XTAL_FREQ 16000000
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
//dichiarazione funzioni utilizzate
void keyPad(void);
void initPic(void);
void init_LCD(void);
void send_command(char);
void send_data(char);
void send_string(char *str);
void send_char(char);
void initADC();
void buttonRb2();
int mapOutput(long valore);
int readADC(char channel);
char checkPin();
  
const unsigned char keys[] = {1,4,7,'*',2,5,8,0,3,6,9,'#'};
const unsigned char display[] = {0x31, 0x34, 0x37, 0x2A, 0x32, 0x35, 0x38, 0x30, 0x33, 0x36, 0x39, 0x23};

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
char secret[] = "1234", valori[]= "    ";
unsigned char colScan = 0;  //contatore delle colonne
unsigned char rowScan = 0;  //contatore delle righe
unsigned char keypressed = 99; //variabile che conterrà il valore numerico premuto
char keyok = 0; //flag di pulsante premuto
char keyPress = 0;
char value,oldValue;
int valore, oldValore, valoreDes = 3, delayBtn = 0;
char flag = 0, test, flagCmp[4] = "";
int cont = 0, cont1 = 0,i, k = 0, res;

void main(void) {
    //INIZIALIZZO I VARI REGISTRI CHE DEVO UTILIZZARE PER IL PROGETTO
    //initADC();
    initPic();
    init_LCD();
    send_command(L_L1);
    while(1){
        //CONTROLLO SE UN TASTO DEL KEYPAD è STATO CLICCATO O MENO OGNI TOT ms
        if(cont1 > 15) { 
            keyPad();
            cont1 = 0;
        }
        
        //controllo per vedere se il keypad è stato cliccato
        if((keypressed != 99)&&(cont <= 3)){
            //send_command(L_L1);
            value = keys[keypressed] + '0';
            if(value != oldValue){
                valori[cont] = value;
                cont++;
                oldValue = value;
                send_char('*');
            }
        }
        //SE L'ARRAY DI NUMERI è STATO RIEMPITO CORRETTAMENTE SETTO QUESTO FLAG AD 1 PER VISUALIZZARE SUL DISPOSITIVO 
        if(cont > 3){
            flag = 1;
        }
        //valore = mapOutput(readADC(0));
        if(flag == 1){
            send_char(' ');
            send_char(' ');
            send_char('P');
            send_char(valoreDes + '0');
            flag = 0;
            cont = 0;
        }
    /*
        if(i == 4) {
            
        
            if(valore != oldValore){
                send_command(L_L2);
                test = valore + '0';
                send_char(test);
                oldValore = valore;
            }   
        }*/
        /*
        res = checkPin();
        if((res) && (valore == valoreDes)){
            send_command(L_CLR);
            send_command(L_L1);
            send_char('A');


        }
     */}
    

    return;
}

int mapOutput(long valore) {   
  return (valore - 0) * (7 - 0) / (1023 - 0) + 0;
}
char checkPin() {
    for(int i = 0; i < 4; i++){
        if(secret[i] == valori[i]){
            flagCmp[i] = 1;
            k++;
        }
    }
    if(k == 3){
        return 1;
    }
    return 0;
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
void buttonRb2() {
    
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
                    keyok = 1;
                }
            }
        }
    }
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
void send_char(char str){
    send_data(str);
}
void send_string(char *str){
    for(int i = 0; str[i] != '\0'; i++) {
        send_data(str[i]);
    }
}
void __interrupt() ISR() {
    if(INTCON & 0x04) {
        cont1++;
        if(keyPress) {
            delayBtn ++;
        }
        INTCON = INTCON & ~0x04;
    }
    return;
}