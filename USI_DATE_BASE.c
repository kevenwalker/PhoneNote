#include "USI_DATE_BASE.h"

#define HEADFLAG 0 
#define NOTHEAD 1

#define DISABLE 0
#define ENABLE 1

#define insertList(element, list, feature) \
	while (list->feature) \
		list = list->feature; \
	list->feature = element;

CONTECT* g_ListContects = NULL;
CONTECT* g_RemoveListContects = NULL;

UINT8 g_total = 1;
FILE* g_fp = NULL;

USI_VOID USI_DATE_initContect(CONTECT* contect)
{
	if (contect == NULL)
	{
		print_debug("init contect is failed");
		return;
	}
	contect->position = g_total++;
	contect->telephone = (PHONE_LIST*)malloc(sizeof(PHONE_LIST));
	if (contect->telephone == NULL)
	{
		print_debug("alloc phone space failed.");
		exit(1);
	}
	contect->telephone->next = NULL;
	memset(contect->name, 0, sizeof(contect->name));
	memset(contect->telephone->phoneNumber, 0, PHONE_LEN);
	strcpy(contect->name, "<NULL>");
	strcpy(contect->telephone->phoneNumber, "<NULL>");
	contect->next = NULL;
	return;
}

CONTECT* USI_DATE_CreateNewContect()
{
	CONTECT* RetNode = NULL;
	RetNode = (CONTECT*)malloc(sizeof(CONTECT));
	if (NULL == RetNode)
	{
		printf("create single node is failed.\n");
		exit(1);
	}
	USI_DATE_initContect(RetNode);
	return RetNode;
}


USI_VOID USI_DATE_addNewContectInList(CONTECT* contect)
{
	CONTECT* curPos = NULL;

	if (g_ListContects == NULL)
	{
		g_ListContects = contect;
		contect->isHead = HEADFLAG;
		contect->next = NULL;
	}
	else
	{
		curPos = g_ListContects;
		insertList(contect, curPos, next);
		contect->isHead = NOTHEAD;
	}
}

CONTECT* USI_DATE_getNewContect()
{
	CONTECT* createdNode = NULL;
	createdNode = USI_DATE_CreateNewContect();
	USI_DATE_addNewContectInList(createdNode);
	return createdNode;
}

VOID USI_DATE_cleanContectList()
{
	CONTECT* curPos = NULL;
	CONTECT* tmpcurPos = NULL;
	char iRet;
	if (g_ListContects == NULL)
	{
	    printf("Contect is empty!\n");
		print_debug("Clear Contect is failed becauce of empty.");
	}
	else
	{
		printf("Do you want to clear all date really?[N] / Y\n");
		iRet = getchar();
		fflush(stdin);
		if ( 'N' == iRet || 'n' == iRet)
		{
		    print_debug("Clear Contect is cancle.");
			return;
		}
	    curPos = g_ListContects;
		while(curPos)
		{
			tmpcurPos = curPos;
			curPos = curPos->next;
			free(tmpcurPos->telephone);
			free(tmpcurPos);
			tmpcurPos = NULL;
		}
		g_ListContects = NULL;
		g_total = 1;
		printf("Clear all contects success!\n");
		print_debug("Clear Contect is success.");
		getchar();
	}
}

/*-------------------------------------------------------
函数名:USI_DATE_delSpecficContect
输入参数:key----指定依据的字段，即搜索对象的类型
         value----指定依据的字段内容，即明确具体执行的对象
输出参数:无
说明:实现对指定对象元素进行删除操作
-------------------------------------------------------*/
VOID USI_DATE_delSpecficContect(UINT8 *key, UINT8 *value)
{
	CONTECT* curPos = NULL;
	CONTECT* prePos = NULL;
	PHONE_LIST *tmpcur = NULL;
	PHONE_LIST *tmp = NULL;
	UINT uiFindFlag = 0;
	INT iRet = 0;
	
	if (g_ListContects == NULL)
	{
	    printf("Contect is empty!\n");
		DEBUG_ON("Delete Contect is success!");
	}
	else
	{
		curPos = g_ListContects;
		iRet = USI_TOOL_CheckIsFuzzySearch(value);
		while(curPos)
		{
			if (strcmp(key, SUBCOMNAME) == 0)
			{
				if (iRet == True)
				{
					if (strstr(curPos->name, value) == NULL)
					{
					    prePos = curPos;
						curPos = curPos->next;
						continue;
					}
				}
				else
				{
					if (strcmp(value, curPos->name) != 0)
					{
					    prePos = curPos;
						curPos = curPos->next;
						continue;
					}
				}

				printf("NO.%d\n", curPos->position);
				printf("Person Name: %s\n", curPos->name);
				tmpcur = curPos->telephone;
				while(tmpcur)
				{
					printf("Phone Number: %s\n", tmpcur->phoneNumber);
					tmpcur = tmpcur->next;
				}
				printf("\n");
				uiFindFlag = 1;
				print_debug("find the specfic contect.");

				printf("Do you want to delete it really?[N] / Y\n");
				iRet = getchar();
				fflush(stdin);  
				if ( 'N' == iRet || 'n' == iRet)
				{
				    print_debug("delete Contect is cancle.");
					return;
				}

                DEBUG_ON("start delete the obj {%s}", curPos->name);
                
				/*需要删除的是链表中的第一个元素*/
				if (prePos == NULL)
				{
					tmpcur = curPos->telephone;
					while(tmpcur)
					{
					    tmp = tmpcur;
					    tmpcur = tmpcur->next;
						free(tmp);
					}
					g_ListContects = curPos->next;
					if (g_ListContects != NULL)
					{
						g_ListContects->isHead = HEADFLAG;
					}
					free(curPos);
					DEBUG_ON("finish delete first obj from context list");
					return;
				}
				else
				{
					tmpcur = curPos->telephone;
					while(tmpcur)
					{
						tmp = tmpcur;
					    tmpcur = tmpcur->next;
						free(tmp);
					}
					prePos->next = curPos->next;
					free(curPos);
					return;
				}
			}
			prePos = curPos;
			curPos = curPos->next;
		}
	}
	if (!uiFindFlag)
	{
		print_debug("can not find any specfic contect.");
		printf("can not find any specfic contect\n");
	}	
}

/*-------------------------------------------------------
函数名:USI_DATE_updatePostionForContect
输入参数:无
输出参数:无
说明:刷新contect中position字段排序
-------------------------------------------------------*/
VOID USI_DATE_updatePositionForContect()
{
	CONTECT* curPos = NULL;
	
	curPos = g_ListContects;
	if (curPos == NULL)
	{
		DEBUG_ON("unnecessary to update postion for contect");
		return;
	}

	DEBUG_ON("positions {%d} of all contect will be update to 1", g_total);
	g_total = 1;

	while(curPos)
	{
		curPos->position = g_total++;
		curPos = curPos->next;
	}
	DEBUG_ON("positions has been updated to %d", g_total);
	return;
}


VOID USI_DATE_printSpecficContect(UINT8 *key, UINT8 *value)
{
	CONTECT* curPos = NULL;
	PHONE_LIST *tmpcur = NULL;
	UINT uiFindFlag = 0;
	INT iRet = 0;
	
	if (g_ListContects == NULL)
	{
	    printf("Contect is empty!\n");
		print_debug("Show Contect is failed because of empty!");
	}
	else
	{
		curPos = g_ListContects;
		iRet = USI_TOOL_CheckIsFuzzySearch(value);
		while(curPos)
		{
			if (strcmp(key, SUBCOMNAME) == 0)
			{
				if (iRet == True)
				{
					if (strstr(curPos->name, value) == NULL)
					{
						curPos = curPos->next;
						continue;
					}
				}
				else
				{
					if (strcmp(value, curPos->name) != 0)
					{
						curPos = curPos->next;
						continue;
					}
				}
				printf("NO.%d\n", curPos->position);
				printf("Person Name: %s\n", curPos->name);
				tmpcur = curPos->telephone;
				while(tmpcur)
				{
					printf("Phone Number: %s\n", tmpcur->phoneNumber);
					tmpcur = tmpcur->next;
				}
				printf("\n");
				uiFindFlag = 1;
				print_debug("find the specfic contect.");
			}
			curPos = curPos->next;
		}
	}
	if (!uiFindFlag)
	{
		print_debug("can not find any specfic contect.");
		printf("can not find any specfic contect\n");
	}
}


VOID USI_DATE_printContectList()
{
	CONTECT* curPos = NULL;
	PHONE_LIST *tmpcur = NULL;
	if (g_ListContects == NULL)
	{
	    printf("Contect is empty!\n");
		print_debug("Show Contect is failed because of empty!");
	}
	else
	{
		curPos = g_ListContects;
		while(curPos)
		{
		    tmpcur = curPos->telephone;
			printf("NO.%d\n", curPos->position);
			printf("Person Name: %s\n", curPos->name);
			while(tmpcur)
			{
				printf("Phone Number: %s\n", tmpcur->phoneNumber);
				tmpcur = tmpcur->next;
			}
			printf("\n");
			curPos = curPos->next;
		}
	}
}

USI_INT USI_DATE_checkNameIsSame(UINT8 *srcname, CONTECT **dstcontect)
{
	CONTECT* curPos = g_ListContects;
	while(curPos)
	{
		if(strcmp(curPos->name, srcname) == 0)
		{
			*dstcontect = curPos;
			return 1;
		}
		curPos = curPos->next;
	}
	return 0;
}

/*-------------------------------------------------------
函数名:USI_DATE_ReleaseContect
输入参数:pSrcData----指定需要释放的操作对象
输出参数:返回 0 表示操作成功；返回 1 表示操作失败
说明:主要实现对指定的类型为CONTECT的对象进行内存空间释放
-------------------------------------------------------*/
USI_INT USI_DATE_ReleaseContect(CONTECT* pSrcData)
{
	CONTECT* curPos = g_ListContects;
	CONTECT* prePos = NULL;
	CONTECT* tmpPos = NULL;
	if ((curPos == NULL) || (pSrcData == NULL))
	{
		print_debug("release contect is failed.");
		return 1;
	}
	
/*情况一:当需要释放的对象在链表的表头位置*/
	if (curPos == pSrcData)
	{
		if (curPos->next == NULL)
		{
			free(curPos->telephone);
			free(curPos);
			curPos = NULL;
			g_ListContects = NULL;
			g_total = 1;
			return 0;
		}	
	}

    while(curPos)
    {
		/*情况二:当需要释放的对象在链表的表中位置*/
		if (curPos == pSrcData)
		{
			if (curPos->next != NULL)
			{
				prePos->next = curPos->next;
				g_total = curPos->position;
				tmpPos = curPos->next;
				while(tmpPos)
				{
					tmpPos->position = g_total++;
					tmpPos = tmpPos->next;
				}
				free(curPos->telephone);
				free(curPos);
				break;
			}
			/*情况三:当需要释放的对象在链表的表尾位置*/
			else
			{
				prePos->next = NULL;
				g_total--;
				free(curPos->telephone);
				free(curPos);
				break;
			}
		}
		prePos = curPos;
		curPos = curPos->next;
    } 
	return 0;
}


VOID USI_DATE_modifyContect(CONTECT* para, FILE_INFO *file)
{
	USI_INT iRet = 0;
	UINT8 srcname[NAME_LEN];
	PHONE_LIST *tmpcur = NULL;
	PHONE_LIST *tmpori = NULL;
	CONTECT *objcontect = NULL;
    INT iFlag = ENABLE;
	
    if (file != NULL)
    {
		iFlag = DISABLE;
    }

	if (iFlag == DISABLE)
	{
		strcpy(srcname, file->bufName);
	}
	else
	{
		printf("input name:");
		gets(srcname);
	}
	iRet = USI_DATE_checkNameIsSame(srcname, &objcontect);
	/*录入的人名信息如果已经存在则iRet值为1，否则为0*/
	if(iRet)
	{
		tmpcur = (PHONE_LIST*)malloc(sizeof(PHONE_LIST));
		if(tmpcur == NULL)
		{
			print_debug("alloc phone space is failed.");
			return;
		}
		
		if (iFlag == DISABLE)
		{
			strcpy(tmpcur->phoneNumber, file->bufNumber);
		}
		else
		{
			printf("input phone Number:");
			gets(tmpcur->phoneNumber);
		}
		
        USI_DATE_ReleaseContect(para);
		tmpcur->next = NULL;
		tmpori = objcontect->telephone;
		while(tmpori->next) 
		{
			/*检测相同一个人的输入电话号码已经存在，提示号码存在并跳过对此号码的记录*/
			if (strcmp(tmpori->phoneNumber, tmpcur->phoneNumber) == 0)
			{
				printf("Input the Phone Number is exist and skip it.\n");
				free(tmpcur);
				tmpcur = NULL;
				return;
			}
			tmpori = tmpori->next;
		}
		if (tmpori->next == NULL)
		{
			if (strcmp(tmpori->phoneNumber, tmpcur->phoneNumber) == 0)
			{
				printf("Input the Phone Number is exist and skip it.\n");
				free(tmpcur);
				tmpcur = NULL;
				return;
			}
		}
		tmpori->next = tmpcur;

		return;
	}
	else
	{
		strcpy(para->name, srcname);
		if (iFlag == DISABLE)
		{
			strcpy(para->telephone->phoneNumber, file->bufNumber);
		}
		else
		{
			printf("input phone Number:");
			gets(para->telephone->phoneNumber);
		}
	}
}

VOID USI_DATE_exportContect()
{
	CONTECT* curPos = g_ListContects;
	PHONE_LIST *tmpcur = NULL;
	g_fp = fopen("./PaperPhone.txt","w");
	if (g_fp == NULL)
	{
		print_debug("get file handle point failed.\n");
		return;
	}
	while(curPos)
	{
	    tmpcur = curPos->telephone;
	    fprintf(g_fp, "%s%d\n", FORMAT_INDEX, curPos->position);
		fprintf(g_fp, "%s %s\n", FORMAT_PHONENAME, curPos->name);
		while(tmpcur)
		{
			fprintf(g_fp, "%s %s\n", FORMAT_PHONENUM, tmpcur->phoneNumber);
			tmpcur = tmpcur->next;
		}
		fprintf(g_fp, "\n");
		curPos = curPos->next;
	}
	printf("Contect export is success!\n");
	fclose(g_fp);
	g_fp = NULL;
	return;
}

USI_VOID USI_DATE_CreateNewContectByFile(FILE_INFO obj)
{
    CONTECT* newContect = NULL;
	newContect = USI_DATE_getNewContect();
	USI_DATE_modifyContect(newContect, &obj);
}

INT USI_DATE_importContect(UINT8 *ucFilename)
{
	UINT8 *tmpFilename = NULL;
	UINT8 tmpbuffer[BUFFER_LEN];
	UINT8 *buffpos = NULL;
	INT8 *cRet = NULL;
    FILE_INFO stInfo;
	
	if (ucFilename == NULL || strlen(ucFilename) == 0)
	{
		print_debug("import file name may err");
		return FAIL;
	}
	print_debug("target Phone Paper is {%s}", ucFilename);
	/*Bug:修复导入的文件不带文件格式的问题，默认格式为txt*/
	if (strstr(ucFilename, ".txt") == NULL)
	{
		tmpFilename = (UINT8*)malloc(strlen(ucFilename) + strlen(".txt") + 1);
		if (NULL == tmpFilename)
		{
			print_debug("alloc tmpFilename without formate space is err");
			return FAIL;
		}
		memset(tmpFilename, 0, strlen(ucFilename) + strlen(".txt") + 1);
		sprintf(tmpFilename, "%s.txt", ucFilename);
	}
	else
	{
		tmpFilename = (UINT8*)malloc(strlen(ucFilename) + 1);
		if (NULL == tmpFilename)
		{
			print_debug("alloc tmpFilename space is err");
			return FAIL;
		}
		memset(tmpFilename, 0, strlen(ucFilename) + 1);
		sprintf(tmpFilename, "%s", ucFilename);
	}
    /*2017.9.1Bug:修复导入的文件不存在时报错问题*/
	g_fp = fopen(tmpFilename,"r");
	if (g_fp == NULL)
	{
		print_debug("open Phone Paper {%s} is failed", tmpFilename);
		free(tmpFilename);
		return FAIL;
	}
	free(tmpFilename);

	do{
		memset(&stInfo, 0, sizeof(FILE_INFO));
    	memset(tmpbuffer, 0, BUFFER_LEN);
		cRet = fgets(tmpbuffer, BUFFER_LEN + 1, g_fp);
		if (!cRet)
		{
			print_debug("read info is finished");
			break;
		}
		if (strstr(tmpbuffer, FORMAT_PHONENAME))
		{
			buffpos = USI_TOOL_DeleteSpecificSubstring(tmpbuffer, FORMAT_PHONENAME);
			strcpy(stInfo.bufName, buffpos);
			cRet = fgets(tmpbuffer, BUFFER_LEN + 1, g_fp);
			while (tmpbuffer != strstr(tmpbuffer, "\n"))
			{
				if (strstr(tmpbuffer, FORMAT_PHONENUM))
				{
					buffpos = USI_TOOL_DeleteSpecificSubstring(tmpbuffer, FORMAT_PHONENUM);
					strcpy(stInfo.bufNumber, buffpos);
					USI_DATE_CreateNewContectByFile(stInfo);
				}
				cRet = fgets(tmpbuffer, BUFFER_LEN + 1, g_fp);
			}
		}
	}while(cRet != NULL);
	fclose(g_fp);
	g_fp = NULL;
	return SUCCESS;
}



