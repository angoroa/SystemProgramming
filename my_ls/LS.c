#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>

void LS(const char *dirName, int longFlag, int recursiveFlag) {
    DIR *dp;
    struct dirent *dent;
    struct stat statbuf;

    // 디렉터리 열기
    if (dirName == NULL) {
        dp = opendir("."); // 현재 디렉터리
    } else {
        dp = opendir(dirName);
    }
    if (dp == NULL) {
        perror("디렉토리 열기");
        return;
    }

    // 현재 디렉터리 경로 출력
    if (recursiveFlag) {
        printf("\n%s:\n", dirName ? dirName : ".");
    }

    // 디렉터리 내 항목들 출력
    while ((dent = readdir(dp)) != NULL) {
        if (strcmp(dent->d_name, ".") == 0 || strcmp(dent->d_name, "..") == 0) {
            continue; // "."과 ".." 제외
        }

        // longFlag이 설정된 경우, 파일의 상세 정보 출력
        if (longFlag) {
            char filePath[1024];
            if (dirName == NULL) {
                snprintf(filePath, sizeof(filePath), "./%s", dent->d_name);
            } else {
                snprintf(filePath, sizeof(filePath), "%s/%s", dirName, dent->d_name);
            }
            if (stat(filePath, &statbuf) == -1) {
                perror("stat 함수");
                continue;
            }
            printf("%o\t", (unsigned int)statbuf.st_nlink); // 하드 링크 수
            printf("%d\t", (int)statbuf.st_size); // 파일 크기
            printf("%s\n", dent->d_name); // 파일 이름
        } else {
            printf("%s ", dent->d_name); // 파일 이름만 출력
        }
    }

    // 디렉터리 항목을 모두 출력한 후에 디렉터리인 경우만 재귀 호출
    rewinddir(dp); // 디렉터리 포인터를 다시 처음으로 되돌린다.
    while ((dent = readdir(dp)) != NULL) {
        if (strcmp(dent->d_name, ".") == 0 || strcmp(dent->d_name, "..") == 0) {
            continue; // "."과 ".." 제외
        }

        // 디렉터리인 경우 재귀적으로 그 디렉터리 내부 내용 출력
        if (recursiveFlag && dent->d_type == DT_DIR) {
            char subDir[1024];
            snprintf(subDir, sizeof(subDir), "%s/%s", dirName ? dirName : ".", dent->d_name);
            LS(subDir, longFlag, recursiveFlag); // 재귀 호출
        }
    }

    closedir(dp);
}

int main(int argc, char* argv[]) {
    int longFlag = 0;
    int recursiveFlag = 0;
    const char* dirName = NULL;
    int i;

    // 명령줄 인수 처리
    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-l") == 0) {
            longFlag = 1; // -l 옵션
        } else if (strcmp(argv[i], "-r") == 0) {
            recursiveFlag = 1; // -r 옵션
        } else {
            dirName = argv[i]; // 디렉터리 이름
        }
    }

    LS(dirName, longFlag, recursiveFlag); // LS 함수 호출
    printf("\n");
    return 0;
}

