#define XTFREQ 10000000
#define PLLMODE 8
#define FCY (XTFREQ*PLLMODE/4)  // 20 MIPS, 20MHz
#define FPWM 3600
 
#include <p30Fxxxx.h>
#include <xc.h>
#include <stdio.h>
#include <libpic30.h>
#include <dsp.h>
#include <math.h>
 
#define WDTE_OFF 0xFFE7             // WDT disabled
_FOSC( CSW_FSCM_OFF & XT_PLL8 ); // External Oscillator 
_FBORPOR( MCLR_DIS );            // Disable reset
 
int ADCValue;
 
void init_ADC(void)
{
    ADCON1bits.ADON = 0;
    TRISB = 0xFFFF;   // set as Input Port
    
    ADCHSbits.CH0SA = 1;
    ADCHSbits.CH0NA = 0;

    IEC0bits.ADIE = 1;       // Enable ADC Interrupt       
    IPC2bits.ADIP = 1;       // set interrupt priority-6  
                     
    ADCSSLbits.CSSL0 = 0;    // Skip input scan for analog pin AN0,AN1,AN2
    ADCSSLbits.CSSL1 = 1;
    ADCSSLbits.CSSL2 = 0;
      
    ADCON3bits.SAMC = 0;     // Sample Time TAD=1.5u
    ADCON3bits.ADRC = 0;     // selecting Conversion clock source derived from system clock
    ADCON3bits.ADCS = 58;     // Selecting conversion clock TAD=1.5u
      
    ADCON1bits.ADSIDL = 0;   
    ADCON1bits.FORM = 0;     // Integer output: 0000 dddd dddd dddd
    ADCON1bits.SSRC = 0;     // 7: auto-start mode Selecting Motor Control PWM interval ends sampling and starts conversion
//    ADCON1bits.SIMSAM = 0;   // Samples CH0, CH1, CH2, CH3 simultaneously 
    ADCON1bits.ASAM = 0;     // Sampling begins when SAMP bit is set
    ADCON1bits.SAMP = 0;     
      
    ADCON2bits.VCFG = 0;     // Voltage Reference Configuration bits: set as AVdd and AVss
    ADCON2bits.CSCNA = 0;    // Disable input scan
//    ADCON2bits.CHPS  = 0;    // Selecting conversion channel CH0
    ADCON2bits.SMPI = 0;     // Selecting 1 conversion sample per interrupt
    ADCON2bits.ALTS = 0;     
    ADCON2bits.BUFM = 0;     
      
    ADCON1bits.ADON = 1;     //A/D converter is ON  
    ADPCFG = 0x0000;  // all ANx pins to Analog mode

}
     
void readADC( )
{                      
//    ADCON1bits.SAMP = 1;  // start sampling
//    __delay32(20);
    ADCON1bits.SAMP = 0;        // start sampling, auto conversion follows
    while ( !ADCON1bits.DONE ); // wait to complete the conversion
        ADCValue = ADCBUF0;         // read the conversion result
}
 
int main( void ) 
{     
    _TRISD0 = 0;  // test
    init_ADC();
    int data;
     
    while( 1 )
    {
        readADC();
        data = ADCValue;
         _LATD0 = 1;  // test 
    } 
    
    return 0;
}
