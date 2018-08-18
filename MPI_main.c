#include "MPI_main.h"

#define COLUMN_LIMIT 128

static UINT8 g_displaywelcominfo[][COLUMN_LIMIT]={
	"------------------------------------------",
	"----------------Phone Note----------------",
	"------------------------------------------"
};

static VOID MPI_ShowWelInfo(VOID* para)
{
	UINT8 iRet = 0;
	for (; iRet < (sizeof(g_displaywelcominfo) / COLUMN_LIMIT); iRet++)
	{
		printf("%s\n", g_displaywelcominfo[iRet]);
	}
}

INT main()
{
    DEBUG_ON("start software");
    USI_LOG_CreateLog();
	USI_DATE_ModuleInit();
	MPI_ShowWelInfo(g_displaywelcominfo);
	while (1)
    {
		USI_GUI_Operating();
	}
	USI_DATE_ModuleExit();
	return 0;
}
