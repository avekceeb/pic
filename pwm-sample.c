
// https://electrosome.com/pwm-pic-microcontroller-mplab-xc8/

/*
                         pic18f2550
                         +---------+
           MCLR/VPP/RE3 -| 1     28|- RB7/KBI3/PGD
                RA0/AN0 -| 2     27|- RB6/KBI2/PGC
                RA1/AN1 -| 3     26|- RB5/KBI1/PGM
    RA2/AN2/Vref-/CVref -| 4     25|- RB4/AN11/KBI0
          RA3/AN3/Vref+ -| 5     24|- RB3/AN9/CCP2/VPO
    RA4/T0CKI/C1OUT/RCV -| 6     23|- RB2/AN8/INT2/VMO
RA5/AN4/SS/HLVDIN/C2OUT -| 7     22|- RB1/AN10/INT1/SCK/SCL
                    Vss -| 8     21|- RB0/AN12/INT0/FLT0/SDI/SDA
              OSC1/CLKI -| 9     20|- Vdd
          OSC2/CLKO/RA6 -|10     19|- Vss
        RC0/T1OSO/T13CK -|11     18|- RC7/RX/DT/SDO
     RC1/T1OSI/CCP2/UOE -|12     17|- RC6/TX/CK
               RC2/CCP1 -|13     16|- RC5/D+/VP
                   Vusb -|14     15|- RC4/D-/VM
                         +---------+

*/

#define _XTAL_FREQ 16000000
#define TMR2PRESCALE 4

#include <stdlib.h>
#include <stdio.h>
#include <p18f2520.h>

// BEGIN CONFIG
#pragma config PLLDIV = 4
#pragma config CPUDIV = OSC1_PLL2
#pragma config FOSC   = HS
#pragma config WDT    = OFF
#pragma config LVP    = OFF
#pragma config BOR    = OFF
#pragma config MCLRE  = OFF
#pragma config PWRT   = OFF
#pragma config PBADEN = OFF

//END CONFIG

long freq;

int pwm_max_duty() {
  return(_XTAL_FREQ/(freq*TMR2PRESCALE));
}

pwm_init(long f) {
    PR2 = (_XTAL_FREQ/(f*4*TMR2PRESCALE)) - 1;
    freq = f;
}

set_pwm1_duty(unsigned int duty) {
    if (duty<1024) {
        duty = ((float)duty/1023)*pwm_max_duty();
        CCP1CONbits.DC1B1 = duty & 2;
        CCP1CONbits.DC1B0 = duty & 1;
        CCPR1L = duty>>2;
    }
}

set_pwm2_duty(unsigned int duty) {
    if (duty<1024) {
        duty = ((float)duty/1023)*pwm_max_duty();
        CCP2CONbits.DC2B1 = duty & 2;
        CCP2CONbits.DC2B0 = duty & 1;
        CCPR2L = duty>>2;
    }
}

start_pwm1() {
    // set PWM Mode in CCPxCON: STANDARD CCPx CONTROL REGISTER
    CCP1M3 = 1;
    CCP1M2 = 1;

    #if TMR2PRESCALE == 1
        T2CKPS0 = 0;
        T2CKPS1 = 0;
    #elif TMR2PRESCALE == 4
        T2CKPS0 = 1;
        T2CKPS1 = 0;
    #elif TMR2PRESCALE == 16
        T2CKPS0 = 1;
        T2CKPS1 = 1;
    #endif

    TMR2ON = 1;
    TRISC2 = 0;
}

stop_pwm1() {
    CCP1M3 = 0;
    CCP1M2 = 0;
}

start_pwm2() {
    // set PWM Mode in CCPxCON: STANDARD CCPx CONTROL REGISTER
    CCP2M3 = 1;
    CCP2M2 = 1;
    #if TMR2PRESCALE == 1
        T2CKPS0 = 0;
        T2CKPS1 = 0;
    #elif TMR2PRESCALE == 4
        T2CKPS0 = 1;
        T2CKPS1 = 0;
    #elif TMR2PRESCALE == 16
        T2CKPS0 = 1;
        T2CKPS1 = 1;
    #endif

    TMR2ON = 1;
    // Since the CCP2 pin is multiplexed with a PORTB or PORTC data latch,
    // the appropriate TRIS bit must be cleared
    // to make the CCP2 pin an output.
    TRISC1 = 0;
}

stop_pwm2() {
    CCP2M3 = 0;
    CCP2M2 = 0;
}

void main() {
    /*
     * 1. Set the PWM period by writing to the PR2 register.
     * 2. Set the PWM duty cycle by writing to the CCPRxL register
     *    and CCPxCON<5:4> bits.
     * 3. Make the CCPx pin an output by clearing the appropriate TRIS bit.
     * 4. Set the TMR2 prescale value, then enable Timer2 by writing to T2CON.
     * 5. Configure the CCPx module for PWM operation.
     */
    
    unsigned char direction = 1;
    unsigned int duty1=512, duty2=512;
    pwm_init(5000);
    set_pwm1_duty(duty1);
    set_pwm2_duty(duty2);
    start_pwm1();
    start_pwm2();
    do {
        if ((1023 == duty1) || (0 == duty1)) {
            direction ^= 1;
        }
        if (direction) {
            duty1++;
            duty2--;
        } else {
            duty1--;
            duty2++;
        }
        set_pwm1_duty(duty1);
        set_pwm2_duty(duty2);
        __delay_ms(50);
    } while(1);

}