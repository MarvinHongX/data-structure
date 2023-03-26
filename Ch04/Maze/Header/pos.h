#ifndef POS_H
#define POS_H

typedef struct pos {
    int x, y;
} Position;

Position move_to(Position position, int direction);

#endif // POS_H