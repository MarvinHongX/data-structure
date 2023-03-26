// 다항식

// 프로그램을 시작하면 화면에 프롬프트($)를 출력하고 사용자의 명령을 기다린다.
// 아래와 같은 기능을 제공하는 프로그램을 작성하라.
// 1. 일원 다항식을 정의한다: f = 2x^5 + 3x - 4x^2 - 8
//    - 다항식의 이름은 x를 제외한 영문 소문자이다.
//    - 변수는 항상 x이다.
//    - 각 항의 지수는 음이 아닌 정수이고, 계수는 정수이다.
//    - = +, - 등의 앞 뒤로 하나 이상의 공백이 있을 수도 있다.
//    - 항들이 차수에 의한 내림차순으로 입력된다는 보장이 없으며, 동일 차수의 항이 여럿 있을 수 있다.
//    - 함수는 항상 내림차수로 정렬되어 저장된다.
// 2. 변수 x의 값이 주어질 때 다항식의 값을 계산한다: calc f 1
// 3. 함수를 출력한다: print g
// 4. 함수를 다시 정의할 수 있다: f = x^2 - x + 1
// 5. 이미 정의한 함수를 이용하여 함수를 정의할 수 있다: g = f + g
// 6. 프로그램을 종료한다: exit

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>


#define MAX_POLYS 100
#define BUFFER_SIZE 100

typedef struct term{
    int coeff; // 계수
    int expo;  // 지수(차수)
    struct term *next;
} Term;

typedef struct polynomial{
    char name; // single character
    Term *first;
    int size;
} Polynomial;

Polynomial *polys[MAX_POLYS];
int n = 0; // number of polynomial
char delimiter[] = " ";

void process_command();
int read_line(FILE *filePtr, char str[], int n);
int search(char name);
void handle_print(char name);
void print_poly(Polynomial *poly);
void print_term(Term *term);
void handle_definition(char *expression);
void erase_blanks(char *expression);
void insert_polynomial(Polynomial *poly);
void destroy_polynomial(Polynomial * poly);
Polynomial *create_by_parse_polynomial(char name, char *body);
int parse_and_add_term(char *expr, int begin, int end, Polynomial *poly);
Polynomial *create_by_add_two_polynomials(char name, char f1, char f2);
Polynomial *create_polynomial_instance(char name);
Term *create_term_instance(int coeff, int expo);
void add_term(int coeff, int expo, Polynomial *poly);
void handle_calc(char name, char *x);
int eval_poly(Polynomial *poly, int x);
int eval_term(Term *term, int x);
void add_poly_to_poly(Polynomial *source, Polynomial *target);

int main(){
    process_command();
    return 0;
}

void process_command(){
   char command_line[BUFFER_SIZE];
   char *command, *arg1, *arg2;
   char copied[BUFFER_SIZE];

    while(1){
        printf("$ ");
        if (read_line(stdin, command_line, BUFFER_SIZE) <= 0) continue;

        strcpy(copied, command_line); // 입력 라인을 복사해두고 handle_definition 에서 사용

        command = strtok(command_line, delimiter);
        if (command == NULL) continue;
        
        if (strcmp(command, "print") == 0){
            arg1 = strtok(NULL, delimiter);
            if (arg1 == NULL){
                printf("Invalid arguments.\n");
                continue;
            }
            handle_print(arg1[0]);
        } else if (strcmp(command, "calc") == 0){
            arg1 = strtok(NULL, delimiter);
            if (arg1 == NULL){
                printf("Invalid arguments.\n");
                continue;
            }
            arg2 = strtok(NULL, delimiter);
            if (arg2 == NULL){
                printf("Invalid arguments.\n");
                continue;
            }
            handle_calc(arg1[0], arg2);
        } else if (strcmp(command, "exit") == 0){
            break;
        } else {
            handle_definition(copied); // 다항식을 정의
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

int search(char name){
    for (int i = 0; i < n; i++){
        if (polys[i]->name == name){
            return i;
        }
    }
    return -1;
}

void handle_print(char name){
    int index = search(name);
    if (index < 0) {
        printf("Invalid function.\n");
        return;
    }
    Polynomial *poly = polys[index];
    print_poly(poly);
}

void print_poly(Polynomial *poly){ // 다항식을 출력
    printf("%c = ", poly->name);
    Term *current = poly->first;
    while (current != NULL){ // 연결리스트 탐색
        print_term(current);
        current = current->next;
        if (current != NULL && current->coeff >= 0)
            printf("+");
    }
    printf("\n");
}

void print_term(Term *term){ // 단일 항을 출력
    if (term->expo == 0){
        printf("%d", term->coeff);
    }else if (term->expo == 1){
        printf("%dx", term->coeff);
    }else {
        printf("%dx^%d", term->coeff, term->expo);
    }
}

void handle_definition(char *expression){
    erase_blanks(expression); // 모든 공백 문자를 제거

    char *f_name = strtok(expression, "=");
    if (f_name == NULL || strlen(f_name) != 1){
        printf("Unsupported command.\n");
        return;
    }
    char *exp_body = strtok(NULL, "=");
    if (exp_body == NULL || strlen(exp_body) <= 0){
        printf("Invalid expression format.\n");
        return;
    }

    if (exp_body[0] >= 'a' && exp_body[0] <= 'z' && exp_body[0] != 'x'){ // 이미 정의한 함수를 이용하여 함수를 정의
        char *f1 = strtok(exp_body, "+");
        if (f1 == NULL || strlen(f1) != 1){
            printf("Invalid expression format.\n");
            return;
        }
        char *f2 = strtok(NULL, "+");
        if (f2 == NULL || strlen(f2) != 1){
            printf("Invalid expression format.\n");
            return;
        }
        Polynomial *poly = create_by_add_two_polynomials(f_name[0], f1[0], f2[0]);
        if (poly == NULL){
            printf("Invalid expression format.\n");
            return;
        }
        insert_polynomial(poly);
    } else { // 함수를 다시 정의
        Polynomial *poly = create_by_parse_polynomial(f_name[0], exp_body);
        if (poly == NULL){
            printf("Invalid expression format.\n");
            return;
        }
        insert_polynomial(poly);
    }
}

void erase_blanks(char *expression){
    int runner = 0, current = 0;
    while (expression[runner] != '\0'){
        if (expression[runner] != ' ')
            expression[current++] = expression[runner];
        runner++;           
    }
    expression[current] = '\0'; // 반드시 null character 넣을 것
}

void insert_polynomial(Polynomial *poly){
    for (int i = 0; i < n; i++){
        if (polys[i]->name == poly->name){
            destroy_polynomial(polys[i]);
            polys[i] = poly;
            return;
        }
    }
    polys[n++] = poly;
}

void destroy_polynomial(Polynomial * poly){
    if (poly == NULL) return;

    Term *current = poly->first, *previous;
    while (current != NULL){
        previous = current;
        current = current->next;
        free(previous);
    }
    free(poly);
}

Polynomial *create_by_parse_polynomial(char name, char *body){
    Polynomial *poly = create_polynomial_instance(name);
    int i = 0, begin_term = 0;
    while (body[i] != '\0'){
        if (body[i] == '+' || body[i] == '-')
            i++;
        while (body[i] != '\0' && body[i] != '+' && body[i] != '-')
            i++;
        int result = parse_and_add_term(body, begin_term, i, poly);
        if (result == 0){
            destroy_polynomial(poly);
            return NULL;
        }
        begin_term = i;
    }
    return poly;
}

int parse_and_add_term(char *expr, int begin, int end, Polynomial *poly){
    int i = begin;
    int sign_coeff = 1, coeff = 0, expo = 1;
    if (expr[i] == '+'){
        i++;
    }else if (expr[i] == '-'){
        sign_coeff = -1;
        i++;
    }
    if (expr[i] == 'x'){ // 계수가 생략됬다면
        coeff = sign_coeff; // 생략된 계수를 정의한다. (뒷부분에서 x를 읽게하기 위해 i를 증가시키지 않는다)
    }else { // 계수가 있다면
        while (i < end && expr[i] >= '0' && expr[i] <= '9'){
            coeff = coeff * 10 + (int)(expr[i] - '0');
            i++;
        }
        coeff *= sign_coeff;    
    }

    if (i >= end){
        add_term(coeff, 0, poly);
        return 1;
    }
    if (expr[i] != 'x')
        return 0;
    i++;
    if (i >= end){
        add_term(coeff, 1, poly);
        return 1;
    }
    if (expr[i] != '^')
        return 0;
    i++;
    expo = 0;
    while (i < end && expr[i] >= '0' && expr[i] <= '9'){
        expo = expo * 10 + (int)(expr[i] - '0');
        i++;
    }
    add_term(coeff, expo, poly);
    return 1;
}

void add_term(int coeff, int expo, Polynomial *poly){
    if (coeff == 0) return;

    Term *current = poly->first, *previous = NULL;
    while (current != NULL && current->expo > expo){ // 현재 보고 있는 항의 차수가 삽입하려는 항의 차수보다 크다면 계속 탐색
        previous = current;
        current = current->next;
    }

    // 동일 차수의 항이 존재하면 기존 항의 계수만 변경
    if (current != NULL && current->expo == expo){ 
        current->coeff += coeff;
        if (current->coeff == 0){ // 계수가 0이 되면
            if (previous == NULL){ // current 가 첫번째 노드라면
                poly->first = current->next;
            }else{
                previous->next = current->next;
            }
            poly->size--;
            free(current);
        }
        return;
    }

    // 동일 차수의 항이 존재하지 않으면 새로운 항을 삽입 (previous와 current사이에 삽입)
    Term *t = create_term_instance(coeff, expo);

    if (previous == NULL){ // 새로 삽입하려는 항이 다항식 내에서 최고차 항이라면 맨 앞에 삽입
        t->next = poly->first;
        poly->first = t;
    } else { 
        t->next = current;
        previous->next = t;
    }
    poly->size++;
}

Polynomial *create_by_add_two_polynomials(char name, char f1, char f2){
    int index1 = search(f1);
    if (index1 < 0) {
        printf("Invalid function.\n");
        return NULL;
    }

    int index2 = search(f2);
    if (index2 < 0) {
        printf("Invalid function.\n");
        return NULL;
    }

    Polynomial *poly = create_polynomial_instance(name);
    
    Polynomial *poly1 = polys[index1];
    Polynomial *poly2 = polys[index2];


    add_poly_to_poly(poly1, poly);
    add_poly_to_poly(poly2, poly);

    return poly;
}

void add_poly_to_poly(Polynomial *source, Polynomial *target){
    Term *current = source->first;
    while (current != NULL){ // 연결리스트 탐색
        add_term(current->coeff, current->expo, target);
        current = current->next;
    }
}

Polynomial *create_polynomial_instance(char name){ 
    Polynomial *poly = (Polynomial *)malloc(sizeof(Polynomial));
    poly->name = name;
    poly->first = NULL;
    poly->size = 0;
    return poly;
}

Term *create_term_instance(int coeff, int expo){
    Term *t = (Term *)malloc(sizeof(Term));
    t->coeff = coeff;
    t->expo = expo;
    return t;
}


int eval_poly(Polynomial *poly, int x){ // 다항식의 값을 계산
    int result = 0;
    Term *current = poly->first;
    while (current != NULL){ // 연결리스트를 탐색
        result += eval_term(current, x); // 각각의 항의 값을 계산하여 더한다.
        current = current->next;
    }
    return result;
}

int eval_term(Term *term, int x){ // 단일 항의 값을 계산
    int result = term->coeff;

    // expo 가 양의 정수라는 가정하에 x를 expo번 거듭제곱 한다.
    for (int i = 0; i < term->expo; i++) 
        result *= x;
    return result;
}

void handle_calc(char name, char *x){
    int index = search(name);
    if (index < 0) {
        printf("Invalid function.\n");
        return;
    }

    int value = 0;
    char *endptr = NULL;
    errno = 0; // 에러 검출을 위한 변수 초기화

    value = strtol(x, &endptr, 10); 
    if (errno == ERANGE || endptr == x || *endptr != '\0') {
        printf("Invalid unknown number.\n");
        return;
    }  

    Polynomial *poly = polys[index];
    printf("%d\n", eval_poly(poly, atoi(x)));
}
