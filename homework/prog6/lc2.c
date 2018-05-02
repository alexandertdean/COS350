#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>

int main (int argc, char *argv[])
{
	char * lc1 = "./lc1";	
	int totalLineCount = 0;
	char *newArgv[3];
	int pipefd[2];
	// check for appropriate number of arguments
	if (argc < 2)
	{
		fprintf(stderr, "Usage: %s [files]...", argv[0]);
		exit(1);
	}

	// become a plumber
	if (pipe(pipefd) < 0)
	{
		perror("Pipe's clogged");
	}

	// loop through all files and tell someone else to deal with it
	int i = 1;
	for (i = 1; i < argc; i++)
	{
		pid_t pid = fork();
		if (pid < 0)
		{
			perror("Forking forked up");
		}
		if (pid == 0)
		{
			// connect pipe to child process's sink
			while ((dup2(pipefd[1], STDOUT_FILENO) == -1) && (errno == EINTR));
			close (pipefd[1]);
			close (pipefd[0]);
			newArgv[0] = lc1;
			newArgv[1] = argv[i];
			newArgv[2] = NULL;
			if (execvp(lc1, newArgv) < 0) 
			{
				perror("A child process didn't wanna do it");
				exit(1);
			}
		}
	}
	close (pipefd[1]);
	int count = 0;
	char *buf = malloc(200);
	char *temp;
	for (i = 0; i < 200; i++)
	{
		buf[i] = 0;
	}
	
	// listen to what the children have to say
	// "wise words from the mouth of babes", as they say
	while (count != (argc - 1))
	{
		if (read(pipefd[0], buf, 200) > 0)
		{
			count++;
			printf("%s", buf);
			temp = buf;
			while (*buf == ' ') buf++;
			buf[strchr(buf, ' ') - buf] = 0;
			totalLineCount += atoi(buf);
			buf = temp;
			for (i = 0; i < 200; i++)
			{
				buf[i] = 0;
			}
		}
	}
	printf("%10d total\n", totalLineCount);

	return totalLineCount;
}

