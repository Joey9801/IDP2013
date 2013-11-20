#include <stdio.h>
#include <stdlib.h>
#include <robot_instr.h>
#include <robot_link.h>

//robot id number - change this to be evil and control other peoples robots
#define ROBOT_NUM 3

using namespace std;

robot_link rlink;

enum idp_errors
{
    ROBOT_INIT_FAIL = 1
    //other errors.. add here as needed
};

void error_handler(idp_errors ret);

void init(void);
void set_intent(void);
void plan_route(void);
void navigate(void);
void perform_action(void);

int main(void)
{
    try
    {
        init();
    }
    catch(idp_errors ret)
    {
        error_handler(ret);
    }

    for(;;)
    {
        set_intent(); //set the relevant globals declaring what we're going to do
        //set_intent is where the really high-level programming stuff happens - determines the overall behaviour of the robot

        plan_route(); //set the route for the line following to follow
        //a stop-gap measure in plan_route would be to hard-code the routes

        navigate(); //loop through line follining and turns as per the route

        perform_action(); //do the thing that set_intent decided we should do at the destination
    }

    return 0;
}


void init(void)
{
    printf("Attempting to initialise link\n");
    if (!rlink.initialise (ROBOT_NUM))   // setup the link
    {
        rlink.print_errs(" ");
        throw(ROBOT_INIT_FAIL);
        //return;
    }
//initialise everything
//flash a couple of lights and make a fuss
    return;
}

void error_handler(idp_errors ret)
{
    printf("Error thrown! - ");
    switch(ret)
    {
    case ROBOT_INIT_FAIL:
        printf("ROBOT_INIT_FAIL\n");
        exit(-1);
    }
}

void set_intent(void) {}
void plan_route(void) {}
void navigate(void) {}
void perform_action(void) {}
