#include "line_following.hh"

//Follow line until junction reached
void lf_until_junction(void)
{
    #ifdef __virtual__
        cout << __PRETTY_FUNCTION__ << " was called\n";
        delay(1000);
        return;
    #endif
    char left_speed, right_speed;
    while(1)//!(get_linesensors()==0b111))
    {
        get_linesensors();
        switch(line_sensors[0])
        {
        case 0b000:
            //All sensors off line
            if(line_sensors[1]&0b100){
                //cout << __func__ << ": Detected end of stub";
                //set_motors(0, 0);
                //return;
            }
            cout << __func__ <<  ": Lost line, calling lf_line_recovery()\n";
            lf_line_recovery(ROT_SPEED);
            break;
            
        case 0b111: case 0b011:
            //Both left and right high, must be at a junction
            DEBUG("Junction detected");
            //if((line_sensors[1]==0b111)&(line_sensors[0]==0b111))
                return;
            //delay(100);
            break;
            
        case 0b100:
            //Only centre sensor high, everything is happy
            left_speed = right_speed = SPEED;
            break;
        case 0b101: case 0b001:
            //Left on line, right off - Boost right motor
            left_speed=SPEED-CORRECTION;
            right_speed=SPEED;
            break;
        case 0b110: case 0b010:
            //Right on line, left off - Boost left motor
            left_speed=SPEED;
            right_speed=SPEED-CORRECTION;
            break;
        default:
            cout << "    lf_until_junction(): get_linesensors() returned something funny: " << line_sensors[0] << endl;
            break;
        }
        set_motors(left_speed, right_speed);
    }
    set_motors(0, 0);
    return;
}

//To be called only at a junction
void lf_turn(turning turn)
{
    #ifdef __virtual__
        cout << __PRETTY_FUNCTION__ << " was called with argument ";
        print_turn(turn); cout << endl;
        delay(1000);
        return;
    #endif
    
    bool reverse = false;
    unsigned int node = status.current_node;
    directions dir = status.direction;
    if((node==4)&&(dir==WEST))
        reverse = true;
    else if((node==11)&&(dir==SOUTH))
        reverse = true;
    else if((node==10)&&(dir==EAST))
        reverse = true;
    if((turn==FORWARD)|(turn==BACKWARD))
        reverse=false;
    
    if(reverse){
        cout << "reverse to turn called\n";
        reverse_to_line(turn);
        return; //don't do another turn
    }
    
    switch(turn)
    {
    case LEFT:
        DEBUG("Performing left turn");
        unit_forwards();
        set_motors(128+127, 127);
        delay(800); //wait for the sensors to clear the line
        set_motors(128+20, 20);
        while(!(get_linesensors()&0b100)); //wait for the center sensor to hit the line
        break;
        
    case RIGHT:
        DEBUG("Performing right turn");
        unit_forwards();
        set_motors(127, 128+127);
        delay(800); //wait for the sensors to clear the line
        set_motors(20, 128+20);
        while(!(get_linesensors()&0b100)); //wait for the centre sensor to return to the line
        break;
        
    case FORWARD:
        DEBUG("Going straight on");
        set_motors(SPEED, SPEED);
        delay(150); //just enough to advance past the junction
        get_linesensors();
        return;
        break;
        
    case BACKWARD:
        DEBUG("Performing 180deg turn");
        unit_forwards();
        set_motors(127, 128+127);
        delay(1800); //to find empirically - must go past a 90 deg line if there is one
        set_motors(20, 128+20);
        while(!(get_linesensors()&0b100));
        break;
        
    default:
        cout << "    lf_turn(): invalid turn asked for\n"; //Panic
        //throw(INVALID_TURN);

    }
    set_motors(0,0);
    return;
}


void lf_line_recovery(char rot_speed)
{
    #ifdef __virtual__
        cout << __PRETTY_FUNCTION__ << " was called with argument " << int(rot_speed) << endl;
        delay(1000);
        return;
    #endif
//we've lost the line
//rotate on the spot toward where the line last was
    set_motors(0, 0);
    DEBUG("Attempting to recover line");
    if(line_sensors[1]&0b001) //Left sensor before leaving was high
        set_motors(128+rot_speed, rot_speed);

    else if(line_sensors[1]&0b010) //Right sensor before leaving was high
        set_motors(rot_speed, 128+rot_speed);

    else
    {
        cout << "        lf_line_recovery(): line_recovery() was called at an odd time\n";
        cout << "        lf_line_recovery(): neither right nor left sensor was high last cycle\n";
        cout << "        lf_line_recovery(): line_sensors[] are: " << line_sensors[0] << " " << line_sensors[1] << endl;
        
        //throw(WRONG_LINE_RECOVERY);
        return;
    }

    while(!(get_linesensors()&0b100)); //wait for centre line sensor to be high
    
    set_motors(0, 0);
    cout << __func__ << ": Success\n";
    return;
}

void unit_forwards(void)
{
    #ifdef __virtual__
        cout << __PRETTY_FUNCTION__ << "was called\n";
        delay(1000);
        return;
    #endif
    DEBUG("Travelling a unit step forwards");
    set_motors(60, 60);
    delay(1400); //empirically found
    set_motors(0, 0);
    return;
}
//called in perform_action, taking a left/right turn to get onto line
void reverse_to_line(turning turn)
{
    #ifdef __virtual__
        cout << __PRETTY_FUNCTION__ << "was called with argument ";
        print_turn(turn); cout << endl;
        delay(1000);
        return;
    #endif
	set_motors(128+50, 128+50);
	delay(1000);
	if(turn == RIGHT)
		{	
		set_motors(60,128+60);
		}
	else
		{
		set_motors(128+60,60);
		}
	delay(1700);
	set_motors(80, 80);
	while(!(get_linesensors()&0b100));
	set_motors(0,0);
}
