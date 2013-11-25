int set_actuator(actuator_command command);
int set_belt_motor(int belt_speed);

//Extend or retract actuator
// actuator_command: Extend / Retract
int set_actuator(actuator_command command)
{
int message = command | outputs; 
rlink.command(WRITE_PORT_1, message);
return 1;	
}

//Change belt motor speed, reverse if >127
int set_belt_motor(int belt_speed)
{
	rlink.command(MOTOR_3_GO, belt_speed); 
	return 1;
}
