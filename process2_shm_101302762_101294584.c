#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>

int main () {
    key_t key = ftok("shmfile", 65);
    int shmid = shmget(key, 2 * sizeof(int), 0666 | IPC_CREAT);
    
    if (shmid < 0) {
        perror("shmget failed.");
        exit(1);
    }

    int *shared = (int *) shmat(shmid, NULL, 0);
    if (shared == (void *) -1) {
        perror("shmat failed");
        exit(1);
    }

    int cycle = 0;

    while(shared[1] <= 100) {
        usleep(100000);
    }

    while(shared[1] <= 500) {
        if (shared[1] % shared[0] == 0) {
            printf("(Process 2) Cycle %d: %d is a multiple of %d\n", cycle, shared[1], shared[0]);
        } else {
            printf("(Process 2) Cycle %d\n", cycle);
        }
        
        cycle++;
        usleep(250000);
    }
    
    shmdt(shared);
    printf("Process 2 has finished. \n");
    return 0;
}