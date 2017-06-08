#include "USI_LOG_Config.h"

#define LOGNAME "./messages.txt"
FILE* g_fileHandle = NULL;
SYSTEMTIME systime;

VOID USI_LOG_CreateLog()
{
	g_fileHandle = fopen(LOGNAME, "a+");
	if ( g_fileHandle == NULL )
	{
        printf("open messages failed.\n");
	}
}

VOID print_debug(UINT8 *info)
{
    if ( NULL != g_fileHandle && NULL != info )
    {
        GetLocalTime(&systime);
		fprintf(g_fileHandle, "[%4d-%02d-%02d %02d:%02d:%02d][debug]%s\n", 
			systime.wYear, systime.wMonth, systime.wDay, systime.wHour, 
			systime.wMinute, systime.wSecond, info);
		fflush(g_fileHandle);
    }
}