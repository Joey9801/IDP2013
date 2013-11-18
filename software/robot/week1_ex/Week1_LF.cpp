#include <iostream>
using namespace std;
#include <robot_instr.h>
#include <robot_link.h>
#include <stopwatch.h>
#define ROBOT_NUM 3 		// The id number (see below)
robot_link rlink; 		    // datatype for the robot link
stopwatch stopwatch;		//Create stopwatch

int left_speed;
int right_speed;
void turnright()
{
    left_speed=127;
    right_speed=30;
}
void turnleft()
{
    left_speed=30;
    right_speed=127;
}

void sharpRight()
{
    stopwatch.start();
    while(stopwatch.read() < 1000)
    {
        left_speed=127;
        right_speed=127;
        rlink.command(MOTOR_1_GO, left_speed);
        rlink.command(MOTOR_2_GO, right_speed);
    }
    stopwatch.stop();
    //Rotation starts,
    left_speed=127;
    right_speed=127+127;
    //Get off line
    stopwatch.start();
    while(stopwatch.read() < 1000)
    {
        rlink.command(MOTOR_1_GO, left_speed);
        rlink.command(MOTOR_2_GO, right_speed);
    }
    stopwatch.stop();
    //Stop when on new line
    while((rlink.request(READ_PORT_0) & 0x02) == 0)
    {
        rlink.command(MOTOR_1_GO, left_speed);
        rlink.command(MOTOR_2_GO, right_speed);
    }
}
void sharpLeft()
{
    stopwatch.start();
    while(stopwatch.read() < 1000)
    {
        left_speed=127;
        right_speed=127;
        rlink.command(MOTOR_1_GO, left_speed);
        rlink.command(MOTOR_2_GO, right_speed);
    }
    stopwatch.stop();
    //Rotation starts,
    left_speed=127+127;
    right_speed=127;
    //Get off line
    stopwatch.start();
    while(stopwatch.read() < 1000)
    {
        rlink.command(MOTOR_1_GO, left_speed);
        rlink.command(MOTOR_2_GO, right_speed);
    }
    stopwatch.stop();
    //Stop when on new line
    while((rlink.request(READ_PORT_0) & 0x01) == 0)
    {
        rlink.command(MOTOR_1_GO, left_speed);
        rlink.command(MOTOR_2_GO, right_speed);
    }
}

int main ()
{
    int val;				 // Error data from microprocessor
    /*     Initialise link, conditional on compilier (local/wifi)         */
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

    while(!((rlink.request(READ_PORT_0) & 0x02) && (rlink.request(READ_PORT_0) & 0x01)))	//run until both sensors are high/on line, ie on a junction
    {
        cout<< (rlink.request(READ_PORT_0) & 0x02) << ' ' << (rlink.request(READ_PORT_0) & 0x01) <<endl;
        if((rlink.request(READ_PORT_0) & 0x01)) 	//left side reading black
        {
            turnright();
        }
        if((rlink.request(READ_PORT_0) & 0x02)) 	//right side reading black
        {
            turnleft();
        }

        rlink.command(MOTOR_1_GO, left_speed);
        rlink.command(MOTOR_2_GO, right_speed);
        left_speed = 60;
        right_speed = 60;

    }
    sharpRight();
    rlink.command (STOP_IF_HIGH, 0x05);
    cout << rlink.request (STATUS) << endl;

    rlink.command (STOP_IF_LOW, 0x10);
    cout << rlink.request (STATUS) << endl;




}
