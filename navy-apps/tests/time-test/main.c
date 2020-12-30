#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>

int main(){
    while (1)
    {
        struct timeval seconds = {.tv_sec = 0, .tv_usec = 0};
        gettimeofday(&seconds, NULL);
        int i = 0;
        if(seconds.tv_sec > i){
            printf("hello world at %d seconds!\n",i);
            i++;
        }
    }
    return 0;
}