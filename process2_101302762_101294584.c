#include <stdio.h>
#include <unistd.h>

int main () {
    int counter = 0;
    int cycle = 0;

    while(counter >= -500) {
        if (counter % 3 == 0) {
            printf("Cycle %d - %d is a multiple of 3 (Process 2)\n", cycle, counter);

        } else {
            printf("Cycle %d\n", cycle);
        }
        counter--;
        cycle++;
        usleep(50000);
    }
    printf("Process 2 has reached %d, exiting now. \n", counter);
    return 0;
}