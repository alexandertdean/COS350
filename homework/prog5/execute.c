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
	int lastIndex = 0;
	int bool_runInBackground = 0;

	if ( argv[0] == NULL )		/* nothing succeeds	*/
	return 0;
	if (strcmp(argv[0], "exit") == 0)
	{
		int exitVal;
		if (argv[1] == NULL) exitVal = 0;
		else exitVal = atoi(argv[1]);
		exit(exitVal);
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
	
	int i = 0;
	while (1)
	{
		if (argv[i] == NULL)
		{
		 	lastIndex = i - 1;
			break;
		}
		i++;
	}

	if (strcmp(argv[lastIndex], "&") == 0)
	{
		bool_runInBackground = 1;
		argv[lastIndex] = NULL;
	}	
	
	if ( (pid = fork())  == -1 )					// FORK ERROR
		perror("fork");
	else if ( pid == 0 )							// CHILD PROCESS
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		execvp(argv[0], argv);
		perror("cannot execute command");
		exit(1);
	}
	else 											// PARENT PROCESS
	{
		if (!bool_runInBackground)
		{
			int npid = waitpid(-1, &child_info, 0);
			while (npid != pid)
			{
				printf("Done : %d\n", npid);
				npid = waitpid(-1, &child_info, 0);
			}
		}
	}
	return child_info;
}
