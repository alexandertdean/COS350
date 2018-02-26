#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int compare(const void*, const void*);

int main(int argc, char *argv[])
{
	int i;
	qsort(argv, argc, sizeof(*argv), compare);

	for (i = 0; i < argc; i++)
	{
		printf("%s\n", argv[i]);
	}
}

int compare(const void*  arg1, const void* arg2)
{
	if (strlen(*((char**)arg1)) < strlen(*((char**)arg2))) return -1;
	if (strlen(*((char**)arg1)) > strlen(*((char**)arg2))) return 1;
	else return 0;
}
