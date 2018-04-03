#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/signal.h>
#include <unistd.h>

struct itimerval old, new;

int count = 0;

static void handler()
{
	count++;
	struct timeval time;
	gettimeofday(&time, NULL);
	if (count % 100 == 0)
	{
		printf("%d %d\n", count++, time.tv_sec);
		fflush(stdout);
	}
}

 
int main()
{
	struct itimerval timer;

	timer.it_interval.tv_usec = 1000;
	timer.it_interval.tv_sec = 0;
	timer.it_value.tv_usec = 2;
	timer.it_value.tv_sec = 0;

	signal(SIGALRM, handler);

	setitimer(ITIMER_REAL, &timer, NULL);
	while(1)
		pause();
	
	timer.it_value.tv_sec = 0;
	timer.it_value.tv_usec = 0;
	setitimer(ITIMER_REAL, &timer, NULL);
}
