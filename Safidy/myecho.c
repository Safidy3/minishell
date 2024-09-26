#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	printf("executed from myecho program\n");

	for (int i = 0; i < argc; i++)
		printf("%s\n", argv[i]);

	exit(EXIT_SUCCESS);
}