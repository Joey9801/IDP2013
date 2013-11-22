#include <iostream>
using namespace std;
#include <robot_instr.h>
#include <robot_link.h>
#include <stopwatch.h>
#define ROBOT_NUM 3 		// The id number (see below)
robot_link rlink; 		    // datatype for the robot link
stopwatch stopwatch;		//Create stopwatch

int left_speed = 0;
int right_speed = 0;
int init();
int line_follow_junction();
int line_follow_turn();
int error_handling_line();
void pause_1s();



int left_on_line = rlink.request(READ_PORT_0) & 0x01;
int right_on_line = rlink.request(READ_PORT_0) & 0x02;
int centre_on_line = rlink.request(READ_PORT_0) & 0x04;
int last_sensor_values[3]= {0};

int main ()
{
    init();
    line_follow_junction();


}

int init()
{
#ifdef __arm__
    if (!rlink.initialise ())
    {
        cout << "Cannot initialise link" << endl;
        rlink.print_errs(" ");
        return -1;
    }
#else
    if (!rlink.initialise (ROBOT_NUM))
    {
        cout << "Cannot initialise link" << endl;
        rlink.print_errs(" ");
        return -1;
    }
#endif
    int val;				 // Error data from microprocessor
    /*     Initialise link, conditional on compilier (local/wifi)         */


    val = rlink.request (TEST_INSTRUCTION); // send test instruction

    if (val == TEST_INSTRUCTION_RESULT)   // check result
    {
        cout << "Test passed" << endl;
        //All okay, proceed
    }
    else if (val == REQUEST_ERROR)
    {
        cout << "Fatal errors on link:" << endl;
        rlink.print_errs();
    }
    else
    {
        cout << "Test failed (bad value returned)" << endl;
        return -1; // error, finish
    }
    cout << rlink.request (STATUS) << endl;
    rlink.command (STOP_SELECT, 4);
    cout << rlink.request (STATUS) << endl;
    return 1;
}

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

void pause_1s()
{
    stopwatch.start();
    while(stopwatch.read() < 1000)
    {

    }
    stopwatch.stop();
}
