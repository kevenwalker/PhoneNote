#include "USI_LOG_Config.h"

#define LOGNAME "./messages.txt"
#define LOGMAXLEN 1024
FILE* g_fileHandle = NULL;
SYSTEMTIME systime;
static INT8 buffer[LOGMAXLEN];

VOID USI_LOG_CreateLog()
{
	g_fileHandle = fopen(LOGNAME, "a+");
	if ( g_fileHandle == NULL )
	{
        printf("open messages failed.\n");
	}
}

VOID print_debug(INT8 *fmt, ...)
{
	/*2017.9.1:优化日志打印，增加日志中打印变量信息功能*/
	va_list arg;	
    if ( NULL != g_fileHandle)
    {
		va_start(arg, fmt);
		vsprintf(buffer, fmt, arg);
		va_end(arg);
		
        GetLocalTime(&systime);
		fprintf(g_fileHandle, "[%4d-%02d-%02d %02d:%02d:%02d][debug]%s\n", 
			systime.wYear, systime.wMonth, systime.wDay, systime.wHour, 
			systime.wMinute, systime.wSecond, buffer);
		fflush(g_fileHandle);
    }
}