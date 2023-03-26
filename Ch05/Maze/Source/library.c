#include "../Header/library.h"

#define BUFFER_LENGTH 200
#define MAX 100
#define PATH 0  
#define WALL 1

int maze[MAX][MAX];
int route[MAX][MAX];
int rows, cols;

int read_line(FILE *filePtr, char buffer[], int buffer_size);
bool movable(Position current, int direction);
void solve_maze();

void initialize(){
    for (int i = 0; i < MAX; i++)
        for (int j = 0; j < MAX; j++)
            maze[i][j] = 0;

    rows = 0;
    cols = 0;
}

bool open_maze(){
    char fileName[BUFFER_LENGTH];

    printf("Enter the maze file name: ");
    if (read_line(stdin, fileName, BUFFER_LENGTH) <= 0)
        return false;

    FILE *filePtr = fopen(fileName, "r");
    if (filePtr == NULL){
        printf("Failed to open file.\n");
        return false;
    }

    fscanf(filePtr, "%d %d", &rows, &cols);
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++){
            fscanf(filePtr, "%1d", &maze[i][j]);
        }
    }
       
    fclose(filePtr);

    return true;
}



void solve_maze(){
    Queue queue = create();
    Position current;
    current.x = 0;
    current.y = 0;

    enqueue(queue, current);
    maze[0][0] = -1;
    bool found = false;

    while(!is_empty(queue)) {
        Position current = dequeue(queue);
        for (int direction = 0; direction < 4; direction++){
            if (movable(current, direction)){
                Position position = move_to(current, direction);
                maze[position.x][position.y] = maze[current.x][current.y] - 1;
                if (position.x == rows - 1 && position.y == cols - 1){
                    printf("Found the path.\n");
                    found = true;
                    break;
                }
                enqueue(queue, position);
            }
        }
        if (found) break;
    }

    // fill in the route array
    Position pos = {rows - 1, cols - 1};
    while (pos.x != 0 || pos.y != 0) {
        route[pos.x][pos.y] = 1;
        if (pos.x > 0 && maze[pos.x - 1][pos.y] == maze[pos.x][pos.y] + 1) {
            pos.x--;
        } else if (pos.y > 0 && maze[pos.x][pos.y - 1] == maze[pos.x][pos.y] + 1) {
            pos.y--;
        } else if (pos.x < rows - 1 && maze[pos.x + 1][pos.y] == maze[pos.x][pos.y] + 1) {
            pos.x++;
        } else if (pos.y < cols - 1 && maze[pos.x][pos.y + 1] == maze[pos.x][pos.y] + 1) {
            pos.y++;
        }
    }
    route[0][0] = 1;
}


void print_maze(){
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (maze[i][j] == WALL){
                printf("# ");
            } else {
                if (route[i][j] == 1)
                    printf(". ");
                else
                    printf("  ");

            }
        }
        printf("\n");
    }
}

int read_line(FILE *filePtr, char buffer[], int buffer_size){
    int ch, i = 0;
    while ((ch = fgetc(filePtr)) != '\n' && ch != EOF)
        if (i < buffer_size - 1)
            buffer[i++] = ch;

    buffer[i] = '\0';
    return i;
}


bool movable(Position current, int direction){
    int dx = 0, dy = 0;
    switch (direction){
        case 0:  // Up
            dx = -1;
            break;
        case 1:  // Right
            dy = 1;
            break;
        case 2:  // Down
            dx = 1;
            break;
        case 3:  // Left
            dy = -1;
            break;
    }

    int new_x = current.x + dx;
    int new_y = current.y + dy;

    if (new_x < 0 || new_y < 0 || new_x >= rows || new_y >= cols)
        return false;
    if (maze[new_x][new_y] != PATH)
        return false;
    return true;
}