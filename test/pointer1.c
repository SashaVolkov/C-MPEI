#include <stdio.h>


void swap(char*, char*);

int main(int argc, char *argv[])
{
	if (argc == 3){
		char x = *argv[1], y = *argv[2];
		printf("%s, %s\n", &x, &y);
		swap( argv[1], argv[2]);
		printf("%s, %s\n", argv[1], argv[1]);
	}
	return 0;
}

void swap(char *one, char *two){
	char temp;
	temp = *one;
	*one = *two;
	*two = temp;
}