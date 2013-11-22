#include "line_following.hh"


//Follow line until junction reached
int line_follow_junction()
{
    left_on_line = (rlink.request(READ_PORT_0) & 0x01);
    right_on_line = (rlink.request(READ_PORT_0) & 0x02);
    centre_on_line = (rlink.request(READ_PORT_0) & 0x04);
    while(!(right_on_line && left_on_line) )	//Run until both sensors are high/on line, i.e. on a junction
    {
        left_on_line = (rlink.request(READ_PORT_0) & 0x01);
        right_on_line = (rlink.request(READ_PORT_0) & 0x02);
        centre_on_line = (rlink.request(READ_PORT_0) & 0x04);
        if( ((!left_on_line) && (!right_on_line) && (!centre_on_line)) && (last_sensor_values[0] || last_sensor_values[2]))
        {
            cout << error_handling_line();
        }
        last_sensor_values[0] = left_on_line;
        last_sensor_values[1] = centre_on_line;
        last_sensor_values[2] = right_on_line;
        //Print Left/Right line sensor values
        cout << right_on_line << ' ' << centre_on_line << ' ' << left_on_line <<endl;
        //Left side reading black? Boost right motor
        if(left_on_line)
        {
            left_speed=60-30;
            right_speed=60+30;
        }
        //Right side reading black? Boost left motor
        if(right_on_line)
        {
            left_speed=60+30;
            right_speed=60-30;
        }

        rlink.command(MOTOR_1_GO, left_speed);		//Update left motor speed
        rlink.command(MOTOR_2_GO, right_speed);		//Update right motor speed
        left_speed = 60;							//Reset motors to same speeds, will run forward next loop unless correction needed
        right_speed = 60;
    }
    return 1;

}
//Perform 90/180 degree turn at junction
int line_follow_turn()
{
    return 1;
}

int error_handling_line()
{
    rlink.command(MOTOR_1_GO, 0);		//stop left motor
    rlink.command(MOTOR_2_GO, 0);		//stop right motor

    if(last_sensor_values[0]) //Left sensor before leaving was high
    {
        left_speed = 127+90;
        right_speed = 90;
        cout<< "here";
    }
    if(last_sensor_values[2]) //Right sensor before leaving was high
    {
        left_speed = 90;
        right_speed = 127+90;
        cout<< "here";
    }

    while(!(centre_on_line))
    {
        rlink.command(MOTOR_1_GO, left_speed);		//start left motor
        rlink.command(MOTOR_2_GO, right_speed);		//start right motor
        left_on_line = (rlink.request(READ_PORT_0) & 0x01);
        right_on_line = (rlink.request(READ_PORT_0) & 0x02);
        centre_on_line = (rlink.request(READ_PORT_0) & 0x04);
    }

    return 11111;
}
