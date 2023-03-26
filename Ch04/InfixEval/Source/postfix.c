// 후위 표기식

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../Header/stack.h"

static char OPERATORS[] = "+-*/()";
static int PRECEDENCE[] = {1, 1, 2, 2, -1, -1};

Stack operator_stack;
Stack operand_stack;

int precedence(char op);
int is_operator(char ch); 
char *process_op(char op, char *pos);
int eval_op(char op);
void handle_exception(const char *message);


int precedence(char op){
    return PRECEDENCE[is_operator(op)];
}

int is_operator(char ch){
    for (int i = 0; i < strlen(OPERATORS); i++){
        if (OPERATORS[i] == ch){
            return i;
        }
    }
    return -1;
}

char *process_op(char op, char *pos){
    if (is_empty(operator_stack) || op == '('){
        push(operator_stack, op);
    } else {
        char top_op;
        if (!is_empty(operator_stack))
            top_op = peek(operator_stack);
        if (precedence(op) > precedence(top_op)){
            push(operator_stack, op);
        } else {
            while (!is_empty(operator_stack) && precedence(op) <= precedence(top_op)){
                pop(operator_stack);
                if (top_op == '(')
                    break;
                sprintf(pos, "%c ", top_op);
                pos += 2;
                if (!is_empty(operator_stack))
                    top_op = (char)peek(operator_stack);
            }
            if (op != ')')
                push(operator_stack, op);
        }
    }
    return pos;
}

char *convert_infix_to_postfix(char *infix){
    operator_stack = create();

    char *postfix = (char *)malloc(strlen(infix)+1);
    char *pos = postfix;

    char *token = strtok(infix, " "); // 모든 연산자와 피연산자가 공백문자로 구분되어 있다고 가정
    while (token != NULL){
        if (token[0] >= '0' && token[0] <= '9'){ // operand
            sprintf(pos, "%s ", token);
            pos += (strlen(token) + 1);
        } else if (is_operator(token[0]) > -1){ // operator
            pos = process_op(token[0], pos);
        } else {
            handle_exception("Syntax Error: invalid character encountered.");
        }
        token = strtok(NULL, " ");
    }
    while (!is_empty(operator_stack)){
        char op = (char)pop(operator_stack);
        if (op == '(')
            handle_exception("Unmatched parenthesis.");
        sprintf(pos, "%c ", op);
        pos += 2;
    }
    *pos = '\0';
    return postfix;
}

int eval(char *expression){
    operand_stack = create();
    char *token = strtok(expression, " ");
    while (token != NULL){
        if (token[0] >= '0' && token[0] <= '9'){
            int value = atoi(token);
            push(operand_stack, value);
        } else if (is_operator(token[0]) > -1){
            int result = eval_op(token[0]);
            push(operand_stack, result);
        } else {
            handle_exception("Syntax Error: invalid charater encountered.");
        }
        token = strtok(NULL, " ");
    }

    if (is_empty(operand_stack))
        handle_exception("Syntax Error: Stack empty in eval_op.");
    int answer = pop(operand_stack);
    if (is_empty(operand_stack)){
        return answer;
    } else {
        handle_exception("Syntax Error: Stack should be empty.");
        return -1;
    }
}

int eval_op(char op){
    if (is_empty(operand_stack))
       handle_exception("Syntax Error: Stack empty in eval_op."); 
    int rhs = pop(operand_stack);
    if (is_empty(operand_stack))
        handle_exception("Syntax Error: Stack empty in eval_op."); 
    int lhs = pop(operand_stack);
    int result = 0;
    switch (op) {
        case '+': result = lhs + rhs; break;
        case '-': result = lhs - rhs; break;
        case '*': result = lhs * rhs; break;
        case '/': result = lhs / rhs; break;
    }
    return result;
}

void handle_exception(const char *message){
    printf("%s\n", message);
    exit(1);
}

