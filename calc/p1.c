#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <output file>\n", argv[0]);
        exit(1);
    }

    FILE *fp;
    char buf[100];

    // 사용자 입력
    printf("수식을 입력하세요 (예: 10 + 2 * 3 - 5):\n");
    fgets(buf, sizeof(buf), stdin);
    buf[strcspn(buf, "\n")] = '\0';

    // 파일 저장
    fp = fopen(argv[1], "w");
    if (fp == NULL) {
        perror("fopen");
        exit(1);
    }

    fprintf(fp, "%s", buf);
    fclose(fp);

    return 0;
}

