// 음악 라이브러리

// 프로그램을 시작하면 화면에 프롬프트($)를 출력하고 사용자의 명령을 기다린다.
// 아래와 같은 기능을 제공하는 프로그램을 작성하라.
// 1. 파일로부터 데이터를 불러온다: open
// 1. 새로운 노래를 추가한다: add 
// 2. 이름과 제목으로 검색한다: find
// 3. 모든 노래를 출력한다: show
// 4. 번호로 노래를 삭제한다: delete 
// 5. 노래를 재생한다: play
// 6. 파일에 데이터를 저장한다: save as myPlaylist.txt
// 7. 프로그램을 종료한다: exit

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Header/string_tools.h"
#include "../Header/library.h"

#define BUFFER_LENGTH 200

void process_command();
void handle_open();
void handle_add();
void handle_find();
void handle_play();
void handle_save();
void handle_delete();
void handle_help();

int main(){
    initialize();
    handle_open();
    process_command();
    return 0;
}

void process_command(){
    char command_line[BUFFER_LENGTH];
    char *command;
    while (1){
        printf("$ "); // prompt

        if (read_line(stdin, command_line, BUFFER_LENGTH) <= 0) continue;

        command = strtok(command_line, " ");
        if (strcmp(command, "open") == 0)
            handle_open();
        else if (strcmp(command, "add") == 0)
            handle_add();
        else if (strcmp(command, "find") == 0)
            handle_find();
        else if (strcmp(command, "show") == 0)
            show();
        else if (strcmp(command, "play") == 0)
            handle_play();
        else if (strcmp(command, "delete") == 0)
            handle_delete();
        else if (strcmp(command, "save") == 0)
            handle_save();
        else if (strcmp(command, "help") == 0)
            handle_help();
        else if (strcmp(command, "exit") == 0)
            break;
    }
}

void handle_open(){
    char fileName[BUFFER_LENGTH];

    printf("Enter the playlist file name: ");
    if (read_line(stdin, fileName, BUFFER_LENGTH) <= 0)
        return;

    FILE *filePtr = fopen(fileName, "r");
    if (filePtr == NULL){
        printf("Failed to open file.\n");
        return;
    }
    if (open(filePtr) == 1)
        show();
    fclose(filePtr);
}

void handle_add(){
    char buffer[BUFFER_LENGTH];
    char *name = NULL, *title = NULL, *filePath = NULL;

    printf("Artist Name: ");
    if (read_line(stdin, buffer, BUFFER_LENGTH) > 0)
        name = strdup(buffer);

    printf("Title: ");
    if (read_line(stdin, buffer, BUFFER_LENGTH) > 0)
        title = strdup(buffer);

    printf("File Path: ");
    if (read_line(stdin, buffer, BUFFER_LENGTH) > 0)
        filePath = strdup(buffer);

    add_song(name, title, filePath);
}

void handle_find(){
    char buffer[BUFFER_LENGTH];
    char *name = NULL, *title = NULL;

    printf("Artist Name: ");
    if (read_line(stdin, buffer, BUFFER_LENGTH) > 0)
        name = strdup(buffer);

    printf("Title: ");
    if (read_line(stdin, buffer, BUFFER_LENGTH) > 0)
        title = strdup(buffer);

    find_song(name, title);
}

void handle_play(){
    char *buffer = NULL;
    buffer = strtok(NULL, " ");
    if (buffer == NULL){
        printf("Invalid command format.\n");
        return;
    }

    int index = atoi(buffer);
    play(index);
}

void handle_save(){
    char *buffer = NULL;
    buffer = strtok(NULL, " ");
    if (buffer == NULL || strcmp("as", buffer) != 0){
        printf("Invalid command format.\n");
        return;
    }

    buffer = strtok(NULL, " ");
    if (buffer == NULL){
        printf("Invalid command format.\n");
        return;
    }

    char *fileName = strdup(buffer);
    FILE *filePtr = fopen(fileName, "w");
    if (filePtr == NULL){
        printf("Failed to open file.\n");
        return;
    }
    save(filePtr);
    fclose(filePtr);
}

void handle_delete(){
    char *buffer = NULL;
    buffer = strtok(NULL, " ");
    if (buffer == NULL){
        printf("Invalid command format.\n");
        return;
    }

    int index = atoi(buffer);
    delete(index);
}

void handle_help(){
    printf("Commands:\n");
    printf("  open\n");
    printf("  show\n");
    printf("  add\n");
    printf("  delete [index]\n");
    printf("  play [index]\n");
    printf("  save as [file name]\n");
    printf("  exit\n\n");
}
