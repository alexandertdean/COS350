#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/*
 * Author: Alexander Dean
 * Date:   February 5, 2018
 *
 * Description: Simulates Unix expand command. Replaces tab characters ('\t') with specified amount of spaces
 *
 * Usage: myexpand -t [tab size]
 */


int main (int argc, char *argv[])
{
	int tabSize = 8;// multiples of this value are tab stops		
	char nextChar = 0;// stores the next character from stdin
	int bufPos = 0;//tracks position between tab spaces

	// check for correct number of command line arguments
	if (argc == 3)
	{
		if (!(strcmp(argv[1], "-t"))) tabSize = atoi(argv[2]);
		else 
		{
			fprintf(stderr, "Usage: %s -t [tab size]\n", argv[0]);
			return -1;
		}
	}
	else if (argc == 2 || argc > 3)
	{
		fprintf(stderr, "Usage: %s -t [tab size]\n", argv[0]);
		return -1;
	}
	
	char buf[tabSize];// stores the characters between tab stops

	nextChar = getchar();

	// read stdin until end of input file is reached
	while (nextChar != EOF)
	{
		if (nextChar == '\n')		
		{
			// print rest of line, then reset buffer
			int i;
			for (i = 0; i < bufPos; i++)
			{
				putchar(buf[i]);
			}
			putchar(nextChar);
			bufPos = 0;
		}
		else if (nextChar == '\t')
		{
			// fill remainder of buffer with spaces
			int i;
			for (i = bufPos; i < tabSize; i++)
			{
				buf[i] = ' ';
				bufPos++;
			}
		}
		else
		{
			// add character to buffer and increment
			buf[bufPos] = nextChar;
			bufPos++;
		}
	
		//buffer is full (next tab stop has been reached)
		//print buffer
		if (bufPos >= tabSize)
		{
			int i;
			for (i = 0; i < tabSize; i++)
			{
				putchar(buf[i]);
			}
			bufPos = 0;
		}
		nextChar = getchar();
	}
	return 0;
}

