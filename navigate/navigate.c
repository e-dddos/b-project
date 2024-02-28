#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#define MAXLENGTH 5
#define MAXWIDTH 5

struct Robot
{
    uint8_t x_pos;
    uint8_t y_pos;
    /*Which direction the robot is looking at (When looking from the starting perspective).
    0 - forward
    1 - to the right
    2 - back
    3 - to the left*/
    uint8_t direction;
    /*Information about next free fields in all 4 directions 
    (Looking in the direction where the robot is looking). true = field is free, false - there's an obstacle
    free_directions[0] - forward
    free_directions[1] - to the right
    free_directions[2] - back
    free_directions[3] - to the left   */
    bool free_directions[4];
};

void get_free_directions_from_Nils_sensors(struct Robot *myrobot);
void move_forward(struct Robot *myrobot);
void print_coordinates(struct Robot *myrobot);

int main()
{
    // create a room array:
    bool room[MAXLENGTH][MAXWIDTH]; // Set the max room size
    struct Robot myrobot;
    // Start position:
    myrobot.x_pos = 0;
    myrobot.y_pos = 0;
    myrobot.direction = 0;
    for (int x = 0; x < 10; x++)
    { // First, all fields are closed and we're going to open them while going through them
        for (int y = 0; y < 10; y++)
        {
            room[y][x] = false;
        }
    }
    // We assume that we always begin at 0,0 coordinates, and this field is of course free
    room[0][0] = true;

    print_coordinates(&myrobot);
    get_free_directions_from_Nils_sensors(&myrobot);
    while (myrobot.free_directions[0] == true)
    {
        move_forward(&myrobot);
        get_free_directions_from_Nils_sensors(&myrobot);
    }
    print_coordinates(&myrobot);

    return 0;
}

void get_free_directions_from_Nils_sensors(struct Robot *myrobot)
{
    //For debugging without an actual robot and room, there's our virtual room:
    bool room[MAXLENGTH][MAXWIDTH] = {
        {true, false, true, true, true},
        {true, true, true, true, true},
        {true, true, true, true, true},
        {false, true, true, true, true},
        {true, true, true, true, true},
    };

    if (myrobot->x_pos + 1 > 4) // Check if the next field forward exists
        myrobot->free_directions[0] = false;
    else
        myrobot->free_directions[0] = room[myrobot->x_pos + 1][myrobot->y_pos]; // get the value of the field

    if (myrobot->y_pos + 1 > 4) // Check if the next field to the right exists
        myrobot->free_directions[1] = false;
    else
        myrobot->free_directions[1] = room[myrobot->x_pos][myrobot->y_pos + 1]; // get the value of the field

    if (myrobot->x_pos - 1 < 0) // Check if the next field back exists
        myrobot->free_directions[2] = false;
    else
        myrobot->free_directions[2] = room[myrobot->x_pos - 1][myrobot->y_pos]; // get the value of the field

    if (myrobot->y_pos - 1 < 0) // Check if the next field to the left exists
        myrobot->free_directions[3] = false;
    else
        myrobot->free_directions[3] = room[myrobot->x_pos][myrobot->y_pos - 1]; // get the value of the field
}

void move_forward(struct Robot *myrobot)
{
    switch (myrobot->direction)
    {
    case 0:
        myrobot->x_pos++;
        break;
    case 1:
        myrobot->y_pos++;
        break;
    case 2:
        myrobot->x_pos--;
        break;
    case 3:
        myrobot->y_pos--;
        break;
    default:
        break;
    }
    // physical_move_forward_func_from_Peter_and_Michael();
    printf("I moved forward!\n");
}

void print_coordinates(struct Robot *myrobot)
{
    printf("x: %d\ny: %d\ndir: %d\n", myrobot->x_pos, myrobot->y_pos, myrobot->direction);
}