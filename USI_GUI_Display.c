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
	"              -- <name=?> specfic the feature to search.",
	" create       -- create a new personal profile.",
	" clear        -- clean all contects from the phonenote.",
	" delete       -- same as clear",
	"              -- <name=?> specfic the feature to delete.",
	" export       -- export all contects to the local machine.",
	" import       -- import all contects from the local machine.",
	" version      -- display the version of software.",
	" quit         -- exit the system."
};

typedef struct excute{
	UINT8 commandname[AYYAYSIZE];
	VOID (*fp)(UINT8*);
}EXCUTE_COMMAND;

typedef struct subexcute{
	UINT8 commandname[AYYAYSIZE];
	UINT8 subcommandname[AYYAYSIZE];
	VOID (*fp)(UINT8*);
}EXCUTE_SUBCOMMAND;


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

USI_VOID USI_GUI_ShowSpecficContects(UINT8* para)
{
	UINT8* key = NULL;
	UINT8* value = NULL;
	UINT8* tmpPara = para;
	if (para == NULL  || strlen(para) == 0)
	{
		print_debug("get the para is error");
	    return;
	}
	key = (UINT8*)malloc(strlen(para) + 1);
	if (key == NULL)
	{
		print_debug("alloc the key space is failed");
		return ;
	}
	value = (UINT8*)malloc(strlen(para) + 1);
	if (value == NULL)
	{
		print_debug("alloc the value space is failed");
		return ;
	}
	print_debug("entry the function para is {%s}", para);
	USI_TOOL_GetSpecificString(para, key, EQUALBEFORE);
	USI_TOOL_GetSpecificString(para, value, EQUALAFTER);
	if (*key == 0 || *value == 0)
	{
		print_debug("get the key and value failed");
		free(key);
		free(value);
		printf("command is execute failed of formate is invalid.\n");
		return;
	}
	print_debug("get the key {%s} value {%s}", key, value);
	USI_DATE_printSpecficContect(key, value);
	free(key);
	free(value);
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

/*-------------------------------------------------------
函数名:USI_GUI_DelSpecficContect
输入参数:para----输入需要处理的字符串对象
输出参数:无
说明:实现对指定对象进行删除操作的入口
函数创建时间:2018.5.27
-------------------------------------------------------*/
USI_VOID USI_GUI_DelSpecficContect(UINT8* para)
{
	UINT8* key = NULL;
	UINT8* value = NULL;
	UINT8* tmpPara = para;
	if (para == NULL  || strlen(para) == 0)
	{
		print_debug("get the para is error");
	    return;
	}
	
	key = (UINT8*)malloc(strlen(para) + 1);
	if (key == NULL)
	{
		print_debug("alloc the key space is failed");
		return ;
	}
	memset(key, 0, strlen(para) + 1);
	
	value = (UINT8*)malloc(strlen(para) + 1);
	if (value == NULL)
	{
		print_debug("alloc the value space is failed");
		return ;
	}
    memset(value, 0, strlen(para) + 1);
	
	USI_TOOL_GetSpecificString(para, key, EQUALBEFORE);
    if (*key == 0)
	{
		DEBUG_ON("get the key {%s} failed.",key);
		free(key);
		free(value);
		printf("command is execute failed of formate is invalid.\n");
		return;
	}
	
	USI_TOOL_GetSpecificString(para, value, EQUALAFTER);
	if (*value == 0)
	{
		DEBUG_ON("get the value {%s} failed.",value);
		free(key);
		free(value);
		printf("command is execute failed of formate is invalid.\n");
		return;
	}
	
	DEBUG_ON("get the key {%s} value {%s}", key, value);
	USI_DATE_delSpecficContect(key, value);
	free(key);
	free(value);
	USI_DATE_updatePositionForContect();
	printf("commond is running success!\n");
}
	
USI_VOID USI_GUI_DisplayVersion(UINT8* para)
{
	printf("Software Version:\n");
	/*2018.5.27 显示版本号信息时增加换行符*/
	printf("%s\n", SOFTVERSION);
	print_debug("Show Version is success.");
}

EXCUTE_COMMAND g_command[]={
	{"help", USI_GUI_ShowHelpInfo},
	{"create", USI_GUI_CreateContect},
	{"delete", USI_GUI_ClearALLContects},
	{"show", USI_GUI_ShowALLContects},
	{"clear", USI_GUI_ClearALLContects},
	{"export", USI_GUI_ExportALLContects},
	{"import", USI_GUI_ImportALLContects},
	{"version", USI_GUI_DisplayVersion},
	{"quit", USI_GUI_Exit}
};

EXCUTE_SUBCOMMAND g_subcommand[]={
	{"show", "name=", USI_GUI_ShowSpecficContects},
	{"delete", "name=", USI_GUI_DelSpecficContect}
};


USI_VOID USI_GUI_DispathCommand()
{
	UINT iRet = 0;
	UINT iLoop = 0;
	UINT8 Flag = 0;
	UINT8 tmpStr[10]={0};
	UINT8 tmpSubStr[20]={0};
	UINT8 *key = NULL;
	UINT8 *value = NULL;

	iRet = USI_TOOL_CheckAlphaForSercurity(g_buffer);
	if (iRet != 0)
	{
		DEBUG_ON("input para contains invalid alpha");
		return;
	}
	iRet = USI_TOOL_CheckFirstParaIsValid(g_buffer);
	if (iRet == 0)
	{
		USI_TOOL_GetSpecificVarious(g_buffer, MAINCOMPOS, tmpStr);
		for (;iRet < (sizeof(g_command) / sizeof(EXCUTE_COMMAND));iRet++)
		{
			if (strcmp(g_command[iRet].commandname, tmpStr) == 0)
			{
				USI_TOOL_GetSpecificVarious(g_buffer, SUBCOMPOS, tmpSubStr);
				for (;iLoop < (sizeof(g_subcommand) / sizeof(EXCUTE_SUBCOMMAND));iLoop++)
				{
					if (strcmp(tmpStr, g_subcommand[iLoop].commandname) == 0
					  && strstr(tmpSubStr, g_subcommand[iLoop].subcommandname) != NULL)
					{
						Flag = 1;
						print_debug("get the sub command {%s}", tmpSubStr);
						g_subcommand[iLoop].fp(tmpSubStr);
						break;
					}
				}
				if (*tmpSubStr == 0)
				{
					Flag = 1;
					print_debug("get the command {%s}", tmpStr);
					g_command[iRet].fp(NULL);
				}
				memset(tmpSubStr, 0, 20);
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
	//system("cls");
	USI_GUI_ShowWelInfo(g_displaywelcominfo);
	USI_TOOL_DealWithInputInfo(g_buffer);
	USI_GUI_DispathCommand();
	//getchar();
	//system("pause");
}
