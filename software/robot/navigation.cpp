#include "navigation.hh"

void plan_route(){
    //fill in 'robot_route route' with information about the route
    print_route();
}

turning calculate_turn(directions current, directions desired);

void navigate(){

    //check that the route has been prepared appropriatly
    if(route.node[0] != status.current_node){
        cout << "Route was planned incorrectly\n";
        cout << "Route starts at node " << route.node[0] << endl;
        cout << "But we are currently at node " << status.current_node << endl;
        throw(INVALID_ROUTE);
        return;
    }
    
    turning turn;
    for(unsigned int i=1; i<=route.length; i++){
    
        status.last_node = status.current_node;
        status.next_node = route.node[i];
        status.travel = IN_TRANSIT;
        
        //navigate from route.node[i] to route.node[i+1]
        
        //direction logic
        directions current = status.direction;
        directions desired = idp_map[status.current_node][status.next_node];
        
        try{
            turn = calculate_turn(current, desired);
        }
        catch(...){
            throw;
        }
		
		
		cout << "Currently at node " << status.current_node << endl;
		cout << "Turning ";
		print_turn(turn);
		cout << " to travel to node " << status.next_node << endl;
		
		
        DEBUG("Calling lf_turn()");
        lf_turn(turn); //turn to face the desired node;

        cout << "Finished turn, starting transit" << endl << endl;
        DEBUG("Calling lf_until_junction()");
        lf_until_junction();
        
        status.travel = AT_NODE;
        status.current_node = route.node[i];
        status.direction = inverse_direction[(idp_map[status.current_node][status.last_node])];      
    }
    
    try{
        turn = calculate_turn(status.direction, route.end_direction);
        lf_turn(turn);
    }
    catch(...){
        throw;
    }
    
    cout << "\nnavigate() has finished following the route\n";
    print_status();
}

turning calculate_turn(directions current, directions desired){
    if(current==desired)
	    return FORWARD;
    else if (current==inverse_direction[desired])
	    return BACKWARD;
    else if(desired==right_of[current])
	    return RIGHT;
    else if(desired==left_of[current])
	    return LEFT;
    else
	    throw(INVALID_DIRECTIONS);
}
