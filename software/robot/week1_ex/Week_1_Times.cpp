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

    stopwatch.start();
    rlink.request (READ_PORT_0); // Read I2C port
    cout << stopwatch.stop()<<endl;

    stopwatch.start();
    int param = 0xFF;
    rlink.command (WRITE_PORT_1, param); // Write I2C port
    cout << stopwatch.stop()<<endl;


    stopwatch.start();
    int v1123 = rlink.request (ADC0);
    cout << stopwatch.stop()<<endl;




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
