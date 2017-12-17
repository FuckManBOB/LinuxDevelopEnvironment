#ifndef SYS_LOG_H
#define SYS_LOG_H

#include <stdio.h>

#ifdef __cplusplus
extern "C"{
#endif

#define TRUE    1
#define FALSE   0
#define IN
#define OUT
#define LOG_PATH    "sys_log"

typedef enum{
    DEBUG,
    INFO,
    WARN,
    ERROR,
    OFF
}LOG_LEVEL;

void set_log_level(
    IN LOG_LEVEL level
);

char *log_level_2_str(
    IN LOG_LEVEL level
);

LOG_LEVEL log_str_2_level(
    IN const char *level
);

void write_log(
    IN LOG_LEVEL level,     //日志级别
    IN const char *file,    //打印日志所在的问文件
    IN const char *function,//打印日志的函数名
    IN int line,            //打印日志的行号
    IN const char *format,  //日志格式
    ...
);

void write_log_2_file(
    IN const char *file_name,   //需要保存的日志文件名
    IN LOG_LEVEL level,         //日志级别
    IN const char *file,        //打印日志所在的问文件
    IN const char *function,    //打印日志的函数名
    IN int line,                //打印日志的行号
    IN const char *msg
);

//设置日志路径
void set_log_path(IN const char *path);

//日志到标准输出
void log_terminal();

//日志关闭到标准输出
void log_shutdown_terminal();

//日志关闭写入到文件
void log_shutdown_file();

void log_to_file();

//日志管理
#define LOG_DEBUG(format, ...)  write_log(DEBUG, __FILE__, __FUNCTION__, __LINE__, format, ##__VA_ARGS__)
#define LOG_ERROR(format, ...)  write_log(ERROR, __FILE__, __FUNCTION__, __LINE__, format, ##__VA_ARGS__)
#define LOG_INFO(format, ...)   write_log(INFO, __FILE__, __FUNCTION__, __LINE__, format, ##__VA_ARGS__)
#define LOG_WARN(format, ...)   write_log(WARN, __FILE__, __FUNCTION__, __LINE__, format, ##__VA_ARGS__)

#ifdef __cplusplus
}
#endif



#endif
