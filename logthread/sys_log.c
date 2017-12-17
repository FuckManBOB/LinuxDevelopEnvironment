/***********************************************************************
模块名称：C语言多级别日志模块的实现
模块功能：多线程环境下实现多级别日志的记录
模块描述：通过日志产生的时间，将之分别记录到通过时间命名的文件夹下，并可设定日志的输出级别
***********************************************************************/
#include <stdarg.h>
#include <time.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "sys_log.h"

extern char *get_thread_name();
void register_thread_name(IN pthread_t tid, IN const char *name);

char g_log_root[64] = {0};
char g_start_time[64] = {0};


//默认的日志级别
static LOG_LEVEL g_log_level = DEBUG;

static int g_log_terminal = FALSE;

static int g_log_to_file = TRUE;

void log_terminal()
{
    g_log_terminal = TRUE;
}

void log_shutdown_terminal()
{
    g_log_terminal = FALSE;
}

void log_shutdown_file()
{
    g_log_to_file = FALSE;
}

void log_to_file()
{
    g_log_to_file = TRUE;
}

void set_log_path(IN const char *path)
{
    if(path)
    {
        memcpy(g_log_root, path, strlen(path));
    }
}

void set_log_level(IN LOG_LEVEL level)
{
    g_log_level = level;
}

LOG_LEVEL log_str_2_level(IN const char *level)
{
    //level如果为空则默认为DEBUG
    if(!level)
    {
        return DEBUG;
    }

    if(strcmp("DEBUG", level) == 0)
    {
        return DEBUG;
    }

    if(strcmp("INFO", level) == 0)
    {
        return INFO;
    }

    if(strcmp("WARN", level) == 0)
    {
        return WARN;
    }

    if(strcmp("ERROR", level) == 0)
    {
        return ERROR;
    }

    if(strcmp("OFF", level) == 0)
    {
        return OFF;
    }

   return DEBUG;
}

char *log_level_2_str(IN LOG_LEVEL level)
{
    switch(level)
    {
    case DEBUG:
        return (char *)"DEBUG";
    case INFO:
        return (char *)"INFO";
    case WARN:
        return (char *)"WARN";
    case ERROR:
        return (char *)"ERROR";
    case OFF:
        return (char *)"OFF";
    default:
        return (char *)"UNKNOW";
    }

    return (char *)"UNKNOW";
}
void write_log(
    IN LOG_LEVEL level,     //日志级别
    IN const char *file,    //打印日志所在的问文件
    IN const char *function,//打印日志的函数名
    IN int line,            //打印日志的行号
    IN const char *format,  //日志格式
    ...
)
{
    if(level < g_log_level)
    {
        return;
    }

    if(!strlen(g_log_root))
    {
        //生成日志文件的名字
        memcpy(g_log_root, LOG_PATH, strlen(LOG_PATH));
        //assert(ret == 0)
    }

    //创建目录 如：sys_log
    //S_IRWXU:00700权限，代表该文件所有者拥有读，写和执行操作的权限
    //S_IRWXG:00070权限，代表该文件用户组拥有读，写和执行操作的权限
    //S_IROTH:00004权限，代表其他用户拥有可读的权限
    //S_IXOTH:00001权限，代表其他用户拥有执行的权限
    mkdir(g_log_root, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

    //构造年月日并在之前创建目录下继续创建该格式目录 如：sys_log/15-11-11
    time_t now = time(NULL);
    char timestamp[128] = {0};
    struct tm date;
    struct tm *ptr = localtime_r(&now, &date);
    //时间格式化输出年月日 注意：Y表示20xx y表示xx 并且m d必须小写
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d", ptr);    

    char forder[160] = {0};
    sprintf(forder, "%s/%s", g_log_root, timestamp);
    mkdir(forder, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

    //开始运行时间再在之前创建的目录下创建以时分秒格式目录 如：sys_log/15-11-11/15_30_49
    if(strlen(g_start_time) == 0)
    {
        memset(g_start_time, 0, sizeof(g_start_time));
        strftime(g_start_time, sizeof(timestamp), "%H_%M_%S", ptr);     //时间格式化输出时分秒
    }

    memset(forder, 0, sizeof(forder));
    sprintf(forder, "%s/%s/%s", g_log_root, timestamp, g_start_time);
    mkdir(forder,S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    

    char threadId[256] = {0};
    char *threadName = get_thread_name();
    if(!threadName)
    {
        //sprintf(threadId,"%s/thread.%11d.log", forder, pthread_self());
        sprintf(threadId,"%s/%s.log", forder, "test");
    }
    else
    {
        sprintf(threadId, "%s/thread.%s.log", forder, threadName);
    }

    va_list vap;
    va_start(vap, format);
    char msg[2048] = {0};
    vsnprintf(msg, sizeof(msg), format, vap);
    write_log_2_file(threadId, level, file, function, line, msg);
    va_end(vap);
}

void write_log_2_file(
    IN const char *file_name,   //需要保存的日志文件名
    IN LOG_LEVEL level,         //日志级别
    IN const char *file,        //打印日志所在的问文件
    IN const char *function,    //打印日志的函数名
    IN int line,                //打印日志的行号
    IN const char *msg
)
{
    FILE *fp = NULL;
    fp = fopen(file_name, "a+");

    if(!fp)
    {
        return;
    }

    //assert(fp);

    char log_info[4092] = {0};

    //获取当前系统时间
    time_t now = time(NULL);
    struct tm date;
    char timestamp[80] = {0};
    struct tm *ptr = localtime_r(&now, &date);
    strftime(timestamp, 100, "%B %d %Y %T", ptr);

    sprintf(log_info, "[%s|%s|%s:%d|%s]%s\n", timestamp,
     log_level_2_str(level), function, line, get_thread_name(),msg);

    if(g_log_to_file)
    {
        fwrite(log_info, sizeof(char), strlen(log_info), fp);
    }

    fclose(fp);
    if(g_log_terminal)
    {
        fprintf(stdout, (char *)log_info);
    }
}

