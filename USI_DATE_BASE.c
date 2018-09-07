#include "USI_DATE_BASE.h"


#define HEADFLAG 0 
#define NOTHEAD 1

#define DISABLE 0
#define ENABLE 1

#define IDLEN 7 //ID序号显示宽度10字节
#define NAMELEN 13 //NAME字段显示宽度15字节
#define PNONELEN 21 //PHONE字段显示宽度20字节
#define DETAIL_LEN 50


#define insertList(element, list, feature) \
	while (list->feature) \
		list = list->feature; \
	list->feature = element;

CONTECT* g_RemoveListContects = NULL;
/*2018.06.17 修改全局contect数据结构为双链表，之前为单链表*/
USI_LISTENTRY g_ListDoubleForContects;
/*2018.08.31 增加联系人对象待删除链表*/
USI_LISTENTRY g_ListRemovingForContects;



UINT8 g_total = 1;
FILE* g_fp = NULL;

/*-------------------------------------------------------
函数名:USI_DATE_FormateDisplayInfoForHead
输入参数:无
输出参数:无
说明:打印显示contect信息的标题栏，形式如下：
  ID  |  NAME  |  PHONE  |
新增:2018.8.18
-------------------------------------------------------*/

USI_VOID USI_DATE_FormateDisplayInfoForHead()
{
	INT8 *tmpStr = NULL;
    INT  offset  = 0;
	
	tmpStr = (INT8*)malloc(IDLEN + NAMELEN + PNONELEN + 1);
    if(NULL == tmpStr)
    {
    	DEBUG_ON("alloc space for display info head failed.");
    	return;
    }
    memset(tmpStr, ' ', IDLEN + NAMELEN + PNONELEN);
    tmpStr[IDLEN + NAMELEN + PNONELEN] = '\0';
	
    offset = (IDLEN - 2) / 2;
	snprintf(tmpStr + offset, strlen("ID") + 1, strlen("ID"), "%s", "ID");
	tmpStr[offset + 2] = ' ';
	tmpStr[IDLEN - 1] = '|';

	offset = (NAMELEN - 4) / 2;
	snprintf(tmpStr + IDLEN + offset, strlen("NAME") + 1, strlen("NAME"), "%s", "NAME");
	tmpStr[IDLEN + offset + 4] = ' ';
	tmpStr[IDLEN + NAMELEN - 1] = '|';

	offset = (PNONELEN - 5) / 2;
	snprintf(tmpStr + IDLEN + NAMELEN + offset, strlen("PHONE") + 1, strlen("PHONE"), "%s", "PHONE");
	tmpStr[IDLEN + NAMELEN + offset + 5] = ' ';
	tmpStr[IDLEN + NAMELEN + PNONELEN - 1] = '|';

    printf("%s\n", tmpStr);
    free(tmpStr);
    return;
}

/*-------------------------------------------------------
函数名:USI_DATE_FormateDisplayInfoForContect
输入参数:contectinfo----指定需要显示的contect数据对象
输出参数:无
说明:重定向contect中的内容以行形式显示，其中phone字段只显示第一个
新增:2018.8.18
-------------------------------------------------------*/

USI_VOID USI_DATE_FormateDisplayInfoForContect(CONTECT *contectinfo)
{
	PHONE_LIST *tmpcur = NULL;
	INT8 *tmpStr = NULL;
	INT  offset  = 0;

	tmpStr = (INT8*)malloc(IDLEN + NAMELEN + PNONELEN + 1);
	if(NULL == tmpStr)
	{
		DEBUG_ON("alloc space for display info contect failed.");
		return;
	}
	memset(tmpStr, ' ', IDLEN + NAMELEN + PNONELEN);
	tmpStr[IDLEN + NAMELEN + PNONELEN] = '\0';

    if(strlen(contectinfo->id) < IDLEN)
    {
    	offset = (IDLEN - strlen(contectinfo->id)) / 2;
		snprintf(tmpStr + offset, strlen(contectinfo->id) + 1, strlen(contectinfo->id), "%s", contectinfo->id);
		tmpStr[offset + strlen(contectinfo->id)] = ' ';
		tmpStr[IDLEN - 1] = '|';
    }

    if(strlen(contectinfo->name) < NAMELEN)
    {
    	offset = (NAMELEN - strlen(contectinfo->name)) / 2;
		snprintf(tmpStr + IDLEN + offset, strlen(contectinfo->name) + 1 , strlen(contectinfo->name), "%s", contectinfo->name);
		tmpStr[offset + IDLEN + strlen(contectinfo->name)] = ' ';
		tmpStr[IDLEN + NAMELEN - 1] = '|';
    }

   
    if(strlen(contectinfo->telephone->phoneNumber) < NAMELEN)
    {
    	offset = (PNONELEN - strlen(contectinfo->telephone->phoneNumber)) / 2;
		snprintf(tmpStr + IDLEN + NAMELEN + offset, strlen(contectinfo->telephone->phoneNumber) + 1, strlen(contectinfo->telephone->phoneNumber), "%s", contectinfo->telephone->phoneNumber);
		tmpStr[offset + IDLEN + NAMELEN + strlen(contectinfo->telephone->phoneNumber)] = ' ';
		tmpStr[IDLEN + NAMELEN + PNONELEN - 1] = '|';
    }

	printf("%s\n", tmpStr);
	free(tmpStr);
	return;
	
}

/*-------------------------------------------------------
函数名:USI_DATE_DisplayDetailInfoForContect
输入参数:contectinfo----指定需要显示的contect数据对象
输出参数:无
说明:对指定的contect数据对象进行详细信息显示
新增:2018.8.18
-------------------------------------------------------*/

USI_VOID USI_DATE_DisplayDetailInfoForContect(CONTECT *contectinfo)
{
	INT8 *tmpStr = NULL;
	PHONE_LIST *stPhoneList = NULL;
	INT  iTmp = 0;

	tmpStr = (INT8*)malloc(DETAIL_LEN + 1);
	if(NULL == tmpStr)
	{
		DEBUG_ON("alloc space for display detail info contect failed.");
		return;
	}

	memset(tmpStr, 0, DETAIL_LEN + 1);
	snprintf(tmpStr, DETAIL_LEN, DETAIL_LEN, "User NAME:%s", contectinfo->name);
	printf("%s\n", tmpStr);

	memset(tmpStr, 0, DETAIL_LEN + 1);
	snprintf(tmpStr, DETAIL_LEN, DETAIL_LEN, "User ID Number:%s", contectinfo->id);
	printf("%s\n", tmpStr);

    printf("User Phone Number:\n");
    iTmp = 0;
    stPhoneList = contectinfo->telephone;
    while(NULL != stPhoneList)
    {
		memset(tmpStr, 0, DETAIL_LEN + 1);
		snprintf(tmpStr, DETAIL_LEN, DETAIL_LEN, "[%d] %s", iTmp++, stPhoneList->phoneNumber);
		printf("%s\n", tmpStr);
		stPhoneList = stPhoneList->next;
	}
	
}

/*-------------------------------------------------------
函数名:USI_DATE_DisplayContect
输入参数:contectinfo----指定需要显示的contect数据对象
输出参数:无
说明:对指定的contect对象进行显式信息显示
-------------------------------------------------------*/
USI_VOID USI_DATE_DisplayContect(CONTECT *contectinfo, INT enableDetail)
{
	PHONE_LIST *tmpcur = NULL;
	INT8 *tmpStr = NULL;
	INT  offset  = 0;

	if(True == enableDetail)
	{
    	USI_DATE_FormateDisplayInfoForContect(contectinfo);
    }
    else
    {
    	USI_DATE_DisplayDetailInfoForContect(contectinfo);
    }

}


/*-------------------------------------------------------
函数名:USI_DATE_ModuleInit
输入参数:无
输出参数:无
说明:初始化全局数据结构
-------------------------------------------------------*/
VOID USI_DATE_ModuleInit()
{
    DEBUG_ON("starting init USI_DATE_Module");
	g_ListDoubleForContects.pstNext = &g_ListDoubleForContects;
	g_ListDoubleForContects.pstPrev = &g_ListDoubleForContects;

	g_ListRemovingForContects.pstNext = &g_ListRemovingForContects;
	g_ListRemovingForContects.pstPrev = &g_ListRemovingForContects;
}

/*-------------------------------------------------------
函数名:USI_DATE_ModuleExit
输入参数:无
输出参数:无
说明:目前此函数不做任何事情，只是为了与USI_DATE_ModuleInit配对
-------------------------------------------------------*/
VOID USI_DATE_ModuleExit()
{
	/*2018.06.17 后续添加一些必要操作，目前不做任何事情*/
	DEBUG_ON("destory USI_DATE_Module success");
	return;
}


USI_VOID USI_DATE_initContect(CONTECT* contect)
{
	if (contect == NULL)
	{
		print_debug("init contect is failed");
		return;
	}
	_itoa_s(g_total++, contect->id, ID_LEN, 10);
	contect->telephone = (PHONE_LIST*)malloc(sizeof(PHONE_LIST));
	if (contect->telephone == NULL)
	{
		print_debug("alloc phone space failed.");
		exit(1);
	}
	contect->telephone->next = NULL;
	memset(contect->name, 0, sizeof(contect->name));
	memset(contect->telephone->phoneNumber, 0, PHONE_LEN);
	strcpy_s(contect->name, strlen("<NULL>") + 1, "<NULL>");
	strcpy_s(contect->telephone->phoneNumber, strlen("<NULL>") + 1, "<NULL>");
    /*2018.6.17 将内部数据contect结构从单链表实现形式修改为双链表*/
	USI_TOOL_InsertListToHead(&contect->listEntry, &g_ListDoubleForContects);
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


CONTECT* USI_DATE_getNewContect()
{
	CONTECT* createdNode = NULL;
	createdNode = USI_DATE_CreateNewContect();
	return createdNode;
}

/*-------------------------------------------------------
函数名:USI_DATE_cleanContectList
输入参数:无
输出参数:无
说明:清除内部所有contect数据
修改:2018.06.18修改适配双链表形式的contect内部数据，之前为单链表
-------------------------------------------------------*/
VOID USI_DATE_cleanContectList()
{
	CONTECT* curPos = NULL;
	PHONE_LIST* tmpcur = NULL;
	PHONE_LIST* tmp = NULL;
	USI_LISTENTRY *entry = NULL;
	USI_LISTENTRY *pstTmp = NULL;
	char iRet;

	USI_TOOL_ListIsEmpty(&g_ListDoubleForContects)
    {
        printf("Contect is empty!\n");
    	DEBUG_ON("Clear Contect is failed becauce of empty.");
    }
    

	printf("Do you want to clear all date really?[N] / Y\n");
	iRet = getchar();
	fflush(stdin);
	if ( 'N' == iRet || 'n' == iRet)
	{
	    print_debug("Clear Contect is cancle.");
		return;
	}

    USI_TOOL_EmulatingList_For_Each(entry, &g_ListDoubleForContects)
	{
		USI_TOOL_DeleteList(entry);
		curPos = USI_TOOL_GetInfo(entry, CONTECT, listEntry);        
		tmpcur = curPos->telephone;
		
		while(tmpcur != NULL)
		{
			tmp = tmpcur;
			tmpcur = tmpcur->next;
			free(tmp);
		}
		free(curPos);
	}
	
	printf("Clear all contects success!\n");
	print_debug("Clear Contect is success.");

}

/*-------------------------------------------------------
函数名:USI_DATE_delSpecficContect
输入参数:key----指定依据的字段，即搜索对象的类型
         value----指定依据的字段内容，即明确具体执行的对象
输出参数:无
说明:实现对指定对象元素进行删除操作
修改:2018.08.31 优化修改支持适配双链表
-------------------------------------------------------*/
VOID USI_DATE_delSpecficContect(UINT8 *key, UINT8 *value)
{
	CONTECT* curPos = NULL;
	PHONE_LIST *tmpcur = NULL;
	PHONE_LIST *tmp = NULL;
	UINT uiFindFlag = 0;
	INT iFuzzy = 0;
	INT iRet = 0;
	USI_LISTENTRY *entry = NULL;
	USI_LISTENTRY *pstTmp = NULL;
	
	iFuzzy = USI_TOOL_CheckIsFuzzySearch(value);

	USI_TOOL_EmulatingList_For_Each(entry, &g_ListDoubleForContects)
	{
		
		curPos = USI_TOOL_GetInfo(entry, CONTECT, listEntry);
		if (strcmp(key, SUBCOMNAME) == 0)
		{
			if (iFuzzy == True)
			{
				if (strstr(curPos->name, value) == NULL)
				{
					continue;
				}
			}
			else
			{
				if (strcmp(value, curPos->name) != 0)
				{
					continue;
				}
			}

            /*将需要删除的对象放入待删除链表，等待被删除*/
			USI_TOOL_DeleteList(entry);
			USI_TOOL_InsertListToHead(&curPos->listEntry, &g_ListRemovingForContects);				
		}
	}
	
    /*如果待删除链表为空则表示没有找到待删除对象*/
    USI_TOOL_ListIsEmpty(&g_ListRemovingForContects)
	{
		printf("can not find any specfic contect\n");
		return;
	}

	USI_TOOL_EmulatingList_For_Each(entry, &g_ListRemovingForContects)
	{
		curPos = USI_TOOL_GetInfo(entry, CONTECT, listEntry);
		USI_DATE_DisplayContect(curPos, False);
	}
	
	printf("Do you want to delete it really?[N] / Y\n");
	iRet = getchar();
	fflush(stdin);  
	if ( 'N' == iRet || 'n' == iRet)
	{
	    /*如果需要对对象进行取消删除操作，需要将其从待删除链表中删除，并重新添加进对象链表中*/
		USI_TOOL_EmulatingList_For_Each(entry, &g_ListRemovingForContects)
		{
			USI_TOOL_DeleteList(entry);
			curPos = USI_TOOL_GetInfo(entry, CONTECT, listEntry);
			USI_TOOL_InsertListToHead(&curPos->listEntry, &g_ListDoubleForContects);
		}

	    print_debug("delete Contect is cancle.");
		return;
	}

	/*对找到的对象进行删除*/
	USI_TOOL_EmulatingList_For_Each(entry, &g_ListRemovingForContects)
	{
		USI_TOOL_DeleteList(entry);
		curPos = USI_TOOL_GetInfo(entry, CONTECT, listEntry);

		tmpcur = curPos->telephone;
		while(tmpcur)
		{
			tmp = tmpcur;
		    tmpcur = tmpcur->next;
			free(tmp);
		}
		free(curPos);
	}

}

/*-------------------------------------------------------
函数名:USI_DATE_printSpecficContect
输入参数:key---指定的内部数据域；value---指定的内部数据域值
输出参数:无
说明:显示指定的内部数据信息
修改:2018.06.18修改适配双链表形式的contect内部数据，之前为单链表
-------------------------------------------------------*/
VOID USI_DATE_printSpecficContect(UINT8 *key, UINT8 *value)
{
	CONTECT* curPos = NULL;
	UINT uiFindFlag = 0;
	INT iRet = False;
	USI_LISTENTRY *entry = NULL;
	USI_LISTENTRY *pstTmp = NULL;

	USI_TOOL_ListIsEmpty(&g_ListDoubleForContects)
    {
        printf("Contect is empty!\n");
    	DEBUG_ON("Show Contect is failed because of empty!");
    }

    iRet = USI_TOOL_CheckIsFuzzySearch(value);
    
	USI_TOOL_EmulatingList_For_Each(entry, &g_ListDoubleForContects)
	{
		curPos = USI_TOOL_GetInfo(entry, CONTECT, listEntry);

		if (strcmp(key, SUBCOMNAME) == 0)
		{
			if (iRet == True)
			{
				if (strstr(curPos->name, value) == NULL)
				{
					continue;
				}
			}
			else
			{
				if (strcmp(value, curPos->name) != 0)
				{
					continue;
				}
			}
			
			USI_DATE_DisplayContect(curPos, iRet);
			uiFindFlag = 1;
			print_debug("find the specfic contect.");
		}
	}
    
	
	if (!uiFindFlag)
	{
		print_debug("can not find any specfic contect.");
		printf("can not find any specfic contect\n");
	}
}

/*-------------------------------------------------------
函数名:USI_DATE_printContectList
输入参数:无
输出参数:无
说明:显现内部所有contect数据元素
修改:2018.06.17修改适配双链表形式的contect内部数据，之前为单链表
-------------------------------------------------------*/
VOID USI_DATE_printContectList()
{
	CONTECT* curPos = NULL;
    USI_LISTENTRY *entry = NULL;
    USI_LISTENTRY *pstTmp = NULL;

    USI_TOOL_ListIsEmpty(&g_ListDoubleForContects)
    {
        printf("Contect is empty!\n");
    	DEBUG_ON("Show Contect is failed because of empty!");
    	return;
    }
    
    USI_DATE_FormateDisplayInfoForHead(); 
	USI_TOOL_EmulatingList_For_Each(entry, &g_ListDoubleForContects)
	{
		curPos = USI_TOOL_GetInfo(entry, CONTECT, listEntry);

		USI_DATE_DisplayContect(curPos, True);
	}
	
}

/*-------------------------------------------------------
函数名:USI_DATE_checkNameIsSame
输入参数:srcname---需要查找的目标对象域；
输出参数:返1表示在内部数据结构中找到目标对象，反之没有找到；
dstcontect---找到的具体的目标对象contect
说明:根据指定的目标对象数据域，在内部contect数据结构中进行查找
修改:2018.06.17修改适配双链表形式的contect内部数据，之前为单链表
-------------------------------------------------------*/
USI_INT USI_DATE_checkNameIsSame(UINT8 *srcname, CONTECT **dstcontect)
{
	CONTECT* curPos = NULL;
    USI_LISTENTRY *entry = NULL;
    USI_LISTENTRY *pstTmp = NULL;

	USI_TOOL_EmulatingList_For_Each(entry, &g_ListDoubleForContects)
	{
		curPos = USI_TOOL_GetInfo(entry, CONTECT, listEntry);

		if(strcmp(curPos->name, srcname) == 0)
		{
			*dstcontect = curPos;
			DEBUG_ON("Find the exist internal contect {0x%p}",curPos);
			return 1;
		}
	}
	return 0;
}

/*-------------------------------------------------------
函数名:USI_DATE_ReleaseContect
输入参数:pSrcData----指定需要释放的操作对象
输出参数:返回 0 表示操作成功；返回 1 表示操作失败
说明:主要实现对指定的类型为CONTECT的对象进行内存空间释放
修改:2018.06.18修改适配双链表形式的contect内部数据，之前为单链表
-------------------------------------------------------*/
USI_VOID USI_DATE_ReleaseContect(CONTECT* pSrcData)
{
	
	CONTECT* curPos = NULL;
	USI_LISTENTRY *entry = NULL;
	USI_LISTENTRY *pstTmp = NULL;
	PHONE_LIST *curPhone = NULL;
	

	USI_TOOL_EmulatingList_For_Each(entry, &g_ListDoubleForContects)
	{
		curPos = USI_TOOL_GetInfo(entry, CONTECT, listEntry);

		if(curPos == pSrcData)
		{
			DEBUG_ON("Release the new internal contect {0x%p}",curPos);
			USI_TOOL_DeleteList(entry);
			free(curPos->telephone);
			free(curPos);
			return;
		}
	}

}

/*-------------------------------------------------------
函数名:USI_DATE_modifyContect
输入参数:pstcontect----新创建进行存储联系人的contect对象；file---来至存储文件中的信息源
输出参数:无
说明:实现对输入联系人信息的过滤功能，如果联系人存在并且录入的电话信息相同将跳过信息录入
修改:2018.06.18修改适配双链表形式的contect内部数据，之前为单链表
-------------------------------------------------------*/
VOID USI_DATE_modifyContect(CONTECT* pstcontect, FILE_INFO *file)
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
		strcpy_s(srcname, strlen(file->bufName) + 1, file->bufName);
	}
	else
	{
		printf("input name:");
		gets_s(srcname, NAME_LEN);
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
			strcpy_s(tmpcur->phoneNumber, strlen(file->bufNumber) + 1, file->bufNumber);
		}
		else
		{
			printf("input phone Number:");
			gets_s(tmpcur->phoneNumber, PHONE_LEN);
		}


		/*联系人数据结构会无条件创建，检查到已经创建了联系人，需要将新创建涉及分配的空间释放掉*/
        USI_DATE_ReleaseContect(pstcontect);
        
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
		strcpy_s(pstcontect->name, strlen(srcname) + 1, srcname);
		if (iFlag == DISABLE)
		{
			strcpy_s(pstcontect->telephone->phoneNumber, strlen(file->bufNumber) + 1, file->bufNumber);
		}
		else
		{
			printf("input phone Number:");
			gets_s(pstcontect->telephone->phoneNumber, PHONE_LEN);
		}
	}
}

/*-------------------------------------------------------
函数名:USI_DATE_exportContect
输入参数:无
输出参数:无
说明:主要实现内部数据contect链表信息转存入指定的文本文件PaperPhone.txt中
修改:2018.06.18修改适配双链表形式的contect内部数据，之前为单链表
-------------------------------------------------------*/
VOID USI_DATE_exportContect()
{
	CONTECT* curPos = NULL;
	USI_LISTENTRY *entry = NULL;
	USI_LISTENTRY *pstTmp = NULL;
	PHONE_LIST *tmpcur = NULL;
	fopen_s(&g_fp, "./PaperPhone.txt","w");
	if (g_fp == NULL)
	{
		print_debug("get file handle point failed.\n");
		return;
	}

	USI_TOOL_EmulatingList_For_Each(entry, &g_ListDoubleForContects)
	{
		curPos = USI_TOOL_GetInfo(entry, CONTECT, listEntry);

		tmpcur = curPos->telephone;
	    fprintf(g_fp, "%s%s\n", FORMAT_INDEX, curPos->id);
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
		snprintf(tmpFilename, strlen(ucFilename) + strlen(".txt") + 1, strlen(ucFilename) + strlen(".txt"), "%s.txt", ucFilename);
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
		snprintf(tmpFilename, strlen(ucFilename) + 1, strlen(ucFilename), "%s", ucFilename);
	}
    /*2017.9.1Bug:修复导入的文件不存在时报错问题*/
	fopen_s(&g_fp, tmpFilename,"r");
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
		cRet = fgets(tmpbuffer, BUFFER_LEN - 1, g_fp);
		if (!cRet)
		{
			print_debug("read info is finished");
			break;
		}
		if (strstr(tmpbuffer, FORMAT_PHONENAME))
		{
			buffpos = USI_TOOL_DeleteSpecificSubstring(tmpbuffer, FORMAT_PHONENAME);
			strcpy_s(stInfo.bufName, strlen(buffpos) + 1, buffpos);
			cRet = fgets(tmpbuffer, BUFFER_LEN + 1, g_fp);
			while (tmpbuffer == strstr(tmpbuffer, FORMAT_PHONENUM))
			{
				if (strstr(tmpbuffer, FORMAT_PHONENUM))
				{
					buffpos = USI_TOOL_DeleteSpecificSubstring(tmpbuffer, FORMAT_PHONENUM);
					strcpy_s(stInfo.bufNumber, strlen(buffpos) + 1, buffpos);
					USI_DATE_CreateNewContectByFile(stInfo);
				}
				cRet = fgets(tmpbuffer, BUFFER_LEN + 1, g_fp);
				if(cRet == NULL)
				{
					break;
				}
			}
		}
	}while(cRet != NULL);
	fclose(g_fp);
	g_fp = NULL;
	return SUCCESS;
}



