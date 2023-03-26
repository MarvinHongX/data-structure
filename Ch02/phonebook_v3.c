// phone book ver 3.0

// ver 2.0 에서 아래와 같은 기능을 추가한다.
// 1. 잘못된 명령어에 대해 적절하게 반응하도록 한다. (사용자의 입력을 라인 단위로 읽는다.)
// 2. 저장된 연락처가 배열의 용량을 초과하면 동적 메모리 할당으로 배열의 크기를 늘린다.

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define INIT_CAPACITY 3
#define BUFFER_SIZE 30

int capacity = INIT_CAPACITY;
char ** names;
char ** numbers;
int n = 0; //number of contacts
char delimiter[] = " ";

void init_directory();
void process_command();
int read_line(char str[], int limit);
void add(char *name, char *number);
void find(char *name);
void show();
void delete(char *name);
void open(char *fileName);
void save(char *fileName);
int search(char * name);
void reallocate();

int main(){
    init_directory();
    process_command();
 
    return 0;
}

void init_directory(){
    names = (char **)malloc(INIT_CAPACITY * sizeof(char *));
    numbers = (char **)malloc(INIT_CAPACITY * sizeof(char *));
}

void process_command(){
   char command_line[BUFFER_SIZE];
   char *command, *arg1, *arg2;
    while(1){
        printf("$ ");
        if (read_line(command_line, BUFFER_SIZE) <= 0) continue;

        command = strtok(command_line, delimiter);
        if (command == NULL) continue;
        
        if (strcmp(command, "open") == 0){
            arg1 = strtok(NULL, delimiter);
            if (arg1 == NULL){
                printf("File name required.\n");
                continue;
            }
            open(arg1);
            printf("%s was opened successfully.\n", arg1);
        } else if (strcmp(command, "add") == 0){
            arg1 = strtok(NULL, delimiter);
            arg2 = strtok(NULL, delimiter);

            if (arg1 == NULL || arg2 == NULL){
                printf("Invalid arguments.\n");
                continue;
            }
            add(arg1, arg2);
            printf("%s was added successfully.\n", arg2);
        } else if (strcmp(command, "find") == 0){
            arg1 = strtok(NULL, delimiter);
            if (arg1 == NULL){
                printf("Invalid arguments.\n");
                continue;
            }
            find(arg1);
        } else if (strcmp(command, "show") == 0){
            show();
        } else if (strcmp(command, "delete") == 0){
            arg1 = strtok(NULL, delimiter);
            if (arg1 == NULL){
                printf("Invalid arguments.\n");
                continue;
            }
            delete(arg1);
            printf("%s was deleted successfully.\n", arg1);
        } else if (strcmp(command, "save") == 0){
            arg1 = strtok(NULL, delimiter);
            arg2 = strtok(NULL, delimiter);

            if (arg1 == NULL || strcmp("as", arg1) != 0 || arg2 == NULL){
                printf("Invalid command format.\n");
                continue;
            }
            save(arg2);
            printf("%s was saved successfully.\n", arg2);
        } else if (strcmp(command, "exit") == 0){
            break;
        }
    }
}
int read_line(char str[], int limit){
    int ch, i = 0;
    while ((ch = getchar()) != '\n'){ // 줄바꿈 문자가 나올때까지 읽는다.
        if (i < limit - 1){ // null character를 추가할 한 자리를 남겨두는 범위내에서 배열에 저장
            str[i++] = ch;
        }
    }
    str[i] = '\0'; // null character 추가

    return i; // 실제로 읽은 문자수
}

void add(char *name, char *number){
    if (n >= capacity) // 배열이 가득찬 경우 재할당
        reallocate();

    int i = n - 1;
    for (; i >= 0 && (strcmp(names[i], name) > 0); i--){ // insertion sort
        names[i+1] = names[i];
        numbers[i+1] = numbers[i];
    }
    names[i+1] = strdup(name); //strdup vs strcpy
    numbers[i+1] = strdup(number);
    n++;
}

void find(char *name){
    int index = search(name);

    if (index == -1){
        printf("No results found\n");
        return;
    }

    printf("%s\n", numbers[index]);
}

void show(){
    for (int i = 0; i < n; i++)
        printf("%s  %s\n", names[i], numbers[i]);
}

void delete(char *name){
    int index = search(name);
    
    if (index == -1){
        printf("No results found\n");
        return;
    }

    for (int i = index; i < n-1; i++){
        names[i] = names[i+1];
        numbers[i] = numbers[i+1];
    }
    n--;
}

void open(char *fileName){
    char buffer1[BUFFER_SIZE], buffer2[BUFFER_SIZE];
    if (strcmp(fileName, "") == 0) return;

    FILE *filePtr = fopen(fileName, "r");
    if (filePtr == NULL){
        printf("Failed to open file.\n");
        return;
    }
    while (fscanf(filePtr, "%s", buffer1) != EOF){
        fscanf(filePtr, "%s", buffer2);
        add(buffer1, buffer2);
    }
    fclose(filePtr);
}

void save(char *fileName){
    if (n <= 0){
        printf("No data available.\n");
        return;
    }

    FILE *filePtr = fopen(fileName, "w");
    if (filePtr == NULL){
        printf("Failed to open file.\n");
        return;
    }
    for (int i = 0; i < n; i++)
        fprintf(filePtr, "%s %s\n", names[i], numbers[i]);
    fclose(filePtr);
}

int search(char * name){
    for (int i = 0; i < n; i++){
        if (strcmp(name, names[i]) == 0){
            return i;
        }
    }
    return -1;
}

void reallocate(){
    capacity *= 2;
    char **tmp1 = (char **)malloc(capacity * sizeof(char *));
    char **tmp2 = (char **)malloc(capacity * sizeof(char *));
    for (int i = 0; i < n; i++){
        tmp1[i] = names[i];
        tmp2[i] = numbers[i];
    }

    free(names);
    free(numbers);

    names = tmp1;
    numbers = tmp2;
}