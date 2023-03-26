#include "../Header/library.h"

#define BUFFER_LENGTH 200
#define MAX 100
#define PATH 0  
#define WALL 1
#define VISITED 2
#define BACKTRACKED 3


int maze[MAX][MAX];
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
    Stack stack = create();
    Position current;
    current.x = 0;
    current.y = 0;

    int init_direction = 0; // 시도해 볼 방향

    while(1) {
        maze[current.x][current.y] = VISITED;
        if (current.x == rows - 1 && current.y == cols -1){
            printf("Found the path.\n");
            break;
        }
        bool forwarded = false;
        for (int direction = init_direction; direction < 4; direction++){
            if (movable(current, direction)){ // direction 으로 이동할 수 있는지 검사
                push(stack, direction);
                current = move_to(current, direction);
                forwarded = true;
                init_direction = 0;
                break;
            }
        }
        if (!forwarded){
            maze[current.x][current.y] = BACKTRACKED;
            if (is_empty(stack)){
                printf("No path exists.");
                break;
            }
            int d = pop(stack);
            current = move_to(current, (d+2)%4); // 현재 셀에 올 때 d방향으로 왔다면, 되돌아 갈 방향은 (d+2)%4
            init_direction = d + 1;
        }
    }
}


void print_maze(){
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (maze[i][j] == WALL)
                printf("# ");
            else if (maze[i][j] == PATH)
                printf("  ");
            else if (maze[i][j] == VISITED)
                printf(". ");
            else if (maze[i][j] == BACKTRACKED)
                printf("+ ");
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
    Position next = move_to(current, direction);

    if (next.x < 0 || next.x >= rows || next.y < 0 || next.y >= cols) {
        // 미로 범위를 벗어나는 경우
        return false;
    }

    if (maze[next.x][next.y] == WALL || maze[next.x][next.y] == VISITED || maze[next.x][next.y] == BACKTRACKED) {
        // 이동할 위치가 벽이거나 이미 방문한 적이 있는 위치거나 되돌아 온 적이 있는 위치인 경우
        return false;
    }

    return true;
}