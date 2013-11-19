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

enum turn_direction{LEFT, RIGHT, AROUND};
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

    line_follow_junction();

    sharpRight();

    rlink.command (STOP_IF_HIGH, 0x05);
    cout << rlink.request (STATUS) << endl;

    rlink.command (STOP_IF_LOW, 0x10);
    cout << rlink.request (STATUS) << endl;




}

//Follow line around corner and 45 bend. Start on line and stop at first junction reached. 
//n.b. alter to allow start at junction just in case, not currently needed as actions at junctions finish ahead of junctions.
int line_follow_junction() 
{
int left_on_line = rlink.request(READ_PORT_0) & 0x01;
int right_on_line = rlink.request(READ_PORT_0) & 0x02;
 while(!(right_on_line && left_on_line) )	//Run until both sensors are high/on line, i.e. on a junction
    {
        //Print Left/Right line sensor values
		cout << right_sensor << ' ' << left_on_line) <<endl; 
		//Left side reading black? Boost right motor
        if(left_on_line) 	
        {
           left_speed=30;
		   right_speed=127;
        }
        //Right side reading black? Boost left motor
		if(right_on_line) 	
        {
             left_speed=127;
			 right_speed=30;
        }

        rlink.command(MOTOR_1_GO, left_speed);		//Update left motor speed		
        rlink.command(MOTOR_2_GO, right_speed);		//Update right motor speed	
        left_speed = 60;							//Reset motors to same speeds, will run forward next loop unless correction needed
        right_speed = 60;

    }
}


//Called at junctions, performs left or right 90 degree turn.
//Leaves line sensors ahead of junction.
void line_follow_turn(turn_direction turn)
{
    //Move sensors ahead of junction to put turning axis on junction
	stopwatch.start();
    while(stopwatch.read() < 1000)
    {
        left_speed=127;
        right_speed=127;
        rlink.command(MOTOR_1_GO, left_speed);
        rlink.command(MOTOR_2_GO, right_speed);
    }
    stopwatch.stop();
    //Set motor speeds for roatation about turning axis
    if(turn == RIGHT OR turn == AROUND)
	{
	left_speed=127;
    right_speed=127+127;
	}
	if(turn == LEFT)
	{
	left_speed=127+127;
    right_speed=127;
	}
    //Rotate for 1000ms to move sensors clear of line
    stopwatch.start();
    while(stopwatch.read() < 1000)
    {
        rlink.command(MOTOR_1_GO, left_speed);
        rlink.command(MOTOR_2_GO, right_speed);
    }
    stopwatch.stop();
    //Stop rotation when sensors on new line
    while((rlink.request(READ_PORT_0) & 0x02) == 0)
    {
        rlink.command(MOTOR_1_GO, left_speed);
        rlink.command(MOTOR_2_GO, right_speed);
    }
}