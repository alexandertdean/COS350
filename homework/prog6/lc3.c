#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

void *threadFunc(void *filename);
int countNewline(char* buf);

int main (int argc, char *argv[])
{
	int totalLineCount = 0;

	// check number of arguments
	if (argc < 2)
	{
		fprintf(stderr, "Usage: %s [files]...", argv[0]);
		exit(1);
	}
	pthread_t thread[argc];
	// loop through all files and make new thread for each file
	int i = 1;
	for (i = 1; i < argc; i++)
	{
		pthread_create(&thread[i], NULL , threadFunc, argv[i]);
	}

	for (i = 1; i < argc; i++)
	{
		void *retval;
		while (pthread_join(thread[i], &retval) != 0);
		totalLineCount += *((int*) retval);
	}
	printf("%10d total\n", totalLineCount);
	return totalLineCount;
}

void *threadFunc(void *filename)
{
	char buf[1000];
	int lineCount = 0; // line count for file
	// open input file
	int fd = open((char *)filename, O_RDONLY);
	if (fd < 0)
	{
		perror("Problem opening an input file:");
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
	
	printf("%10d %-s\n", lineCount, (char*) filename);
	pthread_exit(&lineCount);
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
