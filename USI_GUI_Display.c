#include "USI_GUI_Display.h"
#define COLUMN_LIMIT 128
#define AYYAYSIZE 32

UINT8 g_buffer[AYYAYSIZE] = {0};

static UINT8 g_displaywelcominfo[][COLUMN_LIMIT]={
	"------------------------------------------",
	"----------------Phone Note----------------",
	"------------------------------------------"
};

static UINT8 g_displayhelpinfo[][COLUMN_LIMIT]={
	" show         -- show all the contects details.",
	" create       -- create a new personal profile.",
	" clear        -- clean all contects from the phonenote.",
	" export       -- export all contects to the local machine.",
	" import       -- import all contects to the local machine.",
	" version      -- display the version of software.",
	" quit         -- exit the system."
};

typedef struct excute{
	UINT8 commandname[AYYAYSIZE];
	VOID (*fp)(UINT8*);
}EXCUTE_COMMAND;

USI_VOID USI_GUI_ShowWelInfo(VOID* para)
{
	UINT8 iRet = 0;
	for (; iRet < (sizeof(g_displaywelcominfo) / COLUMN_LIMIT); iRet++)
	{
		printf("%s\n", g_displaywelcominfo[iRet]);
	}
}

USI_VOID USI_GUI_ShowHelpInfo(VOID* para)
{
	UINT8 iRet = 0;
	for (; iRet < (sizeof(g_displayhelpinfo) / COLUMN_LIMIT); iRet++)
	{
		printf("%s\n", g_displayhelpinfo[iRet]);
	}
	print_debug("Show helpinfo display success.");
}

USI_VOID USI_GUI_Exit(UINT8* para)
{
    print_debug("System is poweroff.");
	exit(0);
}

USI_VOID USI_GUI_ShowALLContects(UINT8* para)
{
	USI_DATE_printContectList();
}

USI_VOID USI_GUI_ClearALLContects(UINT8* para)
{
	USI_DATE_cleanContectList();
}

USI_VOID USI_GUI_ExportALLContects(UINT8* para)
{
	USI_DATE_exportContect();
	printf("commond is running success!\n");
	print_debug("export contect info completely.");
}

USI_VOID USI_GUI_ImportALLContects(UINT8* para)
{
	UINT8 filename[BUFFER_LEN];
	INT iRet;
	printf("Please input contect File:");
	gets(filename);
	iRet = USI_DATE_importContect(filename);
	if (iRet == SUCCESS)
	{
		printf("Import Phone Note completely\n");
	}
	else
	{
		printf("Import Phone Note failed\n");
	}
}


USI_VOID USI_GUI_CreateContect(UINT8* para)
{
	CONTECT* newContect = NULL;
	newContect = USI_DATE_getNewContect();
	USI_DATE_modifyContect(newContect, NULL);
	printf("input contect is success!\n");
	print_debug("Create contect info completely.");
}

USI_VOID USI_GUI_DisplayVersion(UINT8* para)
{
	printf("Software Version:\n");
	printf("%s", SOFTVERSION);
	print_debug("Show Version is success.");
}

EXCUTE_COMMAND g_command[]={
	{"help", USI_GUI_ShowHelpInfo},
	{"create", USI_GUI_CreateContect},
	{"show", USI_GUI_ShowALLContects},
	{"clear", USI_GUI_ClearALLContects},
	{"export", USI_GUI_ExportALLContects},
	{"import", USI_GUI_ImportALLContects},
	{"version", USI_GUI_DisplayVersion},
	{"quit", USI_GUI_Exit}
};

USI_VOID USI_GUI_DispathCommand()
{
	UINT iRet = 0;
	UINT8 Flag = 0;
	UINT8 tmpStr[10]={0};
	iRet = USI_TOOL_CheckFirstParaIsValid(g_buffer);
	if (iRet == 0)
	{
		USI_TOOL_GetSpecificVarious(g_buffer, 2, tmpStr);
		for (;iRet < (sizeof(g_command) / sizeof(EXCUTE_COMMAND));iRet++)
		{
			if (strcmp(g_command[iRet].commandname, tmpStr) == 0)
			{
				Flag = 1;
				g_command[iRet].fp(NULL);
			}
		}
	}
	if (Flag == 0)
	{
		printf("Command is not support.Please see more details in <admin help> command.\n");
	}
}

VOID USI_GUI_Operating()
{
	system("cls");
	USI_GUI_ShowWelInfo(g_displaywelcominfo);
	USI_TOOL_DealWithInputInfo(g_buffer);
	USI_GUI_DispathCommand();
	getchar();
	//system("pause");
}
