//#define __verbose__ //enables the DEBUG("") lines
#define __virtual__ //doesn't actually try to communicate with the robot
#include "main.hh"
#include "debug.cpp"
#include "io.cpp"
#include "line_following.cpp"
#include "navigation.cpp"

#include <stdlib.h> //for rand()

void collection_test(void);



int main ()
{
    try{
        #ifndef __virtual__
        init();
        #else
        cout << "\n\n----------------------------------\n";
        cout << "|   Initialising virtual robot   |\n";
        cout << "----------------------------------\n\n";
        status.initialise();
        init_idp_map();
        #endif
    }
    catch(idp_errors e){
        print_idp_errors(e);
        cout << "Cannot continue, exiting\n";
        return -1;
    }

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
    }

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
                route.end_direction = WEST;
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
    
        set_request(true);
        
        //set_motors(40, 40); //gently push against the delivery conveyor
        set_arm_down();
        delay(1000);
        set_conveyor(127);  //drawing in at max speed - damn this thing is slow

        int i = 0;

        while(i<5){
            if(get_coloursensor()!=NONE)
                i++;
            else
                i=0;
            delay(100);
        }
        
        status.back_parcel = get_coloursensor();
        set_indicators();
        
        delay(4000);

        return;
        i=0;
        while(i<5){
            if(get_coloursensor()!=NONE)
                i++;
            else
                i=0;
            delay(100);
        }
        
        status.front_parcel = get_coloursensor();
        set_indicators();

        delay(3000); //wait for the parcels to get all the way on.
        set_conveyor(0);
        //set_motors(0, 0);
        
        set_request(false);
        
        set_arm_up();
        cout << "Parcels collected\n";
   
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
        if((status.current_node==10)&(status.direction!=WEST)){
            cout << "perform_action() was asked to deliver parcels to D2\n";
            cout << "but we're pointing "; print_direction(status.direction);
            cout << " instead of WEST\n";
            throw(WRONG_DIRECTION);
        }

        if(status.current_node==10){
            unit_forwards();
            unit_forwards();
            lf_turn(BACKWARD);
            status.direction = EAST;
            lf_until_junction();            
        }
        
        
        //Deliver a single parcel
        
        set_motors(40, 40); //gently push
        set_arm_up();
        set_conveyor(128+127);
        delay(9000); //change this
        
        set_motors(0, 0);
        
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
    set_indicators();
    set_request(false);
    
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


void collection_test(void){
    cout << "Advancing to collect parcels\n";
    
    set_request(true);
    
    //set_motors(40, 40); //gently push against the delivery conveyor
    set_arm_down();
    delay(1000);
    set_conveyor(127);  //drawing in at max speed - damn this thing is slow

    int i = 0;

    while(i<5){
        if(get_coloursensor()!=NONE)
            i++;
        else
            i=0;
        delay(100);
    }
    
    status.back_parcel = get_coloursensor();
    set_indicators();
    
    delay(4000);

    return;
    i=0;
    while(i<5){
        if(get_coloursensor()!=NONE)
            i++;
        else
            i=0;
        delay(100);
    }
    
    status.front_parcel = get_coloursensor();
    set_indicators();

    delay(3000); //wait for the parcels to get all the way on.
    set_conveyor(0);
    //set_motors(0, 0);
    
    set_request(false);
    
    set_arm_up();
    cout << "Parcels collected\n";

}
