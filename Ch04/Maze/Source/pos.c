#include "../Header/pos.h"

int offset[4][2] = { {-1, 0}, 
                     {0, 1},
                     {1, 0},
                     {0, -1}};

Position move_to(Position position, int direction){
    Position next;
    next.x = position.x + offset[direction][0];
    next.y = position.y + offset[direction][1];
    return next;
}