#include "USI_LOG_Config.h"

#define LOGNAME "./messages.txt"
#define LOGMAXLEN 256
FILE* g_fileHandle = NULL;
SYSTEMTIME systime;
static INT8 buffer[LOGMAXLEN];

VOID USI_LOG_CreateLog()
{
	fopen_s(&g_fileHandle, LOGNAME, "a+");
	if ( g_fileHandle == NULL )
	{
        printf("open messages failed.\n");
	}
}

VOID USI_LOG_TRANS(INT8 *fmt, ...)
{
	/*2017.9.1:优化日志打印，增加日志中打印变量信息功能*/
	va_list arg;
	INT8 *tmpFmt = fmt;
	static INT8 uniformBuff[LOGMAXLEN]={0};
	INT funcLine = 0;
	INT8* func = NULL;
    if ( NULL != g_fileHandle)
    {
		va_start(arg, fmt);
		func = va_arg(arg, INT8*);
		funcLine = va_arg(arg, INT);
        vsprintf_s(buffer, sizeof(buffer), fmt, arg);
        va_end(arg);
		
        GetLocalTime(&systime);
		snprintf(uniformBuff, LOGMAXLEN, LOGMAXLEN - 1, "[%4d-%02d-%02d %02d:%02d:%02d][debug][%s][%d]%s",
			systime.wYear, systime.wMonth, systime.wDay, systime.wHour, 
			systime.wMinute, systime.wSecond, func, funcLine, buffer);
		
		fprintf(g_fileHandle, "%s\n", uniformBuff);
		fflush(g_fileHandle);
    }
}