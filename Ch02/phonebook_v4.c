// phone book ver 4.0

// ver 3.0 에서 아래와 같은 기능을 추가한다.
// 1. 이름, 전화번호, 이메일 주소, 그룹을 지정할 수 있다. (#문자를 필드간의 구분자로 사용하며 이름을 제외한 항목들은 필수값이 아니다.)
// 2. 이름에 공백을 포함할 수 있다. (이름 사이의 여러 공백은 하나의 공백으로 저장한다.)

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define INIT_CAPACITY 100
#define BUFFER_SIZE 100

typedef struct contact { 
    char *name;
    char *number; 
    char *email;
    char *group;
} Contact;

Contact directory[INIT_CAPACITY];
int n = 0; //number of contacts
char delimiter[] = " ";


void process_command();
int read_line(FILE *filePtr, char str[], int n);
int compose_name(char str[], int limit);
void handle_add(char *name);
void add(char *name, char *number, char *email, char *group);
void find(char *name);
void show();
void delete(char *name);
void open(char *fileName);
void save(char *fileName);
int search(char * name);
void print_contact_header();
void print_contact_body(Contact contact);
void print_contact_footer();

int main(){
    process_command();
    return 0;
}

void process_command(){
   char command_line[BUFFER_SIZE];
   char *command, *arg;
   char name_str[BUFFER_SIZE];
    while(1){
        printf("$ ");
        if (read_line(stdin, command_line, BUFFER_SIZE) <= 0) continue;

        command = strtok(command_line, delimiter);
        if (command == NULL) continue;
        
        if (strcmp(command, "open") == 0){
            arg = strtok(NULL, delimiter);
            if (arg == NULL){
                printf("Invalid arguments.\n");
                continue;
            }
            open(arg);
        } else if (strcmp(command, "add") == 0){
            if (compose_name(name_str, BUFFER_SIZE) <= 0){
                printf("Name required.\n");
                continue;
            }

            handle_add(name_str);
            // printf("%s was added successfully.\n", arg2);
        } else if (strcmp(command, "find") == 0){
            if (compose_name(name_str, BUFFER_SIZE) <= 0){
                printf("Name required.\n");
                continue;
            }

            find(name_str);
        } else if (strcmp(command, "show") == 0){
            show();
        } else if (strcmp(command, "delete") == 0){
            if (compose_name(name_str, BUFFER_SIZE) <= 0){
                printf("Name required.\n");
                continue;
            }
            delete(name_str);
        } else if (strcmp(command, "save") == 0){
            arg = strtok(NULL, delimiter);
            if (arg == NULL || strcmp("as", arg) != 0){
                printf("Invalid command format.\n");
                continue;
            }
            arg = strtok(NULL, delimiter);
            if (arg == NULL){
                printf("Invalid command format.\n");
                continue;
            }
            save(arg);
        } else if (strcmp(command, "exit") == 0){
            break;
        }
    }
}
int read_line(FILE *filePtr, char str[], int n){
    int ch, i = 0;
    while ((ch = fgetc(filePtr)) != '\n' && ch != EOF)
        if (i < n)
            str[i++] = ch;

    str[i] = '\0'; // null character 추가
    return i; // 실제로 읽은 문자수
}

int compose_name(char str[], int limit){
    char *ptr;
    int length = 0;
    ptr = strtok(NULL, " ");

    if (ptr == NULL) return 0; 

    strcpy(str, ptr);
    length += strlen(ptr);
    while ((ptr = strtok(NULL, " ")) != NULL) {
        if (length + strlen(ptr) + 1 < limit) {
            str[length++] = ' ';
            str[length] = '\0';
            strcat(str, ptr);
            length += strlen(ptr);
        }
    }

    return length;
}

void handle_add(char *name) {
    char number[BUFFER_SIZE], email[BUFFER_SIZE], group[BUFFER_SIZE];
    char empty[] = " ";
    printf(" Phone: ");
    read_line(stdin, number, BUFFER_SIZE);
    
    printf(" Email: ");
    read_line(stdin, email, BUFFER_SIZE);
    
    printf(" Group: ");
    read_line(stdin, group, BUFFER_SIZE);

    add(name, (char *)(strlen(number)>0 ? number : empty),
                (char *)(strlen(email)>0 ? email : empty), 
                (char *)(strlen(group)>0 ? group : empty));
}
void add(char *name, char *number, char *email, char *group) {
    int i = n - 1;
    for (; i >= 0 && (strcmp(directory[i].name, name) > 0); i--){ // insertion sort
        directory[i+1] = directory[i];
    }
    directory[i+1].name = strdup(name);
    directory[i+1].number = strdup(number);
    directory[i+1].email = strdup(email);
    directory[i+1].group = strdup(group);
    n++;
}

void find(char *name){
    int index = search(name);

    if (index == -1){
        printf("No results found\n");
        return;
    }

    print_contact_header();
    print_contact_body(directory[index]);
    print_contact_footer();
}

void show(){
    print_contact_header();
    for (int i = 0; i < n; i++)
        print_contact_body(directory[i]);
    
    print_contact_footer();
}

void print_contact_header(){
    printf("+---------------------+---------------------+---------------------------+---------------------+\n");
    printf("| %-19s | %-19s | %-25s | %-19s |\n", "Name", "Number", "Email", "Group");
    printf("+---------------------+---------------------+---------------------------+---------------------+\n");
}

void print_contact_body(Contact contact){
    printf("| %-19s | %-19s | %-25s | %-19s |\n", contact.name, contact.number, contact.email, contact.group);
}

void print_contact_footer(){
    printf("+---------------------+---------------------+---------------------------+---------------------+\n");
}

void delete(char *name){
    int index = search(name);
    
    if (index == -1){
        printf("No results found\n");
        return;
    }

    // release_contact(index);

    for (int i = index; i < n-1; i++){
        directory[i] = directory[i+1];
    }
    n--;
    printf("%s was deleted successfully.\n", name);
}

void open(char *fileName){
    char buffer[BUFFER_SIZE];
    char *name, *number, *email, *group;
    if (strcmp(fileName, "") == 0) return;

    FILE *filePtr = fopen(fileName, "r");
    if (filePtr == NULL){
        printf("Failed to open file.\n");
        return;
    }
    while (1){
        if (read_line(filePtr, buffer, BUFFER_SIZE) <= 0) break;

        name = strtok(buffer, "#");
        number = strtok(NULL, "#");
        email = strtok(NULL, "#");
        group = strtok(NULL, "#");
        add(name, number, email, group);
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
    for (int i = 0; i < n; i++){    
        fprintf(filePtr, "%s#", directory[i].name);
        fprintf(filePtr, "%s#", directory[i].number);
        fprintf(filePtr, "%s#", directory[i].email);
        fprintf(filePtr, "%s#\n", directory[i].group);
    }
    fclose(filePtr);
}

int search(char * name){
    for (int i = 0; i < n; i++){
        if (strcmp(name, directory[i].name) == 0){
            return i;
        }
    }
    return -1;
}

