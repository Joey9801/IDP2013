#include <iostream>
#include <stopwatch.h>
#include <robot_instr.h>
#include <robot_link.h>
#include <delay.h>

#define ROBOT_NUM 3 		// The id number (see below)

using namespace std;

enum idp_errors
{
    ROBOT_INIT_FAIL = 1,
    INVALID_ROUTE
    //other errors.. add here as needed
};

enum travel_status
{
    IN_TRANSIT = 1,
    AT_NODE
};
enum turning
{
    LEFT = 1,
    RIGHT,
    FORWARD,
    BACKWARD
};
enum job_status
{
    COLLECTING_PARCELS = 1,
    TRANSPORTING_PARCELS,
    DELIVERING_PARCELS,
    NO_PARCELS
};
enum parcel_type
{
    RED = 1,
    BLUE,
    GREEN,
    NONE
};  

struct robot_status {
    job_status job;
    travel_status travel;
    parcel_type front_parcel;
    parcel_type back_parcel;
    char current_node;
    char last_node;
    char next_node;

    stopwatch task_time; //Stopwatch measuring time since start of task
};

struct robot_route {
    char length; //number of "follow until junction" calls in route
                 //also equal to the largest valid index of node[]
    char node[10];
    char starting_node;
    char finishing_node;
};


//////////////////////////////////
//     Function Declarations    //
//////////////////////////////////
int init();

//high level functions
void set_intent(void);     //set the relevant globals declaring what we're going to do
                           //set_intent is where the really high-level programming stuff happens - determines the overall behaviour of the robot
                        
void plan_route(void);     //set the route for the line following to follow
                           //a stop-gap measure in plan_route would be to hard-code the routes
                        
void navigate(void);       //loop through line follining and turns as per the route

void perform_action(void); //do the thing that set_intent decided we should do at the destination


//////////////////////////////////
//       Global Variables       //
//////////////////////////////////

robot_link rlink;
robot_status status;
robot_route route;

char line_sensors[2];
//line_sensors[0] = current readings
//line_sensors[1] = last readings
/*
Bits 0-3 of val are the readings from the 4 line sensors
The bits are high when the sensor is on the line
Sensor allocation is currently as follows:
  bit: Thing
    1: Left
    2: Right
    3: Centre
    4: Unallocated
*/
