#define F_CPU 1000000L //8Mhz

#define threshold

#include <avr/io.h>
#include <util/delay.h>

#include "adc.c"

void init(void);
void calibrate(void);
void read_RGBD(int samples);

enum types{
	GREEN = 0,
	RED,
	BLUE,
	NONE
};


long int measured[3]; //last readings [G, R, B]
int dark;
//char calibrations[3]; //calibration multipliers (from eeprom + last calibration)
int working; //working value during the measurements

int i;
int ii; //iterating variables


int main(void)
{
	enum types max, min;
	unsigned char out;
	init();
    for(;;)
    {
        read_RGBD(10);
		
		if((measured[GREEN]<measured[RED])&&(measured[GREEN]<measured[BLUE]))
            min = GREEN;
        else if(measured[RED]<measured[BLUE])
			min = RED;
        else
			min = BLUE;
			
		
		
        //disregard special things for the moment
        //just pick the biggest
        if((measured[GREEN]>measured[RED])&&(measured[GREEN]>measured[BLUE])){
			max = GREEN;
			out = (1<<0);
			}
        else if(measured[RED]>measured[BLUE]){
			out = (1<<1);
			max = RED;
			}
		else{
			out = (1<<2);
			max = BLUE;
			}
		
		if((measured[max]*90) <= (measured[min]*100))
			out = 0;
		
		PORTB = out;
		
		/*
		switch(type){
			case 3:
				PORTB = (PORTB&0xF8);
				break;
			case 2:
				PORTB = (PORTB&0xF8)|(1<<2);
				break;
			case 1:
				PORTB = (PORTB&0xF8)|(1<<1);
				break;
			case 0:
				PORTB = (PORTB&0xF8)|(1<<0);
				break;
			default:
				break;
		}
        */
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
    /*
	for(ii=0; ii<samples; ii++) //take n readings
    {
        dark += read_adc(0b011);
    }
    //dark = working/samples;
	*/
	
    for(i=0; i<3; i++)
    {
        working = 0;
        PORTA = (PORTA&0xF8) | (1<<i);//set the i'th colour on
        _delay_ms(1); //delay to let everything settle
        for(ii=0; ii<samples; ii++) //take n readings
        {
            measured[i] += read_adc(0b011);// - dark;
        }
        //measured[i] = working/samples; //adjust for 'things'
    }

}
