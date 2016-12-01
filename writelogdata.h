#ifndef WRITELOGDATA
#define WRITELOGDATA
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <io.h>

#pragma once


void RemoveLogFile(char * szLogFile);

/* 
    ���ܣ�     ��ȡ��ǰϵͳʱ�� 
    ����ֵ��    0-�ɹ���-1-ʧ�� 
    out:        ���淵�ص�ϵͳʱ�䣬��ʽ��fmt���� 
    fmt:        0-���أ�yyyy-mm-dd hh24:mi:ss, 1-���أ�yyyy-mm-dd, 2-���أ�hh24:mi:ss 3-���أ�yyyy-mm-dd-1, 4-���أ�yyyy-mm-1,5-���أ�yyyy-mm,
*/
int getTime(char *out, int fmt);                 // ��ȡ��ǰϵͳʱ�� 


//ɾ��ǰһ����־�����򿪽�����־
FILE* openFile(const char *fileName, const char *mode);  // ���ı��ļ� 

  
/* 
    ���ܣ�     ��strд�뵽�ļ� 
    ����ֵ��    д�ļ��ɹ�����0,���򷵻�-1 
    fp:     �ļ�ָ�� 
    str:        ��д����ַ��� 
    bLog:       1-����־�ļ���0-������־�ļ� 
    ˵����     �������־�ļ���������strǰ���ϵ�ǰʱ��(��ʽ�磺2011-04-12 12:10:20) 
*/
int writeFile(FILE *fp, const char *str, int bLog = 0);          // д�ַ������ļ�,bLog�����Ƿ�Ϊ��־�ļ� 

int writeFileW(FILE *fp,const wchar_t *str, int bLog = 0);

int closeFile(FILE *fp) ;


void WriteLogData(char * szLogData);


#endif