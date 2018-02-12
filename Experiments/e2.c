#include <stdio.h>

void endlessRec(int count);

int main()
{
	endlessRec(0);
}

void endlessRec(int count)
{
	printf("Called %d times\n", count++);
	endlessRec(count);
}
