#include "line_following.hh"

//Follow line until junction reached
int lf_until_junction()
{
    char left_speed, right_speed;
    while(!(get_linesensors()==0b111))
    {
        switch(line_sensors[0])
        {
        case 0b000:
            //Panic case, all sensors off line
            lf_line_recovery();
            break;
        case 0b111: case 0b011:
            //Both left and right high, must be at a junction
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
    //cout << "
    return 1;
}

//To be called only at a junction
int lf_turn(turning turn)
{
    unit_forwards(); //advance such that the wheel axis is over the junction
    char rot_speed = 100;
    switch(turn)
    {
    case LEFT:
        #ifdef __verbose__
            cout << "    lf_turn(): Performing left turn\n";
        #endif
        set_motors(-rot_speed, rot_speed);
        delay(1000); //wait for the sensors to clear the line
        while(!(get_linesensors()&0b100)); //wait for the center sensor to hit the line
        break;
        
    case RIGHT:
        #ifdef __verbose__
            cout << "    lf_turn(): Performing right turn\n";
        #endif
        set_motors(rot_speed, -rot_speed);
        delay(1000); //wait for the sensors to clear the line
        while(!(get_linesensors()&0b100)); //wait for the centre sensor to return to the line
        break;
        
    case FORWARD:
        #ifdef __verbose__
            cout << "    lf_turn(): Going straight on\n";
        #endif
        set_motors(rot_speed, rot_speed);
        delay(500); //just enough to advance past the junction
        break;
        
    case BACKWARD:
        #ifdef __verbose__
            cout << "    lf_turn(): Performing 180deg turn\n";
        #endif
        set_motors(rot_speed, -rot_speed);
        delay(3000); //to find empirically - must go past a 90 deg line if there is one
        while(!(get_linesensors()&0b100));
        break;
        
    default:
        cout << "    lf_turn(): invalid turn asked for\n"; //Panic
        //throw(INVALID_TURN);

    }
    set_motors(0,0);
    return 1;
}


int lf_line_recovery(void)
{
//we've lost the line
//rotate on the spot toward where the line last was
    set_motors(0, 0);

    if(line_sensors[1]&0b001) //Left sensor before leaving was high
        set_motors(127+90, 90);

    else if(line_sensors[1]&0b010) //Right sensor before leaving was high
        set_motors(90, 127+90);

    else
    {
        cout << "        lf_line_recovery(): line_recovery() was called at an odd time\n";
        cout << "        lf_line_recovery(): neither the right or the left sensor was high last cycle\n";
        cout << "        lf_line_recovery(): line_sensors[] are: " << line_sensors[0] << " " << line_sensors[1] << endl;
        return -1;
    }

    while(!(get_linesensors()&0b100)); //wait for centre line sensor to be high
    
    set_motors(0, 0);
    cout << "    lf_line_recovery(): successful\n";
    return 1;
}

void unit_forwards(void)
{
    #ifdef __verbose__
        cout << "    unit_forwards(): Travelling a unit step forwards\n";
    #endif
    set_motors(60, 60);
    delay(2500); //empirically found
    set_motors(0, 0);
    return;
}
