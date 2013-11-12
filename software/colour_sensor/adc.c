#include <avr/io.h>

void init_adc();
int read_adc(void);

void init_adc ()
{
    ADMUX &= !(1|(1<<1)); //use vcc as reference source
    ADCSRA = (1 << ADEN) | (7 << ADPS0); //enable adv peripheral with /2 clock prescalar
    ADMUX |= 0b111; //selecting PA7/ADC7
}

int read_adc(void)
{
    int adc;
    ADCSRA |= (1 << ADSC);
    while (!(ADCSRA & (1 << ADIF)));
    ADCSRA |= (1 << ADIF);
    adc = ADCL; //read in the low, then the high byte
    adc |= ADCH<<8;
    return (adc);
}
