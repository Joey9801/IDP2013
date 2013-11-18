#include <avr/io.h>

void init_adc();
int read_adc(char);

void init_adc ()
{
    ADMUX = 0; //use vcc as reference source and enable ADC3
    ADCSRA = (1<<ADEN)|(1<<ADPS2); //enable adc peripheral with /2 clock prescalar
    return;
}

int read_adc(char ch)
{
    ch &= 0b00000111;  // limit ch to 0-7
    ADMUX = (ADMUX & 0xF8)|ch; //enable ch with ADMUX
    ADCSRA |= (1<<ADSC); //start single conversion
    while(ADCSRA & (1<<ADSC)); //wait for conversion to complete
    return (ADC);
}
