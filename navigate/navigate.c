#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
// Set the max room size
#define MAXLENGTH 10
#define MAXWIDTH 10

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
    uint8_t fields_visited;
    bool cant_move;
};
void robot_init(struct Robot *myrobot);
void get_free_directions_from_Nils_sensors(struct Robot *myrobot);
void move_forward(struct Robot *myrobot);
void turn_right(struct Robot *myrobot);
void turn_left(struct Robot *myrobot);
void set_field_visited(struct Robot *myrobot);
void print_coordinates(struct Robot *myrobot);
void print_room(bool room[MAXLENGTH][MAXWIDTH]);
void go_go_spiral(struct Robot *myrobot);


int main()
{

    struct Robot myrobot;
    robot_init(&myrobot);
    go_go_spiral(&myrobot);

    print_coordinates(&myrobot);
    print_room(myrobot.room);

    return 0;
}


void robot_init(struct Robot *myrobot) {
    // Start position:
    myrobot->x_pos = 0;
    myrobot->y_pos = 0;
    myrobot->direction = 0;
    myrobot->fields_visited = 0;
    myrobot->cant_move = false;
    for (int x = 0; x < MAXLENGTH; x++)
    { // First, all fields are closed and we're going to open them while going through them
        for (int y = 0; y < MAXWIDTH; y++)
        {
            myrobot->room[x][y] = true;
        }
    }
    // We assume that we always begin at 0,0 coordinates, and this field is of course free
    set_field_visited(myrobot);
    print_coordinates(myrobot);
    get_free_directions_from_Nils_sensors(myrobot);
}

// The function will be replaced by Nils' function, for now just for virtual testing:
void get_free_directions_from_Nils_sensors(struct Robot *myrobot)
{
    // Addind the "walls"
    bool room[MAXLENGTH + 2][MAXWIDTH + 2] = {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
        {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
        {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
        {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
        {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
        {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
        {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
        {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
        {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
        {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    };
    uint8_t x_pos = myrobot->x_pos + 1;
    uint8_t y_pos = myrobot->y_pos + 1;

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

        myrobot->free_directions[0] = myrobot->free_directions[0] && myrobot->room[myrobot->x_pos + 1][myrobot->y_pos];
        myrobot->free_directions[1] = myrobot->free_directions[1] && myrobot->room[myrobot->x_pos][myrobot->y_pos + 1];
        myrobot->free_directions[2] = myrobot->free_directions[2] && myrobot->room[myrobot->x_pos - 1][myrobot->y_pos];
        myrobot->free_directions[3] = myrobot->free_directions[3] && myrobot->room[myrobot->x_pos][myrobot->y_pos - 1];
        break;
    case 1:
        myrobot->free_directions[0] = room[x_pos][y_pos + 1];
        myrobot->free_directions[1] = room[x_pos - 1][y_pos];
        myrobot->free_directions[2] = room[x_pos][y_pos - 1];
        myrobot->free_directions[3] = room[x_pos + 1][y_pos];

        myrobot->free_directions[0] = myrobot->free_directions[0] && myrobot->room[myrobot->x_pos][myrobot->y_pos + 1];
        myrobot->free_directions[1] = myrobot->free_directions[1] && myrobot->room[myrobot->x_pos - 1][myrobot->y_pos];
        myrobot->free_directions[2] = myrobot->free_directions[2] && myrobot->room[myrobot->x_pos][myrobot->y_pos - 1];
        myrobot->free_directions[3] = myrobot->free_directions[3] && myrobot->room[myrobot->x_pos + 1][myrobot->y_pos];
        break;
    case 2:
        myrobot->free_directions[0] = room[x_pos - 1][y_pos];
        myrobot->free_directions[1] = room[x_pos][y_pos - 1];
        myrobot->free_directions[2] = room[x_pos + 1][y_pos];
        myrobot->free_directions[3] = room[x_pos][y_pos + 1];

        myrobot->free_directions[0] = myrobot->free_directions[0] && myrobot->room[myrobot->x_pos - 1][myrobot->y_pos];
        myrobot->free_directions[1] = myrobot->free_directions[1] && myrobot->room[myrobot->x_pos][myrobot->y_pos - 1];
        myrobot->free_directions[2] = myrobot->free_directions[2] && myrobot->room[myrobot->x_pos + 1][myrobot->y_pos];
        myrobot->free_directions[3] = myrobot->free_directions[3] && myrobot->room[myrobot->x_pos][myrobot->y_pos + 1];
        break;
    case 3:
        myrobot->free_directions[0] = room[x_pos][y_pos - 1];
        myrobot->free_directions[1] = room[x_pos + 1][y_pos];
        myrobot->free_directions[2] = room[x_pos][y_pos + 1];
        myrobot->free_directions[3] = room[x_pos - 1][y_pos];

        myrobot->free_directions[0] = myrobot->free_directions[0] && myrobot->room[myrobot->x_pos][myrobot->y_pos - 1];
        myrobot->free_directions[1] = myrobot->free_directions[1] && myrobot->room[myrobot->x_pos + 1][myrobot->y_pos];
        myrobot->free_directions[2] = myrobot->free_directions[2] && myrobot->room[myrobot->x_pos][myrobot->y_pos + 1];
        myrobot->free_directions[3] = myrobot->free_directions[3] && myrobot->room[myrobot->x_pos - 1][myrobot->y_pos];
        break;
    default:
        break;
    }
    if ((myrobot->free_directions[0] == false) &&
    (myrobot->free_directions[1] == false) &&
    (myrobot->free_directions[2] == false) && 
    (myrobot->free_directions[3]) == false) {
        myrobot->cant_move = true;
        printf("Can't move!\n");
    } else {
        myrobot->cant_move = false;
    }
}

void move_forward(struct Robot *myrobot)
{
    char scan;
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
    printf("I moved forward!\n");
    print_room(myrobot->room);
    get_free_directions_from_Nils_sensors(myrobot);
    scanf("%c", &scan);
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
void go_go_spiral(struct Robot *myrobot)
{
    while ((myrobot->cant_move) == false) 
    {
        while ((myrobot->free_directions[3] == false) && ((myrobot->cant_move) == false))
        {
            if (myrobot->free_directions[0] == true)
            {
                move_forward(myrobot);
            }
            else
            {
                while ((myrobot->free_directions[0] != true) && ((myrobot->cant_move) == false))
                    turn_right(myrobot);
                move_forward(myrobot);
            }
        }
        if ((myrobot->cant_move) == true) {
            return;
        }           
        turn_left(myrobot);
        move_forward(myrobot);
    }
}


void set_field_visited(struct Robot *myrobot)
{
    myrobot->room[myrobot->x_pos][myrobot->y_pos] = false;
    myrobot->fields_visited++;
}

void print_coordinates(struct Robot *myrobot)
{
    printf("x: %d\ny: %d\ndir: %d\nvis_fields: %d\n", 
    myrobot->x_pos, myrobot->y_pos, myrobot->direction, myrobot->fields_visited);
}
void print_room(bool room[MAXLENGTH][MAXWIDTH])
{
    for (int x = 0; x < MAXLENGTH; x++)
    {
        for (int y = 0; y < MAXWIDTH; y++)
        {
            printf("%d", room[y][x]);
        }
        printf("\n");
    }
}

