#ifndef LIBRARY_H
#define LIBRARY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../Header/queue.h"
#include "../Header/pos.h"

void initialize();
bool open_maze();
void print_maze();
void solve_maze();
#endif // LIBRARY_H