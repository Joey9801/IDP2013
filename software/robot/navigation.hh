void plan_route(void);
void navigate(void);

void init_idp_map(void);

turning calculate_turn(directions current, directions desired);




//////////////////////
//   Fixed routes   //
//////////////////////
//First element is the length of the route
unsigned int route1_4[]   = {5, 1, 8, 7, 6, 3, 4};
unsigned int route1_11[]  = {5, 1, 9, 10, 11, 12, 11};
unsigned int route1_10[]  = {2, 1, 9, 10};

unsigned int route4_11[]  = {3, 4, 11, 12, 11};
unsigned int route4_10[]  = {2, 4, 11, 10};
unsigned int route4_1[]   = {5, 4, 3, 6, 7, 8, 1};

unsigned int route11_4[]  = {3, 11, 4, 3, 4};
unsigned int route11_10[] = {1, 11, 10};
unsigned int route11_1[]  = {3, 11, 10, 9, 1};

unsigned int route10_1[]  = {2, 10, 9, 1};
unsigned int route10_11[] = {3, 10, 11, 12, 11};
unsigned int route10_4[]  = {4, 10, 11, 4, 3, 4};



//idp_map[i][j] is the direction you have to leave i to get to j
directions idp_map[25][25]; 

//List of nodes connections

void init_idp_map(void){

    for(int i = 0; i<25; i++){
        for(int ii=0; ii<25; ii++){
            idp_map[i][ii] = NC;        
        }
    }
    
    //4-way junctions
    idp_map[0][1] = WEST;
    idp_map[0][8] = NORTH;
    idp_map[0][9] = SOUTH;
    idp_map[0][18] = EAST;

    idp_map[1][0] = EAST;
    idp_map[1][2] = WEST;
    idp_map[1][8] = NORTH;
    idp_map[1][9] = SOUTH;

    idp_map[2][1] = EAST;
    idp_map[2][3] = WEST;
    idp_map[2][8] = NORTH;
    idp_map[2][9] = SOUTH;

    idp_map[3][2] = EAST;
    idp_map[3][4] = WEST;
    idp_map[3][6] = NORTH;
    idp_map[3][12] = SOUTH;

    idp_map[4][3] = EAST;
    idp_map[4][5] = NORTH;
    idp_map[4][11] = SOUTH;
    idp_map[4][24] = WEST;

    idp_map[5][4] = SOUTH;
    idp_map[5][6] = EAST;
    idp_map[5][22] = NORTH;
    idp_map[5][23] = WEST;

    idp_map[6][3] = SOUTH;
    idp_map[6][5] = WEST;
    idp_map[6][7] = EAST;
    idp_map[6][21] = NORTH;

    idp_map[7][6] = WEST;
    idp_map[7][8] = EAST;
    idp_map[7][19] = SOUTH;
    idp_map[7][20] = NORTH;

    idp_map[8][0] = EAST;
    idp_map[8][1] = SOUTH;
    idp_map[8][2] = WEST;
    idp_map[8][7] = NORTH;

    idp_map[9][0] = EAST;
    idp_map[9][1] = NORTH;
    idp_map[9][2] = WEST;
    idp_map[9][10] = SOUTH;

    idp_map[10][9] = NORTH;
    idp_map[10][11] = SOUTH;
    idp_map[10][16] = WEST;
    idp_map[10][17] = EAST;

    idp_map[11][4] = WEST;
    idp_map[11][10] = EAST;
    idp_map[11][12] = NORTH;
    idp_map[11][13] = SOUTH;

    idp_map[12][3] = WEST;
    idp_map[12][11] = SOUTH;
    idp_map[12][14] = NORTH;
    idp_map[12][15] = EAST;
    
    //"stubs"
    idp_map[13][11] = NORTH;

    idp_map[14][12] = SOUTH;

    idp_map[15][12] = WEST;

    idp_map[16][10] = EAST;

    idp_map[17][10] = WEST;

    idp_map[18][0] = WEST;

    idp_map[19][7] = NORTH;

    idp_map[20][7] = SOUTH;

    idp_map[21][6] = SOUTH;

    idp_map[22][5] = SOUTH;

    idp_map[23][5] = EAST;

    idp_map[24][4] = EAST;
}

