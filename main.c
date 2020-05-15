/**
 *  CURRENT TASK: 2
 *  MOVE TASK TO SEPERATE BRANCH ON COMPLETION
 */

#include <stdio.h>
#include "prakt1.h"

#define EATTIME 5


//id_t waitpid(pid_t pid, int *status, int ops);
key_t sem_key;
int sem_id;
int sem_num;
struct sembuf semaphore;

/* Leave the sempahore */
void V(int sem_num){
    semaphore.sem_num=sem_num;
    semaphore.sem_op=1;
    semaphore.sem_flg=~(IPC_NOWAIT|SEM_UNDO);

    if(semop(sem_id, &semaphore, 1)){

        perror("Error in semop V()");
        exit(1);
    }
}

/* Enter the sempahore */
void P(int sem_num){
    semaphore.sem_num = sem_num;
    semaphore.sem_op = -1;
    semaphore.sem_flg = ~(IPC_NOWAIT|SEM_UNDO);

    if(semop(sem_id, &semaphore, 1)){

        perror("Error in semop P()");
        exit(1);
    }
}

void init_sem(){

    // Create unique semaphore key
    if((sem_key = ftok(HOME, '1')) < 0){
        perror("Error in ftok");
        exit(1);
    }
    else{
        printf("Sem_key: %d\n", sem_key);
    }

    // Open semaphore group and creates 5 semaphores for each fork
    if((sem_id = semget(sem_key, 5, IPC_CREAT|0666)) < 0){
        perror("Error in semget");
        exit(1);
    }
    else{
        printf("Sem_ID: %d\n", sem_id);
    }

    // Configure all sempahores in the semaphore group (5 semaphores)
    for(int i = 0; i < 5; i++){
        if(semctl(sem_id, i, SETVAL, 1)<0){
            perror("Error in semctl");
            exit(1);
        }
    }
}


void main(){



    /*int pid[NUMBER_OF_PROCESSES];
    int status;

    init_sem();

    for (int process = 0; process < NUMBER_OF_PROCESSES; process++){
        switch(pid[process] = fork()) {
            case -1:
                perror("Fork failed!!");
                exit(1);
            case 0:
                // child process
                P(0);
                printf("child process %d (%d) entered critical state!\n", process, getpid());
                sleep(1);
                printf("child process %d (%d) leaved critical state!\n", process, getpid());
                V(0);
                sleep(1);
            default:
                // father
                waitpid(pid[process], &status, 0);

        }
    }
    printf("father process has stopped!\n");*/

}