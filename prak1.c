#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>

#include "prak1.h"


void create_process_fork(){

	for (int process = 0; process < NUMBER_OF_PROCESSES; process++){
		switch(fork()) {
			case -1:
				perror("Fork failed!!");
				exit(1);
			case 0:
				/* child process */
				printf("child process %d (%d) entered critical state!\n", process, getpid());
				sleep(1);
				printf("child process %d (%d) leaved critical state!\n", process, getpid());
				sleep(1);
			default: 
				/* father */
				;

		}
	}
	printf("father process has stopped!\n");

}
