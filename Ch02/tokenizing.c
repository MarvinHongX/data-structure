#include <stdio.h>
#include <string.h>

int main(){
    char str[] = "now # is the time # to start preparing ### for the exams";
    char delimiter[] = "#";
    char *token;
    printf("before:%s\n", str);
    token = strtok(str, delimiter);
    while (token != NULL){
        printf("next token:%s(%zu)\n", token, strlen(token));
        token = strtok(NULL, delimiter);
    }
    printf("after:%s\n", str);
    return 0;
}