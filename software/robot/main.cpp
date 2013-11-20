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
int init();


int main ()
{
	init();
	

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
