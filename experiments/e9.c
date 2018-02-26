#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main ()
{
	char *first;
	char *last;
	int i;
	int interval = 10;
	int j;
	
	for (j = 0; j < 7; j++)
	{
		for (i = 0; i < interval + 1; i++)
		{
			if (i == 0) first = malloc(4);
			else if (i == (interval)) last = malloc(4);
			else malloc(4);
		}
	printf("%d, %d\n", interval+1, last - first);
	interval *= 10;
	}
	return 0;
}
