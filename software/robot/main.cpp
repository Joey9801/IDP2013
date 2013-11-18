#include <iostream>
#include <robot_instr.h>
#include <robot_link.h>

//robot id number - change this to be evil and control other peoples robots
#define ROBOT_NUM 3

using namespace std;

robot_link rlink;


int main(void)
{
    cout << "Attempting to initialise link\n";
    if (!rlink.initialise (ROBOT_NUM))   // setup the link
    {
        cout << "Cannot initialise link" << endl;
        rlink.print_errs(" ");
        return -1;
    }
    cout << "successfully linked\n";

        return 0;
}
