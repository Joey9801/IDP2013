#include "navigation.hh"

void plan_route(void){
    //fill in robot_route with information about the route
    print_route();
}

void navigate(void)

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
