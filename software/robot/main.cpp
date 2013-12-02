//#define __verbose__

#include "main.hh"
#include "debug.cpp"
#include "io.cpp"
#include "line_following.cpp"
#include "navigation.cpp"

#include <stdlib.h> //for rand()


int main ()
{
    try{
        init();
    }
    catch(idp_errors e){
        print_idp_errors(e);
        cout << "Cannot continue, exiting\n";
        return -1;
    }

    status.initialise();
    init_idp_map();
    

	reverse_to_line(RIGHT);
	lf_until_junction();

	return 0;

	set_motors(128+50, 128+50);
	delay(800);
	set_motors(60,128+60);
	delay(1400);
   	set_motors(100, 100);
	while(!(get_linesensors()&0b100));
	set_motors(0,0);
	lf_until_junction();
	/*
    status.task_time.start();
    while(status.task_time.read()<(1000*60*5)) //Spend 5 minutes on the task before quitting
    {
        set_intent();
    
        try{
            plan_route();
        }
        catch(idp_errors e){
            print_idp_errors(e);
            cout << "No recovery options, exiting\n\n\n";
            return -1;
        }
        
        print_route();
        
        try{
            navigate();
        }
        catch(idp_errors e){
            print_idp_errors(e);
            print_status();
            cout << "No recovery options, exiting\n\n\n";
            return -1;
        }
        
        print_status();
        
        try{
            perform_action();
        }
        catch(idp_errors e){
            print_idp_errors(e);
            print_status();
            cout << "No recovery options, exiting\n\n\n";
            return -1;
        }
        
        print_status();
    }*/

    cout << "5 minutes are up, stopping\n";
    return 0;
    //Shutdown robot properly
    
}

void set_intent(void){

    if(status.front_parcel){
        status.job = DELIVERING_PARCELS;
        switch(status.front_parcel){
            case RED: case GREEN:
                route.end_node = 11;
                route.end_direction = SOUTH;
                break;
            case BLUE:
                route.end_node = 10;
                route.end_direction = EAST;
                break;
            default:
                cout << __func__ << " at line: " << __LINE__ << "failed horribly";
                break;
        }
    }
    else{
        status.job = COLLECTING_PARCELS;
        route.end_node = 4;
        route.end_direction = WEST;
    }
    return;
}

void perform_action(void){
    
    if(status.job==COLLECTING_PARCELS){
        //Check that we're in the right place
        if(status.current_node!=4){
            cout << "perform_action() was asked to collect parcels\n";
            cout << "but status.current_node = " << status.current_node << endl;
            throw(WRONG_PLACE);
        }
        if(status.direction != WEST){
            cout << "perform_action() was asked to collect parcels\n";
            cout << "but we're pointing " << status.direction << " instead of WEST\n";
            throw(WRONG_DIRECTION);
        }
        
        cout << "Advancing to collect parcels\n";
        
        set_motors(50, 50); //gently push against the delivery conveyor
        set_arm_down();
        set_conveyor(127);  //drawing in at max speed - damn this thing is slow
        
        
        //temp code for running without colour sensor
        delay(2000);
        //randomly set the front and back parcels
        status.front_parcel = static_cast<parcel_type>(rand()%3+1);
        status.back_parcel  = static_cast<parcel_type>(rand()%3+1);
        
        
        set_indicators();
        cout << "Parcels collected\n";
        delay(1000);

   
    }
    else if(status.job == DELIVERING_PARCELS){
        //Check that we're in a valid position for the job
        if(!((status.current_node==11)|(status.current_node==10))){
            cout << "perform_action() was asked to deliver parcels\n";
            cout << "but status.current_node = " << status.current_node << endl;
            throw(WRONG_PLACE);
        }
        if((status.current_node==11)&(status.direction!=SOUTH)){
            cout << "perform_action() was asked to deliver parcels to D1\n";
            cout << "but we're pointing "; print_direction(status.direction); 
            cout << " instead of SOUTH\n";
            throw(WRONG_DIRECTION);
        }
        if((status.current_node==10)&(status.direction!=EAST)){
            cout << "perform_action() was asked to deliver parcels to D2\n";
            cout << "but we're pointing "; print_direction(status.direction);
            cout << " instead of EAST\n";
            throw(WRONG_DIRECTION);
        }

        //Deliver a single parcel
        
        delay(1000);
        
        cout << "Delivered "; print_parcel_type(status.front_parcel);
        if(status.current_node==11)
            cout << " to D1\n";
        else
            cout << " to D2\n";
            
        delay(1000);
        
        status.front_parcel = status.back_parcel;
        status.back_parcel = NONE;
        set_indicators();
    }

}


void init(void)
{
    status.initialise();
    init_idp_map();
    set_arm_down();
    
    #ifdef __arm__
    if (!rlink.initialise ())
    {
        rlink.print_errs(" ");
        throw(ROBOT_INIT_FAIL);
    }
    #else
    if (!rlink.initialise (ROBOT_NUM))
    {
        rlink.print_errs(" ");
        throw(ROBOT_INIT_FAIL);
    }
    #endif
    
    int val = rlink.request(TEST_INSTRUCTION);
    if (val == TEST_INSTRUCTION_RESULT)
        cout << "Test passed" << endl;

    else if (val == REQUEST_ERROR)
    {
        cout << "Fatal errors on link:" << endl;
        rlink.print_errs();
        throw(ROBOT_INIT_FAIL);
    }
    else
    {
        cout << "Test failed (bad value returned)" << endl;
        throw(ROBOT_INIT_FAIL);
    }
    return;
    
}
