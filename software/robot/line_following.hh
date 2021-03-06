#include <iostream>
#include <robot_instr.h>
#include <robot_link.h>
#include <delay.h>

using namespace std;
#define SPEED 127
#define CORRECTION 75
#define ROT_SPEED 70

//prefix lf_ indicates part of the line_following algorithm
void lf_until_junction(void);
void lf_turn(turning);
void lf_line_recovery(char rot_speed);

void unit_forwards(void); //travel forwards the distance between the sensors and the wheels
void reverse_to_line(turning); //Reverse onto line after action at sleigh/belt
