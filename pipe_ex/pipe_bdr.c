#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
int main() {
    // 파일 디스크립터를 이용하기 때문에
    // 저수준 I/O 함수인 read()와 write()를 사용하는 것이다 !
    // pipe() 이용시 (한 쪽은 읽기_ 한 쪽은 쓰기)
    int fd1[2]; // 부모(쓰기) -> 자식(읽기)
    int fd2[2]; // 자식(쓰기) -> 부모(읽기)
    
    pid_t pid;
    int status;
    ssize_t len;
    char buf[256];
    struct stat st;
    struct dirent *entry;
    DIR *dp;
    int count =0;

    if (pipe(fd1) == -1){
        perror("pipe1(부모->자식)");
        exit(1);
    }
    if (pipe(fd2)==-1){
        perror("pipe2(자식->부모)");
    }
    switch(pid = fork()){
        case -1:
            perror("fork");
            exit(1);
        case 0:
            // 자식 프로세스 (= 프로세스2)
            close(fd1[1]);
            close(fd2[0]);
            while(read(fd1[0], buf, sizeof(buf))>0){
                buf[strcspn(buf, "\n")]='\0';
                if(strcmp(buf, "//")==0){
                    write(fd2[1], &count, sizeof(count));
                    // write()는 count의 주소에서 count 바이트만큼 읽는 거다.
                    close(fd1[0]);
                    close(fd2[1]);
                    break;
                }
                else if(stat(buf, &st) == 0 && S_ISDIR(st.st_mode)){
                    dp = opendir(buf);
		    if (dp==NULL){
			perror("opendir");
			continue;
		    }
                        while((entry = readdir(dp)) != NULL){
                            if (entry->d_type == DT_REG){
                                count++;
                            }
                        }
                    closedir(dp);
                }
                
            }
            close(fd1[0]);
            break;
        default:
            // 부모 프로세스 (= 프로세스 1)
            close(fd1[0]);
            close(fd2[1]);
            while(fgets(buf, sizeof(buf), stdin) != NULL){
                buf[strcspn(buf, "\n")] = '\0';
                
                if (strcmp(buf, "//")==0){
                    write(fd1[1], buf, strlen(buf)+1);
                    break;
                }
                write(fd1[1], buf, strlen(buf)+1);
            }
            close(fd1[1]);
            wait(NULL);
            read(fd2[0], &count, sizeof(count));
            close(fd2[0]);
            printf("최종 파일 개수 : %d", count);
            break;
        }
 }
    

 
