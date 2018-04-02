#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
	int i = 0;
	pid_t pid;
	for (i = 0; i < 4; i++)
	{
		pid = fork();
		if (pid == 0)
		{
			printf("%d\n", i);
			break;
		}
	}
	if (pid != 0) wait();
	return 0;
	
}
