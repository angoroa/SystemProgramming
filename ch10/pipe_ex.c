#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    // 파일 디스크립터를 이용하기 때문에
    // 저수준 I/O 함수인 read()와 write()를 사용하는 것이다 !
    // pipe() 이용시 (한 쪽은 읽기_ 한 쪽은 쓰기)
    int fd[2];
    char msg[] = "Hello from Parent";
    pid_t pid;
    int status;
    ssize_t len;
    char buf[256];
    if (pipe(fd) == -1){
        perror("pipe");
        exit(1);
    }
    pid = fork();
    switch (pid){
        case -1:
            perror("fork");
            exit(1);
        case 0:
            // 자식 프로세스
            close(fd[1]);
            read(fd[0], buf, sizeof(buf));
            printf("Child received: %s\n", buf);
            len = read(fd[0], buf, 256);
            write(1, buf, len);
            close(fd[0]);
            break;
        default:
            // 부모 프로세스
            close(fd[0]);
            write(fd[1], msg, strlen(msg)+1);
            close(fd[1]);
            waitpid(pid, &status, 0);
            break;
    }

} 
