#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>

int main(){
    struct timeval seconds = {.tv_sec = 0, .tv_usec = 0};
    int i = 1;
    while (1)
    {
        while (seconds.tv_sec < i)
        {
            gettimeofday(&seconds, NULL);
        }
        printf("hello world at %d seconds\n", i);
        i ++;
        
        
        
    }
    return 0;
}