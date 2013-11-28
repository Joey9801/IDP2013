#include "io.hh"

//The next two functions are not complete
void set_indicators(void){
    outputs[1] |= 0b1110111; //set all 6 coloured led's off
    outputs[1] ^= (1<<status.front_parcel); //These two lines will need changing/replacing
    outputs[1] ^= (1<<status.back_parcel)<<4; //I have no idea which colours are connected to which pins atm
    rlink.command(WRITE_PORT_1, outputs[1]);

}

char get_linesensors(void)
{
    char val = rlink.request(READ_PORT_0);
    val = val&0b0111; //only take first three sensors
    line_sensors[1] = line_sensors[0];
    line_sensors[0] = val;
    return val;
}

void set_motors(signed char left_speed, signed char right_speed)
{
/*
    if(left_speed<0)
        left_speed ^= 0x7F;
    if(right_speed<0)
        right_speed ^= 0x7F;
*/        
    rlink.command(MOTOR_1_GO, 127+left_speed);
    rlink.command(MOTOR_2_GO, right_speed);
    return;
}


//next three functions are not complete
void set_arm_up(void)
{
    //something about outputs[1]
    outputs[1] |= (1<<7);
    rlink.command(WRITE_PORT_1, outputs[1]);
    return;	
}

void set_arm_down(void)
{
    //something about outputs[1]
    outputs[1] &= ~(1<<7);
    rlink.command(WRITE_PORT_1, outputs[1]);
    return;	
}

void set_conveyor(signed char belt_speed)
{
    if(belt_speed<0)
        belt_speed ^= 0x7F;
	rlink.command(MOTOR_4_GO, belt_speed); 
	return;
}


