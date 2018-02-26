#include <stdio.h>
#include <unistd.h>

int main()
{
	FILE *fd;
	fd = fopen("e4output.txt", "w+");
	while (1)
	{
		fprintf(fd, "0");
		fflush(fd);
		usleep(1000);
	}
	return 0;
}
