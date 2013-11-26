//#include "navigation.hh"

void plan_route(void);
void navigate(void);

void plan_route(){
    //fill in robot_route with information about the route
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

    for(int i = 0; i<=route.length; i++){
    
        status.last_node = status.current_node;
        status.next_node = route.node[i];
        status.travel = IN_TRANSIT;
        
        //navigate from route.node[i] to route.node[i+1]
        
        status.travel = AT_NODE;
        status.current_node = route.node[i];
        i++;        
    }
}
