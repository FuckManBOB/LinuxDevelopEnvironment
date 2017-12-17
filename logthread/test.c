// Created Time:    2017-12-12 16:47:26
// Modified Time:   2017-12-12 18:56:51

#include <stdio.h>
#include "sys_log.h"

void func(int x)
{
        LOG_INFO("world\n");
        sleep(1);
        LOG_ERROR("BOB\n");
        sleep(1);
        LOG_ERROR("cnt = %d\n",x);
}

int main(int argc, char *argv[]) {
        int cnt = 0;
        LOG_DEBUG("hello\n");
        sleep(1);
        while(1)
        func(cnt++);

    return 0;
}

