#include "line_following.hh"

//Follow line until junction reached
void lf_until_junction(void)
{
    char left_speed, right_speed;
    while(!(get_linesensors()==0b111))
    {
        switch(line_sensors[0])
        {
        case 0b000:
            //All sensors off line
            if(line_sensors[1]&0b100){
                cout << __func__ << ": Detected end of stub";
                set_motors(0, 0);
                return;
            }
            cout << __func__ <<  ": Lost line, calling lf_line_recovery()\n";
            lf_line_recovery();
            break;
            
        case 0b111: case 0b011:
            //Both left and right high, must be at a junction
            DEBUG("Junction detected");
            return;
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
    char rot_speed = 80;
    switch(turn)
    {
    case LEFT:
        DEBUG("Performing left turn");
        unit_forwards();
        set_motors(127+rot_speed, rot_speed);
        delay(500); //wait for the sensors to clear the line
        while(!(get_linesensors()&0b010)); //wait for the center sensor to hit the line
        break;
        
    case RIGHT:
        DEBUG("Performing right turn");
        unit_forwards();
        set_motors(rot_speed, 127+rot_speed);
        delay(500); //wait for the sensors to clear the line
        while(!(get_linesensors()&0b001)); //wait for the centre sensor to return to the line
        break;
        
    case FORWARD:
        DEBUG("Going straight on");
        set_motors(SPEED, SPEED);
        delay(150); //just enough to advance past the junction
        return;
        break;
        
    case BACKWARD:
        DEBUG("Performing 180deg turn");
        unit_forwards();
        set_motors(rot_speed, 127+rot_speed);
        delay(2000); //to find empirically - must go past a 90 deg line if there is one
        while(!(get_linesensors()&0b100));
        break;
        
    default:
        cout << "    lf_turn(): invalid turn asked for\n"; //Panic
        //throw(INVALID_TURN);

    }
    set_motors(0,0);
    return;
}


void lf_line_recovery(void)
{
//we've lost the line
//rotate on the spot toward where the line last was
    set_motors(0, 0);
    DEBUG("Attempting to recover line");
    if(line_sensors[1]&0b001) //Left sensor before leaving was high
        set_motors(127+45, 45);

    else if(line_sensors[1]&0b010) //Right sensor before leaving was high
        set_motors(45, 127+45);

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
    DEBUG("Travelling a unit step forwards");
    set_motors(60, 60);
    delay(1550); //empirically found
    set_motors(0, 0);
    return;
}
