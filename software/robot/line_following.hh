#include <iostream>
#include <robot_instr.h>
#include <robot_link.h>

using namespace std;

/*
#define left_sensor (rlink.request(READ_PORT_0) & (1<<0))>>0;
#define right_sensor (rlink.request(READ_PORT_0) & (1<<1))>>1;
#define centre_sensor (rlink.request(READ_PORT_0) & (1<<2))>>2;
*/

int line_follow_junction();
int line_follow_turn();
int error_handling_line();

int left_speed = 0;
int right_speed = 0;
int left_on_line = rlink.request(READ_PORT_0) & 0x01;
int right_on_line = rlink.request(READ_PORT_0) & 0x02;
int centre_on_line = rlink.request(READ_PORT_0) & 0x04;
int last_sensor_values[3]= {0};
