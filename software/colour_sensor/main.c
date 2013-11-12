#define F_CPU 8000000L //8Mhz

#include <avr/io.h>
#include <util/delay.h>

void init(void);

int main(void){
    init();
    for(;;){
        //main loop
    }
    return 0;
}


void io_init(void){
/*
setup:
DDR registers
ADC
i2c as slave
*/
}
