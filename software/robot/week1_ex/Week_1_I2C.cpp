#include <iostream>
using namespace std;
#include <robot_instr.h>
#include <robot_link.h>
#include <stopwatch.h>
#define ROBOT_NUM 3 // The id number (see below)
robot_link rlink; // datatype for the robot link
stopwatch stopwatch;
// http://www-h.eng.cam.ac.uk/help/tpl/IDP/softwarenotes/
int main ()
{
    int val; // data from microprocessor
#ifdef __arm__
    if (!rlink.initialise ())   // setup the link
    {
        cout << "Cannot initialise link" << endl;
        rlink.print_errs(" ");
        return -1;
    }
#else
    if (!rlink.initialise (ROBOT_NUM))   // setup the link
    {
        cout << "Cannot initialise link" << endl;
        rlink.print_errs(" ");
        return -1;
    }
#endif

    val = rlink.request (TEST_INSTRUCTION); // send test instruction


//cout << rlink.request (READ_PORT_0); // Read I2C port
    int param = 0xFF;
    rlink.command (WRITE_PORT_1, param); // Write I2C port

//motor
    int speed = 127;
    int t = 254;
    rlink.command (RAMP_TIME, t);
    rlink.command (STOP_SELECT, 0x01);
    rlink.command (STOP_IF_HIGH, 0x01);
    rlink.command(MOTOR_1_GO, speed);
    rlink.command(MOTOR_2_GO, speed);

    while(rlink.request (READ_PORT_1) != 128)
    {
    }



    if (val == TEST_INSTRUCTION_RESULT)   // check result
    {
        cout << "Test passed" << endl;
        return 0; // all OK, finish
    }
    else if (val == REQUEST_ERROR)
    {
        cout << "Fatal errors on link:" << endl;
        rlink.print_errs();
    }
    else
        cout << "Test failed (bad value returned)" << endl;
    return -1; // error, finish
}
