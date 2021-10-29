/*
 * Jesus Barba, 9/9/21
 * Purpose of program: Program1 - creates signal handlers for SIGUSR1 and SIGUSR2 signals, main routine is an infinite loop
 */

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>

//sigusr1 handler: output is day and time of the final exam
void SIGUSR1handler(int num){
	write(STDOUT_FILENO, "OS1 final exam is on May 10, 8-10am\n",36);//output to USR1 signal
	signal(SIGINT, SIG_DFL);//signal reset
}

//sigurs2 handler: output is notice of conflict, if any, with final exam
void SIGUSR2handler(int num){
	write(STDOUT_FILENO, "nah i don't have a conflict with the final exam",50);// output for USR2 signal
	signal(SIGINT, SIG_DFL); //signal reset
}

int main()
{
	if (signal(SIGUSR1, SIGUSR1handler) == SIG_ERR)
		exit(1); //if sig err is raised, indicate error and exit with return value 1
	if (signal(SIGUSR2, SIGUSR2handler) == SIG_ERR)
		exit(1); //if sig err is raised, indicate error and exit with return value 1
	while(1)
	{
		sleep(1);
	}
}
