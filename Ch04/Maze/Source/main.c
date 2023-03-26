// 미로찾기

// n x m 2차원 배열의 미로가 있다. 입구는 (0,0) 이고 출구는 (n-1, m-1) 이다. 
// 스택을 사용하여 미로를 찾을 수 있는지 확인하고 그 경로를 찾는 프로그램을 작성하라.

// 1. 이미 방문한 위치는 표시한다.
// 2. 현재 셀에서 인접한 셀을(동, 서 남, 북)을 검사하여 방문한적이 없거나, 벽이 아니면 그 셀로 이동한다.
// 3. 막다른 곳에 위치했다면 현재 셀의 직전 셀로 되돌아간다.

#include <stdio.h>
#include "../Header/library.h"

int main(){
    initialize();
    if (open_maze() == true){
        solve_maze();
        print_maze();
    }

    return 0;
}
