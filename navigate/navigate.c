#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
// Set the max room size
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
    // create a room array:
    bool room[MAXLENGTH][MAXWIDTH];
};

void get_free_directions_from_Nils_sensors(struct Robot *myrobot);
void move_forward(struct Robot *myrobot);
void turn_right(struct Robot *myrobot);
void turn_left(struct Robot *myrobot);
void set_field_visited(struct Robot *myrobot);
void print_coordinates(struct Robot *myrobot);
void print_room(bool room[MAXLENGTH][MAXWIDTH]);
void complete_a_row(struct Robot *myrobot);
int main()
{

    struct Robot myrobot;
    // Start position:
    myrobot.x_pos = 0;
    myrobot.y_pos = 0;
    myrobot.direction = 0;
    for (int x = 0; x < MAXLENGTH; x++)
    { // First, all fields are closed and we're going to open them while going through them
        for (int y = 0; y < MAXWIDTH; y++)
        {
            myrobot.room[x][y] = false;
        }
    }
    // We assume that we always begin at 0,0 coordinates, and this field is of course free
    myrobot.room[myrobot.x_pos][myrobot.y_pos] = true;

    print_coordinates(&myrobot);
    get_free_directions_from_Nils_sensors(&myrobot);
    // We'll try to go zigzag every line of the grid:

    // While no obstacle in front:
    while (myrobot.free_directions[0] == true)
    {
        move_forward(&myrobot);
        // measure_air_conditions();
    }
    complete_a_row(&myrobot);

    print_room(myrobot.room);

    return 0;
}

// The function will be replaced by Nils' function, for now just for virtual testing:
void get_free_directions_from_Nils_sensors(struct Robot *myrobot)
{
    //Addind the "walls"
    bool room[MAXLENGTH+2][MAXWIDTH+2] = {
        {false, false, false, false, false, false, false},
        {false, true, false, true, true, true, false},
        {false, true, true, true, false, true, false},
        {false, false, false, true, true, true, false},
        {false, false, true, false, true, true, false},
        {false, false, true, true, true, true, false},
        {false, false, false, false, false, false, false},
    };
    uint8_t x_pos=myrobot->x_pos + 1;
    uint8_t y_pos=myrobot->y_pos + 1;

    // for (int x = 0; x < (MAXLENGTH + 2); x++)
    // {
    //     for (int y = 0; y < (MAXWIDTH + 2); y++)
    //     {
    //         printf("%d", room[y][x]);
    //     }
    //     printf("\n");
    // }
    switch (myrobot->direction)
    {
    case 0:
        myrobot->free_directions[0] = room[x_pos + 1][y_pos];
        myrobot->free_directions[1] = room[x_pos][y_pos + 1];
        myrobot->free_directions[2] = room[x_pos - 1][y_pos];
        myrobot->free_directions[3] = room[x_pos][y_pos - 1];        
        break;
    case 1:
        myrobot->free_directions[0] = room[x_pos][y_pos + 1];
        myrobot->free_directions[1] = room[x_pos - 1][y_pos];
        myrobot->free_directions[2] = room[x_pos][y_pos - 1];  
        myrobot->free_directions[3] = room[x_pos + 1][y_pos];
        break;
    case 2:
        myrobot->free_directions[0] = room[x_pos - 1][y_pos];
        myrobot->free_directions[1] = room[x_pos][y_pos - 1];  
        myrobot->free_directions[2] = room[x_pos + 1][y_pos];
        myrobot->free_directions[3] = room[x_pos][y_pos + 1];
        break;
    case 3:
        myrobot->free_directions[0] = room[x_pos][y_pos - 1];  
        myrobot->free_directions[1] = room[x_pos + 1][y_pos];
        myrobot->free_directions[2] = room[x_pos][y_pos + 1];       
        myrobot->free_directions[3] = room[x_pos - 1][y_pos];
        break;
    default:
        break;
    }
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
    // After we moved, we want to set the field as visited and get the data from the sensors
    set_field_visited(myrobot);
    get_free_directions_from_Nils_sensors(myrobot);
    printf("I moved forward!\n");
}

void print_coordinates(struct Robot *myrobot)
{
    printf("x: %d\ny: %d\ndir: %d\n", myrobot->x_pos, myrobot->y_pos, myrobot->direction);
}
void print_room(bool room[MAXLENGTH][MAXWIDTH]) {
    for (int x = 0; x < MAXLENGTH; x++)
    {
        for (int y = 0; y < MAXWIDTH; y++)
        {
            printf("%d", room[y][x]);
        }
        printf("\n");
    }
}
void set_field_visited(struct Robot *myrobot)
{
    myrobot->room[myrobot->x_pos][myrobot->y_pos] = true;
}
void turn_right(struct Robot *myrobot)
{
    if (myrobot->direction == 3)
        myrobot->direction = 0;
    else
        myrobot->direction++;
    // physical_turn_right_func_from_Peter_and_Michael();
    printf("I turned right!\n");
    get_free_directions_from_Nils_sensors(myrobot);
}
void turn_left(struct Robot *myrobot)
{
    if (myrobot->direction == 0)
        myrobot->direction = 3;
    else
        myrobot->direction--;
    // physical_turn_left_func_from_Peter_and_Michael();
    printf("I turned left!\n");
    get_free_directions_from_Nils_sensors(myrobot);
}
void complete_a_row(struct Robot *myrobot)
{
    uint8_t start_column = myrobot->y_pos;
    // First, turn right:
    turn_right(myrobot);
        // We always try to turn left:
    while (myrobot->free_directions[3] == false)
    {
        if (myrobot->free_directions[0] == true)
        {
            move_forward(myrobot);
        }
        else
        {
            while (myrobot->free_directions[0] != true)
            turn_right(myrobot);
            move_forward(myrobot);
        }
    }
    // If we're out of the loop, we can turn left.
    turn_left(myrobot);
    move_forward(myrobot);
    // We should have definitely changed the column by now. Now we want to return to the column where we started.
    while (myrobot->y_pos != start_column)
    {
        while ((myrobot->free_directions[3] == false) && (myrobot->y_pos != start_column))
    {
        if (myrobot->free_directions[0] == true) 
        {
            move_forward(myrobot);
        }
        else
        {
            while (myrobot->free_directions[0] != true)
                turn_right(myrobot);
            move_forward(myrobot);
        }
    }
        if (myrobot->y_pos == start_column)
            break;
    turn_left(myrobot);
    move_forward(myrobot);
    }
}