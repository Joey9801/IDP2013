#include <iostream>
#include <stopwatch.h>
#include <robot_instr.h>
#include <robot_link.h>
#include <delay.h>


robot_link rlink;
#include "line_following.cpp"

#define ROBOT_NUM 3 		// The id number (see below)

using namespace std;


int init();
void pause_1s();

stopwatch task_time;		//Create stopwatch


int main ()
{
    init();
    line_follow_junction();

    /*
    for(;;)
    {
        set_intent(); //set the relevant globals declaring what we're going to do
        //set_intent is where the really high-level programming stuff happens - determines the overall behaviour of the robot

        plan_route(); //set the route for the line following to follow
        //a stop-gap measure in plan_route would be to hard-code the routes

        navigate(); //loop through line follining and turns as per the route

        perform_action(); //do the thing that set_intent decided we should do at the destination
    }
    */
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


    int val = rlink.request (TEST_INSTRUCTION);

    if (val == TEST_INSTRUCTION_RESULT)
        cout << "Test passed" << endl;

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
