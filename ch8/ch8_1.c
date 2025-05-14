#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>

int main(){
	printf("Before SIGCONT Signal to Parent.\n");
	kill(getppid(), SIGCONT);

	printf("Before SIGQUIT Signal to me.\n");
	kill(getpid(), SIGQUIT);

	printf("AFTER SIGQUIT Signal.\n");





}

