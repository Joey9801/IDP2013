#include "line_following.hh"

//Follow line until junction reached
int line_follow_junction()
{
    while(!(get_linesensors()==0b111))
    {
        switch(line_sensors[0])
        {
        case 0b000:
            //Panic case, all sensors off line
            line_recovery();
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
            right_speed=SPEED+CORRECTION;
            break;
        case 0b110: case 0b010:
            //Right on line, left off - Boost left motor
            left_speed=SPEED+CORRECTION;
            right_speed=SPEED-CORRECTION;
            break;
        default:
            cout << "get_linesensors() returned something funny: " << line_sensors[0] << endl;
            break;
        }
        set_motors(left_speed, right_speed);
    }
    set_motors(0, 0);
    //cout << "
    return 1;
}


char get_linesensors(void)
{
    char val = rlink.request(READ_PORT_0);
    val = val&0b0111; //only take first three sensors
    line_sensors[1] = line_sensors[0];
    line_sensors[0] = val;
    return val;
}


void set_motors(char left_speed, char right_speed)
{
    rlink.command(MOTOR_1_GO, left_speed);
    rlink.command(MOTOR_2_GO, right_speed);
    return;
}

//To be called only at a junction
int line_follow_turn(turning turn)
{
    switch(turn)
    {
    case Left:
        cout << "Performing left turn\n";
        //do something
        break;
    case Right:
        cout << "Performing right turn\n";
        //do something
        break;
    case Forward:
        cout << "Going straight on\n";
        //do something
        break;
    case Backward:
        cout << "Performing 180deg turn\n";
        //do something
        break;
    default:
        cout << "invalid turn asked for\n"; //Panic
        //throw(INVALID_TURN);

    }
    return 1;
}


int line_recovery(void)
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
        cout << "line_recovery() was called at the wrong time\n";
        cout << "line_sensors[] are: " << line_sensors[0] << " " << line_sensors[1] << endl;
        return -1;
    }

    while(!(get_linesensors()&0b100)); //wait for centre line sensor to be high
    set_motors(0, 0);
    cout << "line_recovery() successful\n";

    return 1;
}
