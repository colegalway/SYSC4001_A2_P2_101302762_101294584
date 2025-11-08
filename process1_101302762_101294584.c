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
        execl("./process2_101302762_101294584", "process2_101302762_101294584", NULL);
        perror("exec failed");
        return 1;
    } else {
        int counter = 0;
        int cycle = 0;

        printf("Process 1 started... waiting for process 2 (child).\n");

        wait(NULL);
        printf("Process 2 finished... terminating process 1.\n");
   }
   return 0;
}