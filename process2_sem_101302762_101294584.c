#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

int main () {
    key_t shmkey = ftok("shmfile", 65);
    int shmid = shmget(shmkey, 2 * sizeof(int), 0666);
    
    if (shmid < 0) {
        perror("shmget failed.");
        exit(1);
    }

    int *shared = (int *) shmat(shmid, NULL, 0);
    if (shared == (void *) -1) {
        perror("shmat failed");
        exit(1);
    }

    key_t semkey = ftok("semfile", 75);
    int semid = semget(semkey, 1, 0666);

    struct sembuf P = {0, -1, 0};
    struct sembuf V = {0, 1, 0};


    int cycle = 0;

    while (1) {
        semop(semid, &P, 1);
        int counter = shared[1];
        semop(semid, &V, 1);
        if (counter > 100) {
            break;
        }
        usleep(100000);
    }

    while (1) {
        semop(semid, &P, 1);
        if (shared[1] > 500) {
            semop(semid, &V, 1);
            break;
        }
    
        if (shared[1] % shared[0] == 0) {
            printf("(Process 2) Cycle %d: %d is a multiple of %d\n", cycle, shared[1], shared[0]);
        } else {
            printf("(Process 2) Cycle %d\n", cycle);
        }
        semop(semid, &V, 1);
        cycle++;
        usleep(250000);
    }
    
    shmdt(shared);
    printf("Process 2 has finished. \n");
    return 0;
}