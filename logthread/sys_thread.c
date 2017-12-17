#include <string.h>
#include <pthread.h>
#include "sys_log.h"


typedef struct tagTHREAD_INFO{
    char        name[64];
    pthread_t   tid;
}THREAD_INFO;

static THREAD_INFO g_thread_info[1024];
static int         g_thread_num = 0;

//用于将工程中创建的线程需要区分日志文件则在这里注册
void register_thread_name(
    IN pthread_t tid,
    IN const char *name
)
{
    memset((void *)&g_thread_info[g_thread_num], 0, sizeof(THREAD_INFO));
    g_thread_info[g_thread_num].tid = tid;
    memcpy((void *)g_thread_info[g_thread_num].name, (void *)name, strlen(name));
    g_thread_num++;
}

//获取当前执行线程的tid并在注册列表中查找有无注册信息有则返回线程名称用于后面创建日志文件名称使用
char *get_thread_name()
{
    int i = 0;
    pthread_t tid = pthread_self();
    for(; i < g_thread_num; i++)
    {
        if(tid == g_thread_info[i].tid)
        {
            return g_thread_info[i].name;
        }
    }

    return NULL;
}
