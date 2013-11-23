void print_status(void);
void print_parcel_type(parcel_type type);

void print_status(void){
    cout << "Current job: ";
    switch(status.job){
        case(COLLECTING_PARCELS):
            cout << "COLLECTING_PARCELS\n";
            break;
        case(TRANSPORTING_PARCELS):
            cout << "TRANSPORTING_PARCELS\n";
            break;
        case(DELIVERING_PARCELS):
            cout << "DELIVERING_PARCELS\n";
            break;
        case(NO_PARCELS):
            cout << "NO_PARCELS\n";
            break;
        default:
            cout << "\n!Attempting to print invalid status.job!\n";
            break;
    }
    
    if(status.travel==AT_NODE)
        cout << "Currently: AT_NODE - " << status.current_node << endl;
    else
        cout << "Currently: IN_TRANSIT between nodes " << status.last_node << " and " << status.next_node << endl;

    cout << "Front parcel is ";
    print_parcel_type(status.front_parcel);
    cout << "Back parcel is ";
    print_parcel_type(status.back_parcel);
    
    return;
}

void print_route(void){
    cout << "Printing route plan\n";
    cout << "route.starting_node = " << route.starting_node << endl;
    cout << "route.finishing_node = " << route.finishing_node << endl;
    cout << " (Sanity check - status.current_node = " << status.current_node << ")\n";
    cout << "Proposed route is:\n";
    for(int i = 0; i<=route.length; i++)
        cout << route.node_list[i] << " -> ";
        
    return;
}

void print_parcel_type(parcel_type type){
    switch(type){
        case(RED):
            cout << "RED\n";
            break;
        case(GREEN):
            cout << "GREEN\n";
            break;
        case(BLUE):
            cout << "BLUE\n";
            break;
        case(NONE):
            cout << "NONE\n";
            break;
        default:
            cout << "\n!Attempting to print invalid parcel type!\n";
            break;
    }
    return;
}
