#include "../Header/string_tools.h"

int read_line(FILE *filePtr, char buffer[], int buffer_size){
    int ch, i = 0;
    while ((ch = fgetc(filePtr)) != '\n' && ch != EOF)
        if (i < buffer_size - 1)
            buffer[i++] = ch;

    buffer[i] = '\0';
    return i;
}