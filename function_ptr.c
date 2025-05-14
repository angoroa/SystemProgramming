// 함수 포인터
#include <stdlib.h>
#include <stdio.h>
int  f1(int a) {return a+1;}
int main(){
	int (*f)(int a);
	f = f1;
	printf("%d", f(10));
}
