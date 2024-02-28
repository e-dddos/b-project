#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#define MAXLENGTH 5
#define MAXWIDTH 5

struct Robot {
    uint8_t x_pos;
    uint8_t y_pos;
    uint8_t direction;
};

bool* get_free_directions_from_Nils_sensors(struct Robot* myrobot);
void move_forward(struct Robot* myrobot);
void print_coordinates(struct Robot* myrobot);

int main() {
    struct Robot myrobot;
    //Start position:
    myrobot.x_pos=0;
    myrobot.y_pos=0;
    /*Which direction the robot is looking at (When looking from the starting perspective).
    0 - forward
    1 - to the right
    2 - back
    3 - to the left*/
    myrobot.direction=0; 
    print_coordinates(&myrobot);
    bool room[MAXLENGTH][MAXWIDTH]; // Set the max room size
    for (int x=0;x<10;x++) { //First, all fields are closed and we're going to open them while going through them
        for (int y=0;y<10;y++) {
            room[y][x]=false;
        }
    }
    room[0][0]=true;     //We assume that we always begin at 0,0 coordinates
    /* ein boolsches array mit 4 Richtungen, true - nächstes Feld in die Richtung ist frei, false - ist gesperrt
    freedirections[0]=geradeaus
    freedirections[1]=rechts
    freedirections[2]=hinten
    freedirections[3]=links
    */
    bool* freedirections = get_free_directions_from_Nils_sensors(&myrobot);  
    while (freedirections[0] == true) {
        move_forward(&myrobot);
        freedirections = get_free_directions_from_Nils_sensors(&myrobot);
    }
    print_coordinates(&myrobot);

    free(freedirections);   
        return 0;
}

bool* get_free_directions_from_Nils_sensors(struct Robot* myrobot) {
    bool room[MAXLENGTH][MAXWIDTH] = {
    {true, false, true, true, true},   
    {true, true, true, true, true},      
    {true, true, true, true, true},      
    {false, true, true, true, true},      
    {true, true, true, true, true},         
    };
    bool* freedirections = malloc(4 * sizeof(bool));

    if (myrobot->x_pos+1 > 4) //Check if the next field forward exists
        freedirections[0]=false;
    else
        freedirections[0]=room[myrobot->x_pos+1][myrobot->y_pos]; //get the value of the field

    if (myrobot->y_pos+1 > 4) //Check if the next field to the right exists
        freedirections[1]=false;
    else
        freedirections[1]=room[myrobot->x_pos][myrobot->y_pos+1]; //get the value of the field

    if (myrobot->x_pos-1 < 0) //Check if the next field back exists
        freedirections[2]=false;
    else
        freedirections[2]=room[myrobot->x_pos-1][myrobot->y_pos]; //get the value of the field    
    
    if (myrobot->y_pos-1 < 0) //Check if the next field to the left exists
        freedirections[3]=false;
    else
        freedirections[3]=room[myrobot->x_pos][myrobot->y_pos-1]; //get the value of the field

        return freedirections;
}

void move_forward(struct Robot* myrobot) {
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
    //physically_moved_forward_func_from_Peter_and_Michael();
    printf("I moved forward!\n");
}

void print_coordinates(struct Robot* myrobot) {
    printf("x: %d\ny: %d\ndir: %d\n", myrobot->x_pos, myrobot->y_pos, myrobot->direction);
}