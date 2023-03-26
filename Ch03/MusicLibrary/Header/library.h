#ifndef LIBRARY_H
#define LIBRARY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Header/string_tools.h"

typedef struct song Song;
typedef struct s_node SNode;
typedef struct artist Artist;

struct song {
    Artist *artist;
    char *title;
    char *filePath;
    int index;  // unique id
};

struct s_node {
    SNode *prev, *next;
    Song *song;
};

struct artist {
    char *name;
    struct artist *next; // pointer to the next artist
    SNode *head, *tail; // double linked list of s_node
};

void initialize();
void add_song(char *name, char *title, char *filePath);
void find_song(char *name, char *title);
void show();
int open(FILE *filePtr);
void play(int index);
void delete(int index);
void save(FILE *filePtr);

#endif // LIBRARY_H