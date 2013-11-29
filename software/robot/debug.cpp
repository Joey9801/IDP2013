#include <iostream>

//Generic debug message
#ifdef __verbose__
    #define DEBUG(msg) cout << __PRETTY_FUNCTION__ << \
            " at line " << __LINE__ << ": " << #msg << endl;
#else
    #define DEBUG(msg) while(false)
#endif

void print_status(void);
void print_route(void);

//enum printers
void print_parcel_type(parcel_type type);
void print_turn(turning turn);
void print_job(job_status job);
void print_direction(directions direction);
void print_idp_errors(idp_errors e);



void print_status(void){
    cout << "\n------Printing Status------\n";
    cout << "Current job: ";
    print_job(status.job);
    cout << endl;


    if(status.travel==AT_NODE)
        cout << "Currently: AT_NODE - node:" << status.current_node << endl;
    else
        cout << "Currently: IN_TRANSIT between nodes " << status.last_node << " and " << status.next_node << endl;
        
    cout << "Current direction: ";
    print_direction(status.direction);
    cout << endl;

    cout << "Front parcel is "; print_parcel_type(status.front_parcel);
    cout << "\nBack parcel is "; print_parcel_type(status.back_parcel);
    cout << "\n---------------------------\n\n";
    return;
}

void print_route(void){
    cout << "\n----Printing route plan----\n";
    cout << "route starting node = " << route.node[0] << endl;
    cout << "route.end_node = " << route.end_node << endl;
    cout << "route.end_direction = "; print_direction(route.end_direction); cout << endl;
    cout << "    (Sanity check -- status.current_node = " << status.current_node << ")\n";
    cout << "Proposed route is:\n";
    for(unsigned int i = 0; i<=route.length; i++){
        cout << route.node[i];
        if(i<route.length)
            cout << " -> ";
    }
    cout << "\n---------------------------\n\n";
        
    return;
}

void print_parcel_type(parcel_type type){
    switch(type){
        case(RED):
            cout << "RED";
            break;
        case(GREEN):
            cout << "GREEN";
            break;
        case(BLUE):
            cout << "BLUE";
            break;
        case(NONE):
            cout << "NONE";
            break;
        default:
            cout << "\n!Attempting to print invalid parcel type!\n";
            break;
    }
    return;
}

void print_turn(turning turn){
    switch(turn){
        case LEFT:
            cout << "LEFT";
            break;
        case RIGHT:
            cout << "RIGHT";
            break;
        case FORWARD:
            cout << "FORWARD";
            break;
        case BACKWARD:
            cout << "BACKWARD";
            break;
        default:
            cout << "print_turn() was passed an invalid turn";
            break;
    } 
    return;
}

void print_job(job_status job){
    switch(status.job){
        case(COLLECTING_PARCELS):
            cout << "COLLECTING_PARCELS";
            break;
        case(TRANSPORTING_PARCELS):
            cout << "TRANSPORTING_PARCELS";
            break;
        case(DELIVERING_PARCELS):
            cout << "DELIVERING_PARCELS";
            break;
        case(NO_PARCELS):
            cout << "NO_PARCELS";
            break;
        default:
            cout << "\n!Attempting to print invalid status.job!\n";
            break;
    }
    return;
}

void print_direction(directions direction){
    switch(direction){
        case NORTH:
            cout << "NORTH";
            break;
        case SOUTH:
            cout << "SOUTH";
            break;
        case EAST:
            cout << "EAST";
            break;
        case WEST:
            cout << "WEST";
            break;
        case NC:
            cout << "NC";
            break;
    }
    return;
}

void print_idp_errors(idp_errors e){
    cout << "Error thrown: ";
    switch(e){
        case ROBOT_INIT_FAIL:
            cout << "ROBOT_INIT_FAIL\n";
            break;
        case INVALID_ROUTE:
            cout << "INVALID_ROUTE\n";
            break;
        case INVALID_DIRECTIONS:
            cout << "INVALID_DIRECTIONS\n";
            break;
        case UNKNOWN_ROUTE:
            cout << "UNKNOWN_ROUTE\n";
            break;
        case WRONG_PLACE:
            cout << "WRONG_PLACE\n";
            break;
        case WRONG_DIRECTION:
            cout << "WRONG_DIRECTION\n";
            break;
        default:
            cout << "\nUnkown error was passed to print_idp_errors()\n";
    }
    return;
}
