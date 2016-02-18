#include <stdio.h>
#define PERMS 0666



int main(int argc, char const *argv[]){
	FILE *fp;
	char msg[7] = "Privet!";

	creat("text.txt", PERMS);
	fp = fopen("text.txt", "w");
	fwrite(msg, sizeof(char), sizeof(msg),fp);
	fclose(fp);

	return 0;
}