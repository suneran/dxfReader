#include "StdAfx.h"
#include "writelogdata.h"
#include <stdio.h>  
#include <string.h>  
#include <Windows.h> 


//#define _DE_BUG_

void RemoveLogFile(char * szLogFile)
{
	FILE * fp = fopen(szLogFile,"rt");
	if( fp )
	{
		fclose(fp);
		remove(szLogFile);
	}
}

/* 
    ���ܣ�     ��ȡ��ǰϵͳʱ�� 
    ����ֵ��    0-�ɹ���-1-ʧ�� 
    out:        ���淵�ص�ϵͳʱ�䣬��ʽ��fmt���� 
    fmt:        0-���أ�yyyy-mm-dd hh24:mi:ss, 1-���أ�yyyy-mm-dd, 2-���أ�hh24:mi:ss 3-���أ�yyyy-mm-dd-1, 4-���أ�yyyy-mm-1,5-���أ�yyyy-mm,
*/
int getTime(char *out, int fmt)                 // ��ȡ��ǰϵͳʱ�� 
{ 
    time_t t; 
    struct tm *tp; 

	if(out == NULL) 
        return -1; 

    t = time(NULL); 
  
    tp = localtime(&t); 
    if(fmt == 0) 
        sprintf(out, "%2.2d-%2.2d-%2.2d %2.2d:%2.2d:%2.2d", tp->tm_year+1900, tp->tm_mon+1, tp->tm_mday, tp->tm_hour, tp->tm_min, tp->tm_sec); 
    else if(fmt == 1) 
        sprintf(out, "%2.2d-%2.2d-%2.2d", tp->tm_year+1900, tp->tm_mon+1, tp->tm_mday); 
    else if(fmt == 2) 
        sprintf(out, "%2.2d:%2.2d:%2.2d", tp->tm_hour, tp->tm_min, tp->tm_sec); 
	else if (fmt == 3)
		sprintf(out, "%2.2d-%2.2d-%2.2d", tp->tm_year+1900, tp->tm_mon+1, tp->tm_mday-1); 
	else if (fmt == 4)
		sprintf(out, "%2.2d-%2.2d", tp->tm_year+1900, tp->tm_mon); 
	else if (fmt == 5)
		sprintf(out, "%2.2d-%2.2d", tp->tm_year+1900, tp->tm_mon+1); 
    return 0; 
} 

//ɾ��ǰһ����־�����򿪽�����־
FILE* openFileEx(const char *fileName, const char *mode)  // ���ı��ļ� 
{ 
	char curTime[100] = {0}; 
	char LogFile[100] = {0};
	char LogFileToday[100] = {0};
	char * p = NULL;
	FILE *fp = NULL;
	errno_t err;

	strcpy(LogFile,fileName);
	strcpy(LogFileToday,fileName);

	CreateDirectory("c:\\jylog",NULL);

	p = strchr(LogFile,'.');
	if (p)
	{
		*p = '\0';
		if(getTime(curTime, 3)==0) 
			strcat(LogFile,curTime);
		strcat(LogFile,".txt");
	}
	RemoveLogFile(LogFile);

	p = strchr(LogFileToday,'.');
	if (p)
	{
		*p = '\0';
		if(getTime(curTime, 1)==0) 
			strcat(LogFileToday,curTime);
		strcat(LogFileToday,".txt");
	}

	//fp = fopen(LogFileToday, mode); 
	if( (err  = fopen_s( &fp,LogFileToday,mode)) !=0 )
		return NULL;
	else
		return fp;
} 


FILE* openFile(const char *fileName, const char *mode)  // ���ı��ļ� 
{ 

 	FILE *fp = NULL;
	errno_t err;
	RemoveLogFile((char*)fileName);
    //fp = fopen_s(fileName, mode); 
	if( (err  = fopen_s( &fp,fileName,mode)) !=0 )
		return NULL;
	else
		return fp;
} 
  
/* 
    ���ܣ�     ��strд�뵽�ļ� 
    ����ֵ��    д�ļ��ɹ�����0,���򷵻�-1 
    fp:     �ļ�ָ�� 
    str:        ��д����ַ��� 
    bLog:       1-����־�ļ���0-������־�ļ� 
    ˵����     �������־�ļ���������strǰ���ϵ�ǰʱ��(��ʽ�磺2011-04-12 12:10:20) 
*/
int writeFile(FILE *fp, const char *str, int bLog)          // д�ַ������ļ�,bLog�����Ƿ�Ϊ��־�ļ� 
{ 
    char curTime[100] = {0}; 
    int ret = -1; 

	if(fp == NULL)
		return -1;

    
// 	wchar_t wc[2048];  
//     // ��ANSI����Ķ��ֽ��ַ���ת���ɿ��ַ��ַ���  
//     int n = MultiByteToWideChar(CP_ACP, 0, str, strlen(str), wc, 2048);  
//     if ( n > 0 )  
//     {  
//         wc[n] = 0;  
// 		
//         char mb[2048];  
//         // �����ַ��ַ���ת����UTF-8����Ķ��ֽ��ַ���  
//         n = WideCharToMultiByte(CP_UTF8, 0, wc, wcslen(wc), mb, 2048, NULL, NULL);  
//         if ( n > 0 )  
//         {  
//             mb[n] = 0;  
//             fwrite(mb, sizeof(char), strlen(mb), fp);  
//         }  
//     }
	fputs(str,fp);
	ret = fprintf(fp, "\n"); 
	fflush(fp); 
 
	return 0;
	
	
} 

int writeFileW(FILE *fp,const wchar_t *str, int bLog)          // д�ַ������ļ�,bLog�����Ƿ�Ϊ��־�ļ� 
{ 
	int ret = -1; 

	if(fp == NULL)
		return -1;

	int n = 0;
	{  
		char mb[2048];  
		// �����ַ��ַ���ת����UTF-8����Ķ��ֽ��ַ���  
		n = WideCharToMultiByte(CP_UTF8, 0, str, wcslen(str), mb, 2048, NULL, NULL);  
		if ( n > 0 )  
		{  
			mb[n] = 0;  
			fwrite(mb, sizeof(char), strlen(mb), fp);  
		}  
	}

	ret = fprintf(fp, "\n"); 
	fflush(fp); 

	return 0;
} 


int closeFile(FILE *fp) 
{ 
    return fclose(fp); 
}



/* 
���ܣ�     ��strд�뵽�ļ� 
����ֵ��    д�ļ��ɹ�����0,���򷵻�-1 
fp:     �ļ�ָ�� 
str:        ��д����ַ��� 
bLog:       1-����־�ļ���0-������־�ļ� 
˵����     �������־�ļ���������strǰ���ϵ�ǰʱ��(��ʽ�磺2011-04-12 12:10:20) 
*/
int writeFileEx(FILE *fp, const char *str, int bLog)          // д�ַ������ļ�,bLog�����Ƿ�Ϊ��־�ļ� 
{ 

	char curTime[100] = {0}; 
	int ret = -1; 

	if(fp == NULL)
		return -1;

	if(bLog) // ��ȡ��ǰϵͳʱ�� 
	{ 
		getTime(curTime, 0); 
		ret = fprintf(fp, "[%s] %s\n", curTime, str); 
	} 
	else
		ret = fprintf(fp, "%s\n", str); 

	if(ret >= 0) 
	{ 
		fflush(fp); 
		return 0;               // д�ļ��ɹ� 
	} 
	else
		return -1; 
} 

void WriteLogData(char * szLogData){
#ifdef _DE_BUG_
	FILE * fp = openFileEx("c:\\jylog\\logfile.txt","a+");
	if(fp)
	{
		writeFileEx(fp,szLogData,1);
		closeFile(fp);
	}
#endif
}

