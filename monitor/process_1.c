// Created Time:    2017-12-10 12:51:22
// Modified Time:   2017-12-17 17:17:31

#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    char *strPtr = "hello world";
    char *ptr = NULL;
    int i = 0;
    
    for(i = 0; i < 20;i++)
    {
        printf("process 1 start \n");
        sleep(1);
    }
    //下面语句由于ptr为NULL，会发生coredump
    printf("strlen = %d\n",strlen(ptr));

    return 0;
}

