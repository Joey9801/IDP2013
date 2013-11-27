#include "main.hh"
#define __verbose__
#include "debug.cpp"
#include "io.cpp"
#include "line_following.cpp"
#include "navigation.cpp"

int main ()
{
    init();
    /*
    try{
        init();
    }
    catch(idp_errors e){
        //do something with the error
        return;
    }
    */
    lf_until_junction();
    lf_turn(RIGHT);

    //demo route
    route.node[0] = 1;
    route.node[1] = 2;
    route.node[2] = 9;
    route.node[3] = 10;
    route.node[4] = 17;
    route.length = 4;
    
    status.initialise();
    
    navigate();
	    
    /*status.time.start();
    while(status.time.read()<(1000*60*5)) //Spend 5 minutes on the task before quitting
    {
        set_intent();

        plan_route(); 

        navigate(); 

        perform_action(); 
    }
    */
    //Shutdown robot properly
    
}

void init()
{
    //status.initialise(void); //Initialises status facing west in the starting box
    
    #ifdef __arm__
    if (!rlink.initialise ())
    {
        cout << "Cannot initialise link" << endl;
        rlink.print_errs(" ");
        //throw(ROBOT_INIT_FAIL);
        return;
    }
    #else
    if (!rlink.initialise (ROBOT_NUM))
    {
        cout << "Cannot initialise link" << endl;
        rlink.print_errs(" ");
        //throw(ROBOT_INIT_FAIL);
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
        //throw(ROBOT_INIT_FAIL);
        return -1;
    }
    else
    {
        cout << "Test failed (bad value returned)" << endl;
        //throw(ROBOT_INIT_FAIL);
        return -1; // error, finish
    }
    cout << rlink.request (STATUS) << endl;
    rlink.command (STOP_SELECT, 4);
    cout << rlink.request (STATUS) << endl;
    return 1;
    
}
