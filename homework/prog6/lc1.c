#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int countNewline(char* buf);

int main (int argc, char *argv[])
{
	char* buf = malloc(1000);	// buffer to hold result from read()
	int fd;	// file descriptor for input files
	int totalLineCount = 0; // total count of lines in all input files	

	// check for proper number of arguments
	if (argc < 2)
	{
		fprintf(stderr, "Usage: %s [files]..", argv[0]);
		free(buf);
		exit(1);
	}

	// begin loop over all input files (argument 1 and greater...)
	int i = 0;
	for (i = 1; i < argc; i++)
	{
		int lineCount = 0; // line count for file
		// open input file
		fd = open(argv[i], O_RDONLY);
		if (fd < 0)
		{
			perror("Problem opening an input file:");
			free(buf);
			exit(1);
		}

		//read 1000 bytes at a time until whole file has been read
		int result = read(fd, buf, 1000);
		lineCount = countNewline(buf);
		while (result >= 999)
		{
			result = read(fd, buf, 1000);
			lineCount += countNewline(buf);
		}
		
		printf("%10d %-s\n", lineCount, argv[i]);
		totalLineCount += lineCount;
	}

	if (argc > 2) printf("%10d total\n", totalLineCount);

	free(buf);
	return totalLineCount;
}


int countNewline(char *buf)
{
	char *nextNewlineChar;
	int count = 0;
	while (1)
	{
		// find next occurence of newline character, exits when no more are found
		nextNewlineChar = strchr(buf, '\n');
		if (nextNewlineChar == NULL) return count;
		else
		{
			nextNewlineChar[0] = 1;
			count++;
		}
	}
}

