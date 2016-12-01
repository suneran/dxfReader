#ifndef WRITELOGDATA
#define WRITELOGDATA
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <io.h>

#pragma once


void RemoveLogFile(char * szLogFile);

/* 
    功能：     获取当前系统时间 
    返回值：    0-成功，-1-失败 
    out:        保存返回的系统时间，格式由fmt决定 
    fmt:        0-返回：yyyy-mm-dd hh24:mi:ss, 1-返回：yyyy-mm-dd, 2-返回：hh24:mi:ss 3-返回：yyyy-mm-dd-1, 4-返回：yyyy-mm-1,5-返回：yyyy-mm,
*/
int getTime(char *out, int fmt);                 // 获取当前系统时间 


//删除前一天日志，并打开今天日志
FILE* openFile(const char *fileName, const char *mode);  // 打开文本文件 

  
/* 
    功能：     将str写入到文件 
    返回值：    写文件成功返回0,否则返回-1 
    fp:     文件指针 
    str:        待写入的字符串 
    bLog:       1-是日志文件，0-不是日志文件 
    说明：     如果是日志文件，将会在str前加上当前时间(格式如：2011-04-12 12:10:20) 
*/
int writeFile(FILE *fp, const char *str, int bLog = 0);          // 写字符串到文件,bLog表明是否为日志文件 

int writeFileW(FILE *fp,const wchar_t *str, int bLog = 0);

int closeFile(FILE *fp) ;


void WriteLogData(char * szLogData);


#endif