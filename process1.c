#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    pid_t pid = fork();

    if (pid < 0 ) {
        perror("Fork failed");
        return 1;
    }

    if (pid == 0) {
        execl("./process2", "process2", NULL);
        perror("exec failed");
        return 1;
    } else {
        int counter = 0;
        int cycle = 0;

        while (1) {
            if (counter % 3 == 0) {
                printf("cycle %d - %d is a multiple of 3 (Process 1)\n)", cycle, counter);

            } else {
                printf("Cycle %d\n", cycle);
            }
            counter++;
            cycle++;
            usleep(50000);
        }
   }
}