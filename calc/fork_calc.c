#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid;

    // 첫 번째 자식 - p1 실행
    pid = fork();
    if (pid == 0) {
        execl("./p1", "p1", "input.txt", NULL);
    }
    wait(NULL); // p1 종료 대기

    // 두 번째 자식 - p2 실행
    pid = fork();
    if (pid == 0) {
        execl("./p2", "p2", "input.txt", NULL);
    }
    wait(NULL); // p2 종료 대기

    return 0;
}

