#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

#define passwordLength 20

void get_passwd(char *passwd);

int main (void)
{
	struct termios info;

	tcgetattr(0, &info);
	info.c_lflag &= ~(ECHO | ICANON);
	tcsetattr(0, TCSANOW, &info);
	printf("Enter password: ");

	char passwd[passwordLength];
	get_passwd(passwd);
	
	tcgetattr(0, &info);
	info.c_lflag |= (ECHO | ICANON);
	tcsetattr(0, TCSANOW, &info);
	printf("Got %s\n", passwd);

	return 0;
}

void get_passwd(char passwd[passwordLength])
{
	int i = 0;
	char c = 0;
	while ((c = getchar()) != EOF && c != '\n')
	{
		if (i < passwordLength - 1)
		{
			passwd[i++] = c;
			printf("*");
		}
	}
	passwd[passwordLength - 1] = 0;
	printf("\n");
}
