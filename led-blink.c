
#include <stdio.h>
#include <stdlib.h>
#include <p18f2550.h>
#define _XTAL_FREQ 16000000
#pragma config PLLDIV = 4
#pragma config CPUDIV = OSC1_PLL2
#pragma config FOSC   = HS
#pragma config WDT    = OFF
#pragma config LVP    = OFF
#pragma config BOR    = OFF
#pragma config MCLRE  = OFF
#pragma config PWRT   = OFF
#pragma config PBADEN = OFF

void main(void) {

    TRISB = 0;

    while (1) {
        LATBbits.LATB0 = 1;
        LATBbits.LATB1 = 0;
        __delay_ms(1000);
        LATBbits.LATB0 = 0;
        LATBbits.LATB1 = 1;
        __delay_ms(1000);
    }
}
