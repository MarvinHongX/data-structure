// phone book

// 프로그램을 시작하면 화면에 프롬프트($)를 출력하고 사용자의 명령을 기다린다.
// 아래와 같은 기능을 제공하는 프로그램을 작성하라.
// 1. 새로운 연락처를 추가한다: add [name] [phone number]
// 2. 이름으로 연락처를 검색한다: find [name]
// 3. 모든 연락처를 출력한다: show
// 4. 이름으로 연락처를 삭제한다: delete [name]
// 5. 프로그램을 종료한다: exit

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

int main(){
    char command[BUFFER_SIZE];
    while(1){
        printf("$ ");
        scanf("%s", command);

        if (strcmp(command, "add") == 0)
            add();
        else if (strcmp(command, "find") == 0)
            find();
        else if (strcmp(command, "show") == 0)
            show();
        else if (strcmp(command, "delete") == 0)
            delete();
        else if (strcmp(command, "exit") == 0)
            break;
    }
    return 0;
}

void add(){
    char buffer1[BUFFER_SIZE], buffer2[BUFFER_SIZE];
    scanf("%s", buffer1);
    scanf("%s", buffer2);

    names[n] = strdup(buffer1); //strdup vs strcpy
    numbers[n] = strdup(buffer2);
    n++;

    printf("%s was added successfully.\n", buffer1);
}
void find(){
    char buffer[BUFFER_SIZE];
    scanf("%s", buffer);

    for (int i = 0; i < n; i++){
        if (strcmp(buffer, names[i]) == 0){
            printf("%s\n", numbers[i]);
            return;
        }
    }
    printf("No results found\n");
}
void show(){
    for (int i = 0; i < n; i++)
        printf("%s  %s\n", names[i], numbers[i]);
}
void delete(){
    char buffer[BUFFER_SIZE];
    scanf("%s", buffer);

    for (int i = 0; i < n; i++){
        if (strcmp(buffer, names[i]) == 0){
            names[i] = names[n-1];
            numbers[i] = numbers[n-1];
            n--;

            printf("%s was deleted successfully.\n", buffer);
            return;
        }
    }
    printf("Nothing to delete.\n");
}
