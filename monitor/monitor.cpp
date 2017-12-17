// Created Time:    2017-12-10 11:38:55
// Modified Time:   2017-12-12 10:25:15

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

#define RET_ERROR   -1
#define RET_OK      0

/****************************子进程参数配置*********************************/
//子进程个数
#define CHILD_MAX_NUM       4
pid_t subPid[CHILD_MAX_NUM] = {0};



//PROCESS 1
const char *pro1_path   = "./process_1";
char *const pro1_argv[] = {/*"","","",*/NULL}; //注掉部分根据子进程实际入参情况修改,当前示例框架无入参演示,下同
//PROCESS 2
const char *pro2_path   = "./process_2";
char *const pro2_argv[] = {/*"","","",*/NULL};
//PROCESS 3

//PROCESS 4

/***************************************************************************/




//子进程的事件循环
void childCycle(int index);

//处理子进程的退出信号
void sub_quit_signal_handle(int sig);

//父进程的事件循环
void parentCycle(void);




void sub_quit_signal_handle(int sig)
{
    int index;
    int status;
    //获取退出的那个子进程的状态
    int quit_pid = wait(&status);
    printf("sub process %d quit,exit status %d\n",quit_pid, status);

    //利用子进程异常退出的pid查找创建子进程时对应的子进程索引
    for(index = 0; index < CHILD_MAX_NUM; index++)
    {
        if(quit_pid == subPid[index])
        {
            /*
             * 将发生异常退出的子进程的异常内容记录在日志当中,记录内容：时间,次数，子进程退出状态等内容
             */
            break;
        }
    }

    //新创建一个子进程
    pid_t pid = fork();
    if(0 == pid)
        //重新执行异常退出的子进程
        childCycle(index);
    //更新子进程表
    subPid[index] = pid;
}

void childCycle(int index)
{
    //通过子进程数组的索引来指定执行的子进程任务
    switch(index)
    {
        case 0:
            printf("I am the Process 1,pid = %d\n",getpid());
            if(RET_ERROR == execv(pro1_path,pro1_argv))
                perror("pro1_path");
            break;
        case 1:
            printf("I am the Process 2,pid = %d\n",getpid());
            if(RET_ERROR == execv(pro2_path,pro2_argv))
                perror("pro2_path");
            break;
        case 2:
            printf("I am the Process 3,pid = %d\n",getpid());
            /*
              if(RET_ERROR == execv(pro3_path,pro3_argv))
                perror("pro3_path");
            */
            break;
        case 3:
            printf("I am the Process 4,pid = %d\n",getpid());
            /*
              if(RET_ERROR == execv(pro4_path,pro4_argv))
                perror("pro4_path");
            */
            break;
    }
    while(1)
        pause();
}

void parentCycle()
{
    printf("parent process %d\r\n",getpid());
    signal(SIGCHLD,sub_quit_signal_handle);
    while(1)
       pause();
}


int main(int argc, char *argv[]) 
{
    int iter = 0;
    pid_t pid;

    for(iter = 0; iter < CHILD_MAX_NUM; iter++)
    {
       pid = subPid[iter] = fork();
        if(pid == 0 || pid < 0)
            break;
    }
    //创建子进程失败
    if(-1 == pid)
    {
        printf("No. %d: fork error\r\n",iter);
    }
    //子进程事件循环
    else if(pid == 0)
    {
        childCycle(iter);
    }
    //父进程事件循环
    else
    {
        parentCycle();
    }
    return 0;
}



