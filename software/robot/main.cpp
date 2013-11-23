#include "main.hh"
#include "line_following.cpp"
#include "debug.cpp"

int main ()
{
    init();
    line_follow_junction();

    /*
    status.time.start()
    while(status.time.read()<(1000*60*5)) //Spend 5 minutes on the task before quitting
    {
        set_intent();

        plan_route(); 

        navigate(); 

        perform_action(); 
    }
    
    //Shutdown robot properly
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


    int val = rlink.request(TEST_INSTRUCTION);

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
