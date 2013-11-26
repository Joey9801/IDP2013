void set_actuator(actuator_command command);
void set_belt_motor(int belt_speed);

void set_arm_up(void)
{
    int message = command | outputs; 
    rlink.command(WRITE_PORT_1, message);
    return;	
}

//Change belt motor speed, reverse if >127
void set_belt_motor(int belt_speed)
{
	rlink.command(MOTOR_3_GO, belt_speed); 
	return;
}
