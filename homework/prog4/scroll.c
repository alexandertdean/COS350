#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>

#define MAX_LINE_LENGTH 128

// global variable declaration, used to interval timer interrupt handler
char **lines;				// lines of input file
int currentLine = 0;		// current line in input file
struct winsize termSize;	// struct for holding terminal size information
struct itimerval timer;		// struct for setting interval timer
long timerInterval = 2000000;
long timerTime = 0;
long lines_allocated = 128;

static void handler();
void displayScreenful();

int main (int argc, char* argv[])
{
	// variable declaration
	FILE *fp; // file descriptor for input file
	struct termios termConfig, termConfig_orig;	// struct for holding terminal configuration information, and original configuration
	int i; // everyone's favorite for loop variable
	char input;	// character inputted by user for control
	int bool_timerOn = 0; // boolean to determine if timer is currently running

	// check for correct number of arguments
	if (argc != 2)
	{
		fprintf(stderr, "Usage: scroll [file]\n");
		exit(1);
	}
	
	// open source file
	fp = fopen(argv[1], "r");
	if (fp == NULL)
	{
		fprintf(stderr, "Error opening file %s\n", argv[1]);
		exit(1);
	}

	// get terminal size information
	if (ioctl(1, TIOCGWINSZ, &termSize) < 0)
	{
		printf("Unable to get terminal window size: %s\n", strerror(errno));
		exit(1);
	}


	// allocate array to hold lines of file
	lines = (char **)malloc(sizeof(char*)*lines_allocated);
	if (lines == NULL)
	{
		fprintf(stderr, "Not enough memory.\n");
		exit(1);
	}

	for (i = 0; 1; i++)
	{
		// check if there are more lines than originally accounted for
		if (i >= lines_allocated)
		{
			int new_size;
			
			// double array size and re-allocate
			new_size = lines_allocated * 2;
			lines = (char **)realloc(lines, sizeof(char*)*new_size);
			if (lines == NULL)
			{
				fprintf(stderr, "Not enough memory.\n");
				exit(1);
			}
			lines_allocated = new_size;
		}

		// allocate space for next line
		lines[i] = malloc(MAX_LINE_LENGTH);
		if (lines[i] == NULL)
		{
			fprintf(stderr, "Not enough memory.\n");
			exit(1);
		}
		if (fgets(lines[i], termSize.ws_col, fp) == NULL)
			break;

	}	
	lines_allocated = i;
	

	// turn echo and canonical stuff off
	if (tcgetattr(0, &termConfig) < 0)	// get current terminal configuration
	{
		printf("Unable to get terminal settings(0): %s\n", strerror(errno));
		exit(1);
	}
	if (tcgetattr(0, &termConfig_orig) < 0) // get current terminal configuration (again)
	{
		printf("Unable to get terminal settings(1): %s\n", strerror(errno));
		exit(1);
	}

	termConfig.c_lflag &= ~ECHO;	// disable echoing
	termConfig.c_lflag &= ~ICANON;  // disable canonical mode
	
	if (tcsetattr(0, TCSANOW, &termConfig) < 0)	// set terminal settings to changed settings
	{
		printf("Unable to set terminal settings: %s\n", strerror(errno));
		exit(1);
	}
	
	signal(SIGINT, SIG_IGN); // from now on, ignore interrupt (Ctrl + C), program will be exitted type typing q
	
	// start by displaying a screenful of the input file
	system("clear");
	fflush(stdout);
	system("clear");
	displayScreenful(lines, &currentLine, termSize);
	printf("\033[7m%s", argv[1]);
	input = getchar();
	for (i = 0; i < strlen(argv[1]); i++) printf("\b");
	printf("\033[0m");
	printf("\033[0J");
	while (input != 'q')
	{
		if (input == ' ')
		{
			displayScreenful(lines, &currentLine, termSize);
			printf(":");
		}
		else if (input == '\n')
		{
			if (!bool_timerOn)
			{
				// configure timer settings for 1 ms
				timer.it_interval.tv_usec = 1000;
				timer.it_interval.tv_sec = 0;
				timer.it_value.tv_usec = 2;
				timer.it_value.tv_sec = 0;
	
				// set interval timer to call handler()
				signal(SIGALRM, handler);

				bool_timerOn = 1;
				setitimer(ITIMER_REAL, &timer, NULL);
				printf("\033[7mScrolling 1 line per %3.2f seconds(s) \033[0m", ((double)timerInterval / 1000000));
			}
			else
			{
				// disable timer
				timer.it_interval.tv_usec = 0;
				timer.it_value.tv_usec = 0;
				setitimer(ITIMER_REAL, &timer, NULL);
				bool_timerOn = 0;
				timerTime = 0;
				printf("\033[2K\033[%d;0H:", termSize.ws_row);
			}
		}
		else if (input == 'f' && bool_timerOn)
		{
			// decrease timer interval
			timerInterval *= 0.8;
			printf("\033[2K\033[%d;0H", termSize.ws_row);
			printf("\033[7mScrolling 1 line per %3.2f second(s) \033[0m", ((double)timerInterval / 1000000));
		}
		else if (input == 's' && bool_timerOn)
		{
			// increase timer interval
			timerInterval *= 1.2;
			printf("\033[2K\033[%d;0H", termSize.ws_row);
			printf("\033[7mScrolling 1 line per %3.2f second(s) \033[0m", ((double)timerInterval / 1000000));
		}
		input = getchar();
		printf("\033[0J");
	}
	printf("\n");
	
	// put terminal settings back to original so the user isn't confused why they can't type anything
	tcsetattr(0, TCSANOW, &termConfig_orig);

	// clean up memory
	for (i = 0; i < lines_allocated; i++) free(lines[i]);
	free(lines);
	return 0;	//	returns 0 like a good little C program
}

void displayScreenful()
{	
	system("clear");
	fflush(stdout);
	system("clear");
	int i = 0;
	for (i = 0; (i < termSize.ws_row - 1) && ( currentLine < lines_allocated); i++)
	{
		printf("%s", lines[currentLine++]);
	}
}

static void handler()
{
	timerTime += 1000;
	if (timerTime >= timerInterval)
	{
		currentLine -= (termSize.ws_row - 2);
		displayScreenful();
		printf("\033[0J\033[07mScrolling 1 line per %3.2f second(s) \033[0m", (((double)timerInterval / 1000) / 1000));
		timerTime = 0;
	}
	fflush(stdout);
}
