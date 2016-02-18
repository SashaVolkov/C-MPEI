#include <sys/types.h>
#include <signal.h>
#include <stdio.h>

int count = 1;

void SigHndlr(int s){
	printf("\n I got SIGINT %d times\n", count++);
	if (count == 5){
		signal(SIGINT, SIG_DFL);
	}
}

int main(int argc, char const *argv[])
{

	signal(SIGINT, SigHndlr);
	while(1);
	return 0;
}