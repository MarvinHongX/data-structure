// phone book ver 5.0

// ver 4.0 에서 아래와 같은 기능을 추가한다.
// 배열 directory가 Contact 타입이 아니라 Contact에 대한 포인터 타입이 되도록 하라.
// (포인터라 할지라도 매개 변수 전달방식이 call by value임이 변하지는 않는다. 그러나 구조체를 복사하지 않고 주소값만 복사 하므로 복사되는 데이터 양이 줄어든다.)

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define INIT_CAPACITY 3
#define BUFFER_SIZE 100

typedef struct contact { 
    char *name;
    char *number; 
    char *email;
    char *group;
} Contact;

Contact **directory;
int capacity;
int n = 0; //number of contacts
char delimiter[] = " ";

void init();
void process_command();
int read_line(FILE *filePtr, char str[], int n);
int compose_name(char str[], int limit);
void handle_add(char *name);
void add(char *name, char *number, char *email, char *group);
void find(char *name);
void show();
void delete(char *name);
void release_contact(Contact *contact);
void open(char *fileName);
void save(char *fileName);
int search(char *name);
void print_contact_header();
void print_contact_body(Contact *contact);
void print_contact_footer();
void reallocate();


int main(){
    init();
    process_command();
    return 0;
}

void init(){
    directory = (Contact **)malloc(INIT_CAPACITY * sizeof(Contact *));
    capacity = INIT_CAPACITY;
    n = 0;
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
        
        if (strcmp(command, "add") == 0){
            if (compose_name(name_str, BUFFER_SIZE) <= 0){
                printf("Name required.\n");
                continue;
            }

            handle_add(name_str);
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

    add(strdup(name), strdup(number), strdup(email), strdup(group));
}

void add(char *name, char *number, char *email, char *group) {
    if (n >= capacity) reallocate();

    int i = n - 1;
    for (; i >= 0 && (strcmp(directory[i]->name, name) > 0); i--){ // insertion sort
        directory[i+1] = directory[i];
    }

    directory[i+1] = (Contact *)malloc(sizeof(Contact));

    directory[i+1]->name = name;
    directory[i+1]->number = number;
    directory[i+1]->email = email;
    directory[i+1]->group = group;
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

void print_contact_body(Contact *contact){
    printf("| %-19s | %-19s | %-25s | %-19s |\n", contact->name, contact->number, contact->email, contact->group);
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

    Contact *contact = directory[index];
    for (int i = index; i < n-1; i++){
        directory[i] = directory[i+1];
    }
    n--;
    release_contact(contact);
    printf("%s was deleted successfully.\n", name);
}

void release_contact(Contact *contact){
    free(contact->name);
    if (contact->number != NULL) free(contact->number);
    if (contact->email != NULL) free(contact->email);
    if (contact->group != NULL) free(contact->group);
    free(contact);
}

void open(char *fileName){
    char buffer[BUFFER_SIZE];
    char *name, *number, *email, *group;
    char *token;
    if (strcmp(fileName, "") == 0) return;
    
    FILE *filePtr = fopen(fileName, "r");
    if (filePtr == NULL){
        printf("Failed to open file.\n");
        return;
    }
    while (1){
        if (read_line(filePtr, buffer, BUFFER_SIZE) <= 0) break;

        name = strtok(buffer, "#");

        token = strtok(NULL, "#");
        if (token == NULL || strcmp(token, "") == 0){
            number = NULL;
        } else {
            number = strdup(token);
        }

        token = strtok(NULL, "#");
        if (token == NULL || strcmp(token, "") == 0){
            email = NULL;
        } else {
            email = strdup(token);
        }
        
        token = strtok(NULL, "#");
        if (token == NULL || strcmp(token, "") == 0){
            group = NULL;
        } else {
            group = strdup(token);
        }
        
        add(strdup(name), number, email, group);
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
        fprintf(filePtr, "%s#", directory[i]->name);
        fprintf(filePtr, "%s#", directory[i]->number);
        fprintf(filePtr, "%s#", directory[i]->email);
        fprintf(filePtr, "%s#\n", directory[i]->group);
    }
    fclose(filePtr);
}

int search(char *name){
    for (int i = 0; i < n; i++){
        if (strcmp(name, directory[i]->name) == 0){
            return i;
        }
    }
    return -1;
}

void reallocate(){
    capacity *= 2;
    Contact **tmp = (Contact **)malloc(capacity * sizeof(Contact *));
    for (int i = 0; i < n; i++)
        tmp[i] = directory[i];
    free(directory);
    directory = tmp; 
}
