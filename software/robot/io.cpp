#include "io.hh"

//The next two functions are not complete
void set_front_indicator(parcel_type type){
    outputs[1] |= 0b0000111; //set all front led's off
    //outputs[1]
    return;
}

void set_back_indicator(parcel_type type){
    outputs[1] |= 0b0000111; //set all front led's off
    //outputs[1]
    return;
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
    rlink.command(MOTOR_1_GO, left_speed);
    rlink.command(MOTOR_2_GO, right_speed);
    return;
}


//next three functions are not complete
void set_arm_up(void)
{
    //something about outputs[]
    rlink.command(WRITE_PORT_1, outputs[1]);
    return;	
}

void set_arm_down(void)
{
    //something about outputs[]
    rlink.command(WRITE_PORT_1, outputs[1]);
    return;	
}

void set_conveyor(signed char belt_speed)
{
	rlink.command(MOTOR_3_GO, belt_speed); 
	return;
}

