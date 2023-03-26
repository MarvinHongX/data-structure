// infix eval

// infix 연산자를 사용자로부터 입력받고, postfix로 변환한다.
// 변환된 postfix를 평가하여 콘솔로 출력하는 프로그램을 작성하라.
// 단, 피연산자는 양의 정수라고 가정한다.
// 단, 연산의 결과는 정수라고 가정한다.
// 단, 연산자, 피연산자, 괄호는 한 칸의 공백으로 구분되어 입력받는다고 가정한다.

#include <stdio.h>
#include <string.h>
#include "../Header/postfix.h"
#include "../Header/string_tools.h"
#define MAX 100

int main(){
    printf("Enter the infix: ");
    char infix[MAX];
    read_line(stdin, infix, MAX);
    char *postfix = convert_infix_to_postfix(strdup(infix));
    printf("converted postfix: %s\n", postfix);
    printf("%s = %d\n", infix, eval(postfix));
    return 0;
}