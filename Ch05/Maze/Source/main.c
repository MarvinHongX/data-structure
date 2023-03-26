// 미로찾기

// n x m 2차원 배열의 미로가 있다. 입구는 (0,0) 이고 출구는 (n-1, m-1) 이다. 
// 큐를 사용하여 미로를 찾을 수 있는지 확인하고 그 경로를 찾는 프로그램을 작성하라.

// 1. 큐에 (0,0)을 넣는다.
// 2. 큐가 빌 때까지 아래를 반복한다.
//    2-1) 큐에서 하나의 위치 p를 꺼낸다.
//    2-2) p에서 한 칸 떨어진 위치들 중에서 이동 가능하면서 아직 방문하지 않은 모든 위치들을 방문된 위치임을 표시하고 큐에 넣는다.
//    2-3) 그 위치가 출구라면 종료한다.

#include <stdio.h>
#include "../Header/library.h"

int main(){
    initialize();
    if (open_maze() == true){
        printf("Maze:\n");
        print_maze();
        printf("\n");
        solve_maze();
        print_maze();
    }

    return 0;
}
