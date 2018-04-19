/* execute.c - code used by small shell to execute commands */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

int execute(char *argv[])
/*
 * purpose: run a program passing it arguments
 * returns: status returned via wait, or -1 on error
 *  errors: -1 on fork() or wait() errors
 */
{
	int	pid ;
	int	child_info = -1;

	if ( argv[0] == NULL )		/* nothing succeeds	*/
	return 0;
	if (strcmp(argv[0], "exit") == 0)
	{
		printf("Exitted with value %d\n", atoi(argv[1]));
		exit(atoi(argv[1]));
	}
	else if (strcmp(argv[0], "cd") == 0)
	{
		if (argv[1] == NULL)
		{
			 if (chdir(getenv("HOME")) < 0) perror("cd: ");;
		}
		else
		{
			 if (chdir(argv[1]) < 0) perror("cd: ");
		}
		return 0;
	}

	if ( (pid = fork())  == -1 )
		perror("fork");
	else if ( pid == 0 )
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		execvp(argv[0], argv);
		perror("cannot execute command");
		exit(1);
	}
	else 
	{
		if ( wait(&child_info) == -1 )
			perror("wait");
	}
	return child_info;
}
