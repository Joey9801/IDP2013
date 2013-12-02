#include "io.hh"

#include <bitset>

//The next two functions are not complete
void set_indicators(void){
    outputs[1] |= 0b1110111; //set all 6 coloured led's off
    switch(status.front_parcel){
        case NONE:
            break;
        case RED:
            outputs[1] ^= (1<<0);
            break;
        case GREEN:
            outputs[1] ^= (1<<1);
            break;
        case BLUE:
            outputs[1] ^= (1<<2);
            break;
        default:
            cout << "Warning: status.front_parcel is set to an invalid type\n";
            break;    
    }
    switch(status.back_parcel){
        case NONE:
            break;
        case RED:
            outputs[1] ^= (1<<6);
            break;
        case GREEN:
            outputs[1] ^= (1<<5);
            break;
        case BLUE:
            outputs[1] ^= (1<<4);
            break;
        default:
            cout << "Warning: status.back_parcel is set to an invalid type\n";
            break;    
    }
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

parcel_type get_coloursensor(void){
    char val = rlink.request(READ_PORT_0);
    
    val = val&(0b111<<4);
    if(val){
        if(val&(1<<4))
            return RED;
        if(val&(1<<5))
            return BLUE;
        if(val&(1<<6))
            return GREEN;
    }
    return NONE;
        
    
}

void set_motors(signed char left_speed, signed char right_speed)
{

    if(left_speed<0)
        left_speed = -left_speed+128;
    if(right_speed<0)
        right_speed = -right_speed +128;
      
    rlink.command(MOTOR_1_GO, 128+left_speed); //motor is connected backwards
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


