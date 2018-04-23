/**  smsh1.c  small-shell version 1
 **		first really useful version after prompting shell
 **		this one parses the command line into strings
 **		uses fork, exec, wait, and ignores signals
 **/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include "smsh.h"

#define	DFL_PROMPT	"> "

int main()
{
	char	*cmdline, **arglist;
	int	result;
	void	setup();
	char *prompt = malloc(1024);
	char *cwd = malloc(1022);
	cwd = getcwd(cwd, 1022);
	sprintf(prompt, "\033[32m%s: \033[0m", cwd);
	setup();

	while ( (cmdline = next_cmd(prompt, stdin)) != NULL ){
		if ( (arglist = splitline(cmdline)) != NULL  ){
			result = execute(arglist);
			freelist(arglist);
		}
		free(cmdline);
		cwd = getcwd(cwd, 1022);
		sprintf(prompt, "\033[32m%s: \033[0m", cwd);
	}
	return 0;
}

void setup()
/*
 * purpose: initialize shell
 * returns: nothing. calls fatal() if trouble
 */
{
  signal(SIGINT,  SIG_IGN);
  signal(SIGQUIT, SIG_IGN);
}

void fatal(char *s1, char *s2, int n)
{
  fprintf(stderr,"Error: %s,%s\n", s1, s2);
  exit(n);
}

