#include "navigate.h"
//#include "ultrasonic.h"
#include <stdio.h>

int main()
{
    struct Robot myrobot;
    robot_init(&myrobot);
    printf("%d", MAXLENGTH);
    go_go_spiral(&myrobot);
    print_room(&myrobot);
    return 0;
}