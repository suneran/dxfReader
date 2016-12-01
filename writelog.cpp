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
    功能：     获取当前系统时间 
    返回值：    0-成功，-1-失败 
    out:        保存返回的系统时间，格式由fmt决定 
    fmt:        0-返回：yyyy-mm-dd hh24:mi:ss, 1-返回：yyyy-mm-dd, 2-返回：hh24:mi:ss 3-返回：yyyy-mm-dd-1, 4-返回：yyyy-mm-1,5-返回：yyyy-mm,
*/
int getTime(char *out, int fmt)                 // 获取当前系统时间 
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

//删除前一天日志，并打开今天日志
FILE* openFileEx(const char *fileName, const char *mode)  // 打开文本文件 
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


FILE* openFile(const char *fileName, const char *mode)  // 打开文本文件 
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
    功能：     将str写入到文件 
    返回值：    写文件成功返回0,否则返回-1 
    fp:     文件指针 
    str:        待写入的字符串 
    bLog:       1-是日志文件，0-不是日志文件 
    说明：     如果是日志文件，将会在str前加上当前时间(格式如：2011-04-12 12:10:20) 
*/
int writeFile(FILE *fp, const char *str, int bLog)          // 写字符串到文件,bLog表明是否为日志文件 
{ 
    char curTime[100] = {0}; 
    int ret = -1; 

	if(fp == NULL)
		return -1;

    
// 	wchar_t wc[2048];  
//     // 将ANSI编码的多字节字符串转换成宽字符字符串  
//     int n = MultiByteToWideChar(CP_ACP, 0, str, strlen(str), wc, 2048);  
//     if ( n > 0 )  
//     {  
//         wc[n] = 0;  
// 		
//         char mb[2048];  
//         // 将宽字符字符串转换成UTF-8编码的多字节字符串  
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

int writeFileW(FILE *fp,const wchar_t *str, int bLog)          // 写字符串到文件,bLog表明是否为日志文件 
{ 
	int ret = -1; 

	if(fp == NULL)
		return -1;

	int n = 0;
	{  
		char mb[2048];  
		// 将宽字符字符串转换成UTF-8编码的多字节字符串  
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
功能：     将str写入到文件 
返回值：    写文件成功返回0,否则返回-1 
fp:     文件指针 
str:        待写入的字符串 
bLog:       1-是日志文件，0-不是日志文件 
说明：     如果是日志文件，将会在str前加上当前时间(格式如：2011-04-12 12:10:20) 
*/
int writeFileEx(FILE *fp, const char *str, int bLog)          // 写字符串到文件,bLog表明是否为日志文件 
{ 

	char curTime[100] = {0}; 
	int ret = -1; 

	if(fp == NULL)
		return -1;

	if(bLog) // 获取当前系统时间 
	{ 
		getTime(curTime, 0); 
		ret = fprintf(fp, "[%s] %s\n", curTime, str); 
	} 
	else
		ret = fprintf(fp, "%s\n", str); 

	if(ret >= 0) 
	{ 
		fflush(fp); 
		return 0;               // 写文件成功 
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

