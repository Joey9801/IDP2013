#include <iostream>
#include <robot_instr.h>
#include <robot_link.h>

#define SPEED 60
#define CORRECTION 30

using namespace std;

enum turn_t
{
    Left = 1,
    Right,
    Forward,
    Backward
};

int line_follow_junction(void);
int line_follow_turn(turn_t);
int line_recovery(void);
void set_motors(char left_speed, char right_speed);

char get_linesensors(void);

int left_speed = 0;
int right_speed = 0;

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
