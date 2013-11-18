#define F_CPU 1000000L //8Mhz

#include <avr/io.h>
#include <util/delay.h>

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
    for(;;){
        for(i=0; i<4; i++){
            PORTA = (PORTA&0xF8) | (1<<i);
            PORTB = (PORTB&0xF8) | (1<<i);
            _delay_ms(500);
        }
    }
}


void init(void)
{
    DDRA = 0b111; //scanning LED's
    DDRB = 0b111; //output LED's
    return;
}
