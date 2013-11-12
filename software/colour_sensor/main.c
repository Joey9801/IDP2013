#define F_CPU 8000000L //8Mhz

#define samples 5

#include <avr/io.h>
#include <util/delay.h>
#include <avr/eeprom.h>
#include "adc.c"

void init(void);
int calibrate(void);

int working;
int measured[3]; //last readings [R, G, B]
int dark;
char calibrations[3]; //calibration multipliers (from eeprom + last calibration)
int working; //working value during the measurements


int main(void)
{
    init();
    int i;
    int ii; //iterating variables
    for(;;)
    {
        PORTA &= 0xF8; //get a dark reading
        _delay_us(20); //let things settle
        for(ii=0; ii<samples; i++) //take n readings
        {
            working += read_adc();
        }
        dark = working/5;
        for(i=0; i<3; i++)
        {
            PORTA = (PORTA&0xF8) |  (1<<i);//set the i'th colour on
            _delay_us(20); //delay to let everything settle
            for(ii=0; ii<samples; i++) //take n readings
            {
                working += read_adc() - dark;
            }
            measured[i] = (working/samples)>>2; //adjust for 'things'
            measured[i] = measured[i] * calibrations[i]; //adjust for whatever the calibrations are
        }

        //logic to work out and output the correct colour (/lack of)

    }
    return 0;
}


void init(void)
{
    DDRA = 0b111; //scanning LED's
    DDRA |= 0b111<<4; //3 output pins
    DDRA &= !(1<<3); //PA3 as input for calibration trigger
    PORTA |= (1<<3); //Pull PA3 high internally

    init_adc();
    /*
    setup:
    i2c as slave
    */

    //read in calibration data from eeprom
}
