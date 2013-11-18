#define F_CPU 1000000L //8Mhz

#define threshold

#include <avr/io.h>
#include <util/delay.h>
//#include <avr/eeprom.h>
#include <math.h>
#include "adc.c"

void init(void);
void calibrate(void);
void read_RGBD(int samples);

int measured[3]; //last readings [G, R, B]
int dark;
//char calibrations[3]; //calibration multipliers (from eeprom + last calibration)
int working; //working value during the measurements

int i;
int ii; //iterating variables


int main(void)
{
    init();
    for(;;)
    {
        read_RGBD(8);
        
        measured[0] =measured[0]*1;    //G
        measured[1] =measured[1]*1;    //B
        measured[2] =measured[2]*1;    //R
        
        /*
        float hue = 10;
        hue = atan2(1.1732*(measured[0]-measured[1]), 2*(measured[2]-measured[1]-measured[0]))*57.296;
        if((hue>1.05)&&(hue<3.14))
            PORTB = (PORTB&0xF8)|(1<<0);
        else if((hue>3.14)&&(hue<5.24))
            PORTB = (PORTB&0xF8)|(1<<1);
        else
            PORTB = (PORTB&0xF8)|(1<<2);
        */
        
        //disregard special things for the moment
        //just pick the biggest
        if((measured[0]>measured[1])&&(measured[0]>measured[2]))
            PORTB = (PORTB&0xF8)|(1<<0);
        else if(measured[1]>measured[2])
            PORTB = (PORTB&0xF8)|(1<<1);
        else
            PORTB = (PORTB&0xF8)|(1<<2);
        
    }
    return 0;
}


void init(void)
{
    DDRA = 0b111; //scanning LED's
    DDRB = 0b111; //output LED's
    init_adc();

    //calibrate(); //calibrate at power on regardless for now

    //read in calibration data from eeprom
    //setup i2c as slave
}
/*
void calibrate(void)
{
    read_RGBD(50); //read in lots, accurate calibrations are important
    //something to process this data

    for(i=0; i<3; i++)
    {
        calibrations[i] = measured[i]>>5; //MS8 bits
    }
    return;
}
*/
void read_RGBD(int samples)
{
    int working;
    dark = 0;
    measured[0] = 0;
    measured[1] = 0;
    measured[2] = 0;

    PORTA = PORTA&0xF8; //get a dark reading
    working = 0;
    _delay_ms(1); //let things settle
    for(ii=0; ii<samples; ii++) //take n readings
    {
        dark += read_adc(0b011);
    }
    //dark = working/samples;

    for(i=0; i<3; i++)
    {
        working = 0;
        PORTA = (PORTA&0xF8) | (1<<i);//set the i'th colour on
        _delay_ms(1); //delay to let everything settle
        for(ii=0; ii<samples; ii++) //take n readings
        {
            measured[i] += read_adc(0b011) - dark;
        }
        //measured[i] = working/samples; //adjust for 'things'
    }

}
