// phone book ver 2.0

// ver 1.0 에서 아래와 같은 기능을 추가한다.
// 1. 파일로부터 데이터를 불러온다: open phoneBook.txt
// 2. 항상 알파벳 순으로 정렬된 상태를 유지한다.
// 3. 파일에 데이터를 저장한다: save as phoneBook.txt

#include <stdio.h>
#include <string.h>
#define CAPACITY 100
#define BUFFER_SIZE 30

char * names[CAPACITY];
char * numbers[CAPACITY];
int n = 0; //number of contacts

void add();
void find();
void show();
void delete();
void open();
void save();
int search(char * name);

int main(){
    char command[BUFFER_SIZE];
    while(1){
        printf("$ ");
        scanf("%s", command);

        if (strcmp(command, "open") == 0)
            open();
        else if (strcmp(command, "add") == 0)
            add();
        else if (strcmp(command, "find") == 0)
            find();
        else if (strcmp(command, "show") == 0)
            show();
        else if (strcmp(command, "delete") == 0)
            delete();
        else if (strcmp(command, "save") == 0)
            save();
        else if (strcmp(command, "exit") == 0)
            break;
    }
    return 0;
}

void add(){
    char buffer1[BUFFER_SIZE], buffer2[BUFFER_SIZE];
    scanf("%s", buffer1);
    scanf("%s", buffer2);
    int i = n - 1;
    for (; i >= 0 && (strcmp(names[i], buffer1) > 0); i--){ // insertion sort
        names[i+1] = names[i];
        numbers[i+1] = numbers[i];
    }
    names[i+1] = strdup(buffer1); //strdup vs strcpy
    numbers[i+1] = strdup(buffer2);
    n++;

    printf("%s was added successfully.\n", buffer1);
}

void find(){
    char buffer[BUFFER_SIZE];
    scanf("%s", buffer);
    int index = search(buffer);

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

void delete(){
    char buffer[BUFFER_SIZE];
    scanf("%s", buffer);
    int index = search(buffer);
    
    if (index == -1){
        printf("No results found\n");
        return;
    }

    for (int i = index; i < n-1; i++){
        names[i] = names[i+1];
        numbers[i] = numbers[i+1];
    }
    n--;

    printf("%s was deleted successfully.\n", buffer);
}

void open(){
    char fileName[BUFFER_SIZE], buffer1[BUFFER_SIZE], buffer2[BUFFER_SIZE];
    scanf("%s", fileName);
    if (strcmp(fileName, "") == 0) {
        return;
    }

    FILE *filePtr = fopen(fileName, "r");
    if (filePtr == NULL){
        printf("Failed to open file.\n");
        return;
    }
    while (fscanf(filePtr, "%s", buffer1) != EOF){
        fscanf(filePtr, "%s", buffer2);
        names[n] = strdup(buffer1);
        numbers[n] = strdup(buffer2);
        n++;
    }
    fclose(filePtr);
    printf("%s was opened successfully.\n", fileName);
}

void save(){
    if (n <= 0){
        printf("No data available.\n");
        return;
    }
    char fileName[BUFFER_SIZE], buffer[BUFFER_SIZE];
    scanf("%s", buffer); // as
    scanf("%s", fileName);
    FILE *filePtr = fopen(fileName, "w");
    if (filePtr == NULL){
        printf("Failed to open file.\n");
        return;
    }
    for (int i = 0; i < n; i++)
        fprintf(filePtr, "%s %s\n", names[i], numbers[i]);
    fclose(filePtr);
    printf("%s was saved successfully.\n", fileName);
}

int search(char * name){
    for (int i = 0; i < n; i++){
        if (strcmp(name, names[i]) == 0){
            return i;
        }
    }
    return -1;
}