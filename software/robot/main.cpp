#include "main.hh"
#include "debug.cpp"
#include "io.cpp"
#include "line_following.cpp"
#include "navigation.cpp"

#define __verbose__


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

    //status.initialise();
    //init_idp_map();

    //Test Route for collection    
    
    route.node[0] = 1;
    route.node[1] = 8;
    route.node[2] = 7;
    route.node[3] = 19;
    route.node[4] = 7;
    route.node[5] = 8;
    route.node[6] = 1;
    route.node[7] = 9;
    route.node[8] = 10;
    route.node[9] = 16;
    route.node[10] = 10;
    route.node[11] = 9;
    route.node[12] = 1;
    route.length = 12;
    route.end_direction = WEST;
    print_route();
    
    //route.end_node = 4;
    //route.end_direction = WEST;
    //plan_route();
    
    try{
        navigate();
    }
    catch(idp_errors e){
        print_idp_errors(e);
        print_status();
        cout << "No recovery options, exiting\n\n\n";
    }
    
    set_motors(40, 40);
    
    set_arm_up();
    delay(1000);
    set_arm_down();
    delay(1000);
    set_conveyor(127);
    for(;;);
    //set_conveyor(128+127);
    


//Mechanical Demo!!!
/*

	//lights and actuator demo
	cout << "Lights and actuator demo\n";
	for(int i=0; i<3; i++){
	    rlink.command(WRITE_PORT_1, 0xFF);
	    delay(1000);
	    rlink.command(WRITE_PORT_1, 0x00);
	    delay(1000);
	}
	
	set_arm_up();
	delay(2000);
	
	//Collection and Delivery demo
	cout << "Collection and Delivery demo\n\n";
	cout << "Collecting Parcels\n";
	set_arm_down();
	delay(1000);
	set_conveyor(100);
	delay(10000);
	set_conveyor(0);
	set_arm_up();
	delay(1000);
	
	
	cout << "Delivering Parcels\n";
	set_conveyor(-100);
	delay(10000);
	set_conveyor(0);
*/	
	
	
    /*status.time.start();
    while(status.time.read()<(1000*60*5)) //Spend 5 minutes on the task before quitting
    {
        set_intent();
        
        try{
            plan_route();
        }
        catch(idp_errors e){
            print_idp_error(e);
            cout << "Cannot recover from this, exiting\n";
            return -1;
        }

        navigate(); 

        perform_action(); 
    }
    */
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
    
    print_status();
    return;
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
