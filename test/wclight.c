#include <stdio.h>

int main(int argc, char const *argv[])
{
	int c, nc, nw, nl, state;
	state = 0; nc = nw = 0; nl = 1;
	while((c = getchar()) != EOF){
		nc++;
		if (c == '\n')
		{
			++nl;
		}
		if (c == ' ' || c == '\t' || c =='\n')
		{
			state = 0;
		}
		else if (state == 0)
		{
			++nw;
			state = 1;
		}
	}
	printf("%d,%d,%d\n", nc, nw, nl);
	return 0;
}