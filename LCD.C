#include "xc.h"
#include "LCD.h"
#include "string.h"

void delay_ms(unsigned int ms){
    while (ms-- > 0){
        asm("repeat #15998");
        asm("nop");
    }
}

void init(void){
    _RCDIV = 0;
    AD1PCFG = 0xffff;
}

void lcd_cmd(char cmd) {
    I2C2CONbits.SEN = 1;
    while(I2C2CONbits.SEN == 1);
    IFS3bits.MI2C2IF = 0;
    
    I2C2TRN = 0x7C;
    while(IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;
    
    I2C2TRN = 0x00;
    while(IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;
    
    I2C2TRN = cmd;
    while(IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;
    
    I2C2CONbits.PEN = 1;
    while(I2C2CONbits.PEN ==1);
}

void lcd_init(void) {
    I2C2BRG = 0x9D;
    IFS3bits.MI2C2IF = 0;
    I2C2CONbits.I2CEN = 1;
    
    delay_ms(40);
     
    lcd_cmd(0b00111000); // Function set: 8-bit interface, normal instruction mode
    lcd_cmd(0b00111001); // Function set: 8-bit interface, extended instruction mode
    lcd_cmd(0b00010100); // Internal OSC frequency
    lcd_cmd(0b01110000); // Contrast set
    lcd_cmd(0b01010110); // Power/ICON/Contrast control
    lcd_cmd(0b01101100); // Follower control
    
    delay_ms(200);
     
    lcd_cmd(0b00111000); // Function set: 8-bit interface, normal instruction mode
    lcd_cmd(0b00001100); // Display ON/OFF control: display ON, cursor OFF, blink OFF
    lcd_cmd(0b00000001); // Clear display
    
    delay_ms(1);     
}

void lcd_setCursor(char row, char col) {
    lcd_cmd(((0x40*row)+col) + 0b10000000);
}

void lcd_printChar(char data) {
    I2C2CONbits.SEN = 1;
    while(I2C2CONbits.SEN==1);
    IFS3bits.MI2C2IF = 0;
    
    I2C2TRN = 0x7C;
    while(IFS3bits.MI2C2IF ==0);
    IFS3bits.MI2C2IF = 0 ;
    
    I2C2TRN = 0x40; 
    while(IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF =0;
    
    I2C2TRN = data;
    while(IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF =0;
    
    I2C2CONbits.PEN = 1;
    while(I2C2CONbits.PEN == 1);
}

void lcd_printStr(const char s[]) {
    int i, len, display_col;

    len = strlen(s);

    for (i = 0; i < 8; i++) {
        lcd_setCursor(0, i);

        display_col = i;
        if (display_col < len) {
            lcd_printChar(s[display_col]);
        } else {
            lcd_printChar(' ');
        }
    }
}


void lcd_scrollText(const char s[], int row, unsigned int delay_time) {
    int i, len, start_col, display_col;

    len = strlen(s);
    for (start_col = 0; start_col <= len; start_col++) {
        lcd_setCursor(row, 0);

        for (i = 0; i < 8; i++) {
            display_col = start_col + i;
            if (display_col >= len) {
                display_col -= len;
            }
            lcd_printChar(s[display_col]);
        }

        delay_ms(delay_time);
    }
}
