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
			printf("%d %d\n", i + 1, getpid());
			break;
		}
		if (pid != 0) wait(NULL);
	}
	if (pid != 0) printf("5 %d\n", getpid());
	return 0;
	
}
