#include <iostream>
#include <robot_instr.h>
#include <robot_link.h>

using namespace std;

///////////////////////////////////////
//   low level interface functions   //
///////////////////////////////////////
char get_linesensors(void); //update line_sensors[], also return the current sensor values
parcel_type get_coloursensor(void);
void set_request(bool);


//set the main driving motors
void set_motors(signed char left_speed, signed char right_speed);

//controls the collection arm
void set_arm_up(void);
void set_arm_down(void);

//positive belt_speed is into the robot
void set_conveyor(signed char belt_speed);

void set_indicators(void);


/////////////////////////////
//   Global io variables   //
/////////////////////////////
char outputs[2] = {0xFF, 0xFF}; //initialize with everything off
