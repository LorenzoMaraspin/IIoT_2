#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = ON       // Power-up Timer Enable bit (PWRT enabled)
#pragma config BOREN = ON       // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = ON         // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3/PGM pin has PGM function; low-voltage programming enabled)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

#define _XTAL_FREQ 8000000

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
#include <xc.h>

void init_LCD(void);
void send_command(char);
void send_data(char);
void send_string(char *str);

void main(void) {
    TRISD = 0x00;
    TRISE = 0x00;
    
    init_LCD();
    send_command(L_L1);
    char str[] = "ITS KENNEDY";
    send_string(str);
    
    while(1){}
    
    return;
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
        send_data(str[i]);
    }
}
