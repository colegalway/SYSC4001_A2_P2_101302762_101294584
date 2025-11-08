#include <stdio.h>
#include <unistd.h>

int main () {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        return 1;
    }

    if (pid == 0) { //child process
        int counter = 0;
        while (1) {
            printf("Process 2 (child): Counter %d\n", counter++);
            usleep(50000); //delay
        }
    } else {
        //parent process
        int counter = 0;
        while(1) {
            printf("Process 1 (parent): Counter %d\n", counter++);
            usleep(50000);
        }
    }
    return 0;


}