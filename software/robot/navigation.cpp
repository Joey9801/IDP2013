#include "navigation.hh"

void plan_route(){
    if(route.end_node==status.current_node){
        route.length = 0;
        cout << "Trivial plan route -> route.end_node==status.current_node\n";
        return;
    }
    //fill in 'robot_route route' with information about the route
    bool fail = false;
    switch(route.end_node){
        case 4: //Going to Pickup node
            switch(status.current_node){
                case 1:
                    route.node[0] = 1; //route.node = {1, 8, 7, 6, 4, 3};
                    route.node[1] = 8;
                    route.node[2] = 15;
                    route.node[3] = 6;
                    route.node[4] = 3;
                    route.node[5] = 4;
                    route.length = 5;
                    break;
                case 10:
                    break;
                case 11:
                    break;
                default:
                    fail = true;
            }
            break;
            
        case 11: //Going to Delivery node 1
            switch(status.current_node){
                case 1:
                    break;
                case 10:
                    break;
                case 4:
                    break;
                default:
                    fail = true;
            }
            break;
            
        case 10: //Going to Delivery node 2
            switch(status.current_node){
                case 1:
                    break;
                case 11:
                    break;
                case 4:
                    break;
                default:
                    fail = true;
            }
            break;
            
        case 1: //Going to Starting node - for prizes
                //since points mean prizes
            switch(status.current_node){
                default:
                    fail = true;
            }
            break;
            
        default:
            fail = true;
    }
    
    if(fail){
        cout << "Don't know how to navigate from " << status.current_node 
             << " to " << route.end_node << endl;
        throw(UNKNOWN_ROUTE);
    }
    
    
    print_route();
}

void navigate(){

    //check that the route has been prepared appropriatly
    if(route.node[0] != status.current_node){
        cout << "Route was planned incorrectly\n";
        cout << "Route starts at node " << route.node[0] << endl;
        cout << "But we are currently at node " << status.current_node << endl;
        throw(INVALID_ROUTE);
        return;
    }
    
    for(unsigned int i=0; i<route.length; i++){
        if(idp_map[route.node[i]][route.node[i+1]] == NC){
            cout << "node " << route.node[i] << " is not connected to node " << route.node[i+1] << endl;
            throw(INVALID_ROUTE);
        }
    }
    
    cout << "Route was checked\n";
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
		
		
		cout << "Turning ";
		print_turn(turn);
		cout << " to reach node " << status.next_node << endl;
		
		
        DEBUG("Calling lf_turn()");
        lf_turn(turn); //turn to face the desired node;

        cout << "Finished turn, starting transit" << endl;
        DEBUG("Calling lf_until_junction()");
        lf_until_junction();
        
        status.travel = AT_NODE;
        status.current_node = route.node[i];
        status.direction = inverse_direction[(idp_map[status.current_node][status.last_node])];    
        
        cout << "Reached node " << status.current_node << endl << endl;  
    }
    
    if(route.end_direction){
        try{
            turn = calculate_turn(status.direction, route.end_direction);
            cout << "Making final ";
            print_turn(turn);
            cout << " turn to face ";
            print_direction(route.end_direction);
            cout << endl;
            lf_turn(turn);
        }
        catch(...){
            throw;
        }
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
        cout << __PRETTY_FUNCTION__ << " Failed" << endl;
        cout << "Current  = ";
        print_direction(current);
        cout << "\nDesired = ";
        print_direction(desired);
        cout << endl;
	    throw(INVALID_DIRECTIONS);
}
