//#include "navigation.hh"

void plan_route(void);
void navigate(void);

directions inverse_direction[5] = {NC, SOUTH, WEST, NORTH, EAST};

//idp_map[i][j] is the direction you have to leave i to get to j
directions idp_map[25][25]; 


void plan_route(){
    //fill in 'robot_route route' with information about the route
    print_route();
}

void navigate(){

    //check that the route has been prepared appropriatly
    if(route.node[0] != status.current_node){
        cout << "Route was planned incorrectly\n";
        cout << "Route starts at node " << route.node[0] << endl;
        cout << "But we are currently at node " << status.current_node << endl;
        //throw(INVALID_ROUTE);
        return;
    }

    for(int i=1; i<=route.length; i++){
    
        status.last_node = status.current_node;
        status.next_node = route.node[i];
        status.travel = IN_TRANSIT;
        
        //navigate from route.node[i] to route.node[i+1]
        
        //direction logic
        turning turn;
        directions current = status.direction;
        directions desired = idp_map[status.current_node][status.next_node];
        
        if(current==desired)
		    turn = FORWARD;
	    else if (current==inverse_direction[desired])
		    turn = BACKWARD;
	    else if((current+1==desired)|((current==4)&(desired==1)))
		    turn = RIGHT;
	    else if((current-1==desired)|((current==1)&(desired==4)))
		    turn = LEFT;
	    else
		    cout << "Invalid directions given\n";
        
        lf_turn(turn); //turn to face the desired node
        lf_until_junction(); //
        
        status.travel = AT_NODE;
        status.current_node = route.node[i];
        //status.direction = inverse_direction[status.current_node][status.last_node];
        i++;        
    }
}
