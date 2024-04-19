#include <xc.h>
#include <stdint.h>

// Configuration fuses
#pragma config FOSC = INTOSC    // Internal oscillator
#pragma config WDTE = OFF       // Watchdog timer disabled
#pragma config PWRTE = OFF      // Power-up timer disabled
#pragma config MCLRE = OFF      // MCLR pin function disabled
#pragma config CP = OFF         // Code protection disabled
#pragma config CPD = OFF        // Data protection disabled
#pragma config BOREN = OFF      // Brown-out reset disabled
#pragma config CLKOUTEN = OFF   // Clock out disabled
#pragma config IESO = OFF       // Internal/external oscillator switchover disabled
#pragma config FCMEN = OFF      // Fail-safe clock monitor disabled

#define _XTAL_FREQ 4000000     // Internal oscillator frequency
unsigned int adc_value;
void main(void) {
    // Set internal oscillator frequency
    OSCCONbits.IRCF = 0b1101;  // 4MHz

    // Configure AN3 (RA4) as analog input
    ANSELAbits.ANSA4 = 1;

    // Configure RA2 as digital output (PWM output)
    TRISAbits.TRISA2 = 0;

    // Configure ADC
    ADCON1bits.ADPREF = 0b00;  // VDD is the positive reference
    ADCON1bits.ADCS = 0b101;   // FRC clock source (clock frequency divided by 20)
    ADCON1bits.ADFM = 1;       // Right justified result

    ADCON0bits.CHS = 0b00011;  // Select AN3 (RA4) as ADC input channel
    ADCON0bits.ADON = 1;       // Enable ADC module

    // Configure PWM
    CCP3CONbits.CCP3M = 0b1100;    // PWM mode
    CCP3CONbits.DC3B = 0;           // LSBs of PWM duty cycle value
    T2CONbits.T2CKPS = 0b00;        // Timer2 prescaler 1:1
    T2CONbits.TMR2ON = 1;           // Turn on Timer2



    while(1) {
        // Start ADC conversion
        ADCON0bits.GO_nDONE = 1;

        // Wait for ADC conversion to complete
        while(ADCON0bits.GO_nDONE);

        // Read ADC result
        adc_value = (ADRESH << 8) | ADRESL;

        // Update PWM duty cycle based on ADC result
        CCPR3L = adc_value >> 2; // ADC de?erini 8 bitlik PWM de?erine dönü?türmek için sa?a kayd?rma

    }
}


