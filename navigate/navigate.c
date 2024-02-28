#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

bool* get_free_directions_from_Nils_sensors(uint8_t x_pos, uint8_t y_pos);
void move_forward();

int main() {
/* ein boolsches array mit 4 Richtungen, true - nächstes Feld in die Richtung ist frei, false - ist gesperrt
freedirections[0]=geradeaus
freedirections[1]=rechts
freedirections[2]=hinten
freedirections[3]=links
*/
    //Start position:
    uint8_t x_pos=0;
    uint8_t y_pos=0;
    bool room[10][10];
    for (int x=0;x<10;x++) {
        for (int y=0;y<10;y++) {

        }

    }

    bool* freedirections = get_free_directions_from_Nils_sensors(x_pos, y_pos);
    for (int i=0;i<4;i++) {
        printf("%d:%d, p:%p\n", i, freedirections[i], &freedirections[i]);
    }
    //We assume that we always begin in a corner
    while (freedirections[0] == true) {
        move_forward();
        x_pos++;
        freedirections = get_free_directions_from_Nils_sensors(x_pos, y_pos);
    }

    free(freedirections);   
        return 0;
}

bool* get_free_directions_from_Nils_sensors(uint8_t x_pos, uint8_t y_pos) {
    bool room[5][5] = {
    {true, false, true, true, true},   
    {true, true, true, true, true},      
    {true, true, true, true, true},      
    {false, true, true, true, true},      
    {true, true, true, true, true},         
    };
    bool* freedirections = malloc(4 * sizeof(bool));

    if (x_pos+1 > 4) //Check if the next field forward exists
        freedirections[0]=false;
    else
        freedirections[0]=room[x_pos+1][y_pos]; //get the value of the field

    if (y_pos+1 > 4) //Check if the next field to the right exists
        freedirections[1]=false;
    else
        freedirections[1]=room[x_pos][y_pos+1]; //get the value of the field

    if (x_pos-1 < 0) //Check if the next field back exists
        freedirections[2]=false;
    else
        freedirections[2]=room[x_pos-1][y_pos]; //get the value of the field    
    
    if (y_pos-1 < 0) //Check if the next field to the left exists
        freedirections[3]=false;
    else
        freedirections[3]=room[x_pos][y_pos-1]; //get the value of the field

        return freedirections;
}

void move_forward() {
    printf("I moved forward!\n");
}