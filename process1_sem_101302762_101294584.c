#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdlib.h>


int main() {
    key_t shmkey = ftok("shmfile", 65);
    int shmid = shmget(shmkey, 2 * sizeof(int), 0666 | IPC_CREAT);
    
    if (shmid < 0) {
        perror("shmget failed.");
        exit(1);
    }

    int *shared = (int *) shmat(shmid, NULL, 0);
    if (shared == (void *) -1) {
        perror("shmat failed");
        exit(1);
    }

    shared[0] = 3;
    shared[1] = 0;

    key_t semkey = ftok("semfile", 75);
    int semid = semget(semkey, 1, 0666 |IPC_CREAT);
    semctl(semid, 0, SETVAL, 1);

    struct sembuf P = {0, -1, 0};
    struct sembuf V = {0, 1, 0};

    pid_t pid = fork();

    if (pid == 0) {
        execl("./process2_sem", "process2_sem", NULL);
        perror("exec failed");
        exit(1);
    } else {
        int cycle = 0;
        while(1) {
            semop(semid, &P, 1);
            if (shared[1] > 500) {
                semop(semid, &V, 1);
                break;
            }
            if (shared[1] % shared[0] == 0) {
                printf("(Process 1) Cycle %d: %d is a multiple of %d\n", cycle, shared[1], shared[0]);
            } else {
                printf("(Process 1) Cycle %d\n", cycle);
            }
            shared[1]++;
            cycle++;
            semop(semid, &V, 1);
            usleep(200000);
        }
        shmdt(shared);
        shmctl(shmid, IPC_RMID, NULL);
        semctl(semid, 0, IPC_RMID);
        printf("Process 1 finished.\n");
   }

   return 0;
}