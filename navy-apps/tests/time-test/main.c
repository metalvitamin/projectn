#include <stdio.h>
#include <unistd.h>
#include <NDL.h>


int main(){
    NDL_Init(0);
    int i = 1;
    while (1)
    {
        while (NDL_GetTicks() / 1000 < i);
        printf("hello world at %d seconds\n", i);
        i ++;
        
        
        
    }
    return 0;
}