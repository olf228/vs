#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <time.h>


#define PHILOSOPHS 5
#define HOME "/home/pi/vs"
#define ITERATIONS 3

pid_t waitpid(pid_t pid, int *status, int ops);
key_t sem_key;
int sem_id;
int sem_num;
struct sembuf semaphore;


// Struct for all philsoph data
typedef struct {
    int id;
    int eating_time;
    int think_time;
    int fork[2];
} philosoph;

// Leave the sempahore
void V(int sem_num){
    semaphore.sem_num=sem_num;
    semaphore.sem_op=1;
    semaphore.sem_flg=~(IPC_NOWAIT|SEM_UNDO);

    if(semop(sem_id, &semaphore, 1)){

        perror("Error in semop V()\n");
        exit(1);
    }
}

// Enter the sempahore
void P(int sem_num){
    semaphore.sem_num = sem_num;
    semaphore.sem_op = -1;
    semaphore.sem_flg = ~(IPC_NOWAIT|SEM_UNDO);

    if(semop(sem_id, &semaphore, 1)){

        perror("Error in semop P()\n");
        exit(1);
    }
}

// Initializes Semaphores and philosophes
void init_App(){

    // Create unique semaphore key
    if((sem_key = ftok(HOME, '1')) < 0){
        perror("Error in ftok\n");
        exit(1);
    }
    else{
        printf("Sem_key: %d\n", sem_key);
    }

    // Open semaphore group and creates one
    if((sem_id = semget(sem_key, 5, IPC_CREAT|0666)) < 0){
        perror("Error in semget\n");
        exit(1);
    }
    else{
        printf("Sem_ID: %d\n", sem_id);
    }

    for(int i = 0; i < 5; i++){
        if(semctl(sem_id, i, SETVAL, 1)<0){
            perror("Error in semctl\n");
            exit(1);
        }
    }
}

int main(){

    init_App();
    int id = 1;					// Init the id with a positive digit for the while loop
    int i = 0; 					// Index for the while loop
    while (id != 0 && i < PHILOSOPHS - 1){	// Somehow i need decrement the philosophs variable idk
        id = fork();
        if(id == -1) {
            printf("Error in fork!\n");
            exit(1);
        }

        else if (id == 0){
            // Child process path

        }

        else{
            // Father process path
            i++;
        }
    }

    // Init philosoph struct for current process
    philosoph p;

    // Init the random number generator with the current process id
    srand(i);

    // Init necessary variables for the philosoph "object"
    p.eating_time = (rand() % 10) + 1;
    p.think_time = (rand() % 10) + 1;

    // Assign 2 forks to the process
    p.fork[0] = i;
    p.fork[1] = (i + 1);
    if(i == PHILOSOPHS - 1){ // Special case: last philosopher has to take the first fork again
        p.fork[1] = 0;
    }
    printf("P%d: My forks are %d and %d!\n\n", i - 1, p.fork[0], p.fork[1]);
    //printf("P%d: I selected fork %d!\n", i, p.fork[1]);

    sleep(5);
    for(int j = 0; j < ITERATIONS; j++){
        /*if(i == PHILOSOPHS -1){
            P(p.fork[1]);
            printf("P%d: Took fork %d!\n", i, p.fork[1]);
            P(p.fork[0]);
            printf("P%d: Took fork %d!\n", i, p.fork[0]);
        }
        else{
            P(p.fork[0]);
            printf("P%d: Took fork %d!\n", i, p.fork[0]);
            P(p.fork[1]);                                // Some kind of timer is needed, that fork[0] is dropped, if the process can't get fork[1]
            printf("P%d: Took fork %d!\n", i, p.fork[1]);
        }*/
        // Think for the given time in the philosophs struct
        printf("P%d: I started thinking!\n\n", i);
        sleep(p.think_time);

        // Try to eat
        P(p.fork[0]);
        printf("P%d: Took fork %d!\n", i, p.fork[0]);

        P(p.fork[1]);                                // Some kind of timer is needed, that fork[0] is dropped, if the process can't get fork[1]
        printf("P%d: Took fork %d!\n", i, p.fork[1]);

        printf("P%d: I start to eat!\n\n", i);
        sleep(p.eating_time);

        V(p.fork[0]);
        printf("P%d: Released fork %d!\n", i, p.fork[0]);
        V(p.fork[1]);
        printf("P%d: Released fork %d!\n", i, p.fork[1]);
        printf("P%d: I stopped eating!\n", i);
        printf("I need to eat %d more times!\n\n", ITERATIONS - (1+j));

        // New random times for thinking and eating
        p.eating_time = rand() % 11;
        p.think_time = rand() % 12;
    }

    printf("P%d: I finished eating and thinking!\n\n", i);
    return 0;
}
