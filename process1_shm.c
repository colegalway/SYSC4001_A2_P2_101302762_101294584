#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>


int main() {
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

    shared[0] = 3;
    shared[1] = 0;

    pid_t pid = fork();

    if (pid == 0) {
        execl("./process2_shm_101302762_101294584", "process2_shm_101302762_101294584", NULL);
        perror("exec failed");
        exit(1);
    } else {
        int cycle = 0;
        while(shared[1] <= 500) {
            if (shared[1] % shared[0] == 0) {
                printf("(Process 1) Cycle %d: %d is a multiple of %d\n", cycle, shared[1], shared[0]);
            } else {
                printf("(Process 1) Cycle %d\n", cycle);
            }
            shared[1]++;
            cycle++;
            usleep(200000);
        }
        shmdt(shared);
        shmctl(shmid, IPC_RMID, NULL);
        printf("Process 1 finished.\n");
   }

   return 0;
}