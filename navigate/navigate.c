#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
// Set the max room size
#define MAXLENGTH 12
#define MAXWIDTH 12

struct Robot
{
    uint8_t x_pos;
    uint8_t y_pos;
    /*Which direction the robot is looking at (When standing in the left down corner,
     the wall is on the left).
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
    uint8_t best_directions[4];

    // create a room array:
    uint8_t room[MAXLENGTH][MAXWIDTH];
    uint8_t steps_done;
    uint8_t fields_visited;
    bool cant_move;
    bool finished;
};

void robot_init(struct Robot *myrobot);
void get_free_directions_from_Nils_sensors(struct Robot *myrobot);
void get_best_directions(struct Robot *myrobot);
void move_forward(struct Robot *myrobot);
void turn_right(struct Robot *myrobot);
void turn_left(struct Robot *myrobot);
void set_field_visited(struct Robot *myrobot);
void print_room(struct Robot *myrobot);
void go_go_spiral(struct Robot *myrobot);
uint8_t find_min_index(uint8_t arr[], uint8_t size);

int main()
{
    struct Robot myrobot;
    robot_init(&myrobot);
    printf("Press Enter to make a step\n");
    go_go_spiral(&myrobot);
    print_room(&myrobot);

    return 0;
}

void robot_init(struct Robot *myrobot)
{
    // Start position:
    myrobot->x_pos = 1;
    myrobot->y_pos = 1;
    myrobot->direction = 0;
    myrobot->steps_done = 0;
    myrobot->fields_visited = 0;
    myrobot->cant_move = false;
    myrobot->finished = false;

    /*Romm array, the walls have the value 0xFF, this will be important later,
    the not yet discovered fields have the value 0*/
    //Create the room array with the walls:
    for (int x = 0; x < MAXLENGTH; x++)
    {
        for (int y = 0; y < MAXWIDTH; y++)
        {
            if ((x == 0) || (x == (MAXLENGTH - 1)) || (y == 0) || (y == (MAXWIDTH - 1)))
            {
                myrobot->room[x][y] = 255;
            }
            else
            {
                myrobot->room[x][y] = 0;
            }
        }
    }
    // We assume that we always begin at 0,0 coordinates, and this field is of course free
    set_field_visited(myrobot);
    get_free_directions_from_Nils_sensors(myrobot);
    get_best_directions(myrobot);
}

// The function will be replaced by Nils' function, for now just for virtual testing
void get_free_directions_from_Nils_sensors(struct Robot *myrobot)
{
    // Test the algorithm by adding obstacles (zeros) in the room array here:
    uint8_t room[MAXLENGTH][MAXWIDTH] = {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0},
        {0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 0},
        {0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 0},
        {0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0},
        {0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0},
        {0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0},
        {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
        {0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
        {0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0},
        {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    };
    uint8_t x_pos = myrobot->x_pos;
    uint8_t y_pos = myrobot->y_pos;
    uint8_t free_fields = 0;
    //Count how many free fields there are to know later that we're finished
    for (int x = 0; x < MAXLENGTH; x++)
    {
        for (int y = 0; y < MAXWIDTH; y++)
        {
            free_fields = free_fields + room[x][y];
        }
    }
    //Check whether we finished the room:
    if (myrobot->fields_visited >= free_fields)
    {
        myrobot->finished = true;
        return;
    }

    //Get the free directions:
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
    //If there's no free direction, we can't move:
    if ((myrobot->free_directions[0] == false) &&
        (myrobot->free_directions[1] == false) &&
        (myrobot->free_directions[2] == false) &&
        (myrobot->free_directions[3]) == false)
    {
        myrobot->cant_move = true;
        printf("Can't move!\n");
    }
    else
    {
        myrobot->cant_move = false;
    }
}

/*We check how many times the neighboured fields were already visired and store that values in 
the myrobot->best_directions array*/
void get_best_directions(struct Robot *myrobot)
{

    switch (myrobot->direction)
    {
    case 0:
        myrobot->best_directions[0] = myrobot->room[myrobot->x_pos + 1][myrobot->y_pos];
        myrobot->best_directions[1] = myrobot->room[myrobot->x_pos][myrobot->y_pos + 1];
        myrobot->best_directions[2] = myrobot->room[myrobot->x_pos - 1][myrobot->y_pos];
        myrobot->best_directions[3] = myrobot->room[myrobot->x_pos][myrobot->y_pos - 1];
        break;
    case 1:
        myrobot->best_directions[0] = myrobot->room[myrobot->x_pos][myrobot->y_pos + 1];
        myrobot->best_directions[1] = myrobot->room[myrobot->x_pos - 1][myrobot->y_pos];
        myrobot->best_directions[2] = myrobot->room[myrobot->x_pos][myrobot->y_pos - 1];
        myrobot->best_directions[3] = myrobot->room[myrobot->x_pos + 1][myrobot->y_pos];
        break;
    case 2:
        myrobot->best_directions[0] = myrobot->room[myrobot->x_pos - 1][myrobot->y_pos];
        myrobot->best_directions[1] = myrobot->room[myrobot->x_pos][myrobot->y_pos - 1];
        myrobot->best_directions[2] = myrobot->room[myrobot->x_pos + 1][myrobot->y_pos];
        myrobot->best_directions[3] = myrobot->room[myrobot->x_pos][myrobot->y_pos + 1];
        break;
    case 3:
        myrobot->best_directions[0] = myrobot->room[myrobot->x_pos][myrobot->y_pos - 1];
        myrobot->best_directions[1] = myrobot->room[myrobot->x_pos + 1][myrobot->y_pos];
        myrobot->best_directions[2] = myrobot->room[myrobot->x_pos][myrobot->y_pos + 1];
        myrobot->best_directions[3] = myrobot->room[myrobot->x_pos - 1][myrobot->y_pos];
        break;
    default:
        break;
    }
}

void move_forward(struct Robot *myrobot)
{
    char scan;
    scanf("%c", &scan);
    //We check in which direction we're going and move through our room array
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
    // After we moved, we want to set the field as visited
    set_field_visited(myrobot);
    printf("I moved forward!\n");
    print_room(myrobot);
}
void turn_right(struct Robot *myrobot)
{
    if (myrobot->direction == 3)
        myrobot->direction = 0;
    else
        myrobot->direction++;
    // physical_turn_right_func_from_Peter_and_Michael();
    // printf("I turned right!\n");
}
void turn_left(struct Robot *myrobot)
{
    if (myrobot->direction == 0)
        myrobot->direction = 3;
    else
        myrobot->direction--;
    // physical_turn_left_func_from_Peter_and_Michael();
    printf("I turned left!\n");
}
void go_go_spiral(struct Robot *myrobot)
{
    /*We want to move to the field that was visited less times than the others, so our 
    preferred direction is the minimum of our best_directions array*/
    uint8_t pref_dir = find_min_index(myrobot->best_directions, 4);

    while ((myrobot->cant_move == false) && (myrobot->finished == false))
    {
        /*As long as the preferred direction is not blocked, 
        we check the other directions, giving the blocked direction the 0xFF value*/
        while (myrobot->free_directions[pref_dir] != true)
        {
            myrobot->best_directions[pref_dir] = 255;
            pref_dir = find_min_index(myrobot->best_directions, 4);
        }
        //turn in our preferred direction:
        for (int i = 0; i < pref_dir; i++)
        {
            turn_right(myrobot);
        }
        move_forward(myrobot);
        get_free_directions_from_Nils_sensors(myrobot);
        get_best_directions(myrobot);
        pref_dir = find_min_index(myrobot->best_directions, 4);
    }
}

void set_field_visited(struct Robot *myrobot)
{
    //If the filed has the value 0, means we visited a new field
    if (myrobot->room[myrobot->x_pos][myrobot->y_pos] == 0)
    {
        myrobot->fields_visited++;
    }
    myrobot->room[myrobot->x_pos][myrobot->y_pos]++;
    myrobot->steps_done++;
}

void print_room(struct Robot *myrobot)
{
    for (int x = 0; x < MAXLENGTH; x++)
    {
        for (int y = 0; y < MAXWIDTH; y++)
        {
            printf("%x ", myrobot->room[y][x]);
        }
        printf("\n");
    }
    printf("Fields visited: %d\nSteps done: %d\n", myrobot->fields_visited, myrobot->steps_done);
}

uint8_t find_min_index(uint8_t arr[], uint8_t size)
{
    uint8_t min = 0;

    for (uint8_t i = 0; i < size; i++)
    {
        if (arr[i] < arr[min])
        {
            min = i;
        }
    }
    return min;
}