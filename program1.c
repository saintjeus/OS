#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>


//sigusr1 handler: output is day and time of the final exam
void SIGUSR1handler(int num){
	write(STDOUT_FILENO, "May 10, 8-10am\n",15);
}

//sigurs2 handler: output is notice of conflict, if any, with final exam
void SIGUSR2handler(int num){
	write(STDOUT_FILENO, "nah",3);
}

int main()
{
	signal(SIGUSR1, SIGUSR1handler);
	signal(SIGUSR2, SIGUSR2handler);
	while(1)
	{
		sleep(1);
	}
}
