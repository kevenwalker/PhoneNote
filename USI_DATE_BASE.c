#include "USI_DATE_BASE.h"


#define HEADFLAG 0 
#define NOTHEAD 1

#define DISABLE 0
#define ENABLE 1

#define IDLEN 7 //ID�����ʾ���10�ֽ�
#define NAMELEN 13 //NAME�ֶ���ʾ���15�ֽ�
#define PNONELEN 21 //PHONE�ֶ���ʾ���20�ֽ�
#define DETAIL_LEN 50


#define insertList(element, list, feature) \
	while (list->feature) \
		list = list->feature; \
	list->feature = element;

CONTECT* g_ListContects = NULL;
CONTECT* g_RemoveListContects = NULL;
/*2016.06.17 �޸�ȫ��contect���ݽṹΪ˫����֮ǰΪ������*/
USI_LISTENTRY g_ListDoubleForContects;


UINT8 g_total = 1;
FILE* g_fp = NULL;

/*-------------------------------------------------------
������:USI_DATE_FormateDisplayInfoForHead
�������:��
�������:��
˵��:��ӡ��ʾcontect��Ϣ�ı���������ʽ���£�
  ID  |  NAME  |  PHONE  |
����:2018.8.18
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
	sprintf(tmpStr + offset, "%s", "ID");
	tmpStr[offset + 2] = ' ';
	tmpStr[IDLEN - 1] = '|';

	offset = (NAMELEN - 4) / 2;
	sprintf(tmpStr + IDLEN + offset, "%s", "NAME");
	tmpStr[IDLEN + offset + 4] = ' ';
	tmpStr[IDLEN + NAMELEN - 1] = '|';

	offset = (PNONELEN - 5) / 2;
	sprintf(tmpStr + IDLEN + NAMELEN + offset, "%s", "PHONE");
	tmpStr[IDLEN + NAMELEN + offset + 5] = ' ';
	tmpStr[IDLEN + NAMELEN + PNONELEN - 1] = '|';

    printf("%s\n", tmpStr);
    free(tmpStr);
    return;
}

/*-------------------------------------------------------
������:USI_DATE_FormateDisplayInfoForContect
�������:contectinfo----ָ����Ҫ��ʾ��contect���ݶ���
�������:��
˵��:�ض���contect�е�����������ʽ��ʾ������phone�ֶ�ֻ��ʾ��һ��
����:2018.8.18
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
		snprintf(tmpStr + offset, strlen(contectinfo->id), "%s", contectinfo->id);
		tmpStr[offset + strlen(contectinfo->id)] = ' ';
		tmpStr[IDLEN - 1] = '|';
    }

    if(strlen(contectinfo->name) < NAMELEN)
    {
    	offset = (NAMELEN - strlen(contectinfo->name)) / 2;
		snprintf(tmpStr + IDLEN + offset, strlen(contectinfo->name), "%s", contectinfo->name);
		tmpStr[offset + IDLEN + strlen(contectinfo->name)] = ' ';
		tmpStr[IDLEN + NAMELEN - 1] = '|';
    }

   
    if(strlen(contectinfo->telephone->phoneNumber) < NAMELEN)
    {
    	offset = (PNONELEN - strlen(contectinfo->telephone->phoneNumber)) / 2;
		snprintf(tmpStr + IDLEN + NAMELEN + offset, strlen(contectinfo->telephone->phoneNumber), "%s", contectinfo->telephone->phoneNumber);
		tmpStr[offset + IDLEN + NAMELEN + strlen(contectinfo->telephone->phoneNumber)] = ' ';
		tmpStr[IDLEN + NAMELEN + PNONELEN - 1] = '|';
    }

	printf("%s\n", tmpStr);
	free(tmpStr);
	return;
	
}

/*-------------------------------------------------------
������:USI_DATE_DisplayDetailInfoForContect
�������:contectinfo----ָ����Ҫ��ʾ��contect���ݶ���
�������:��
˵��:��ָ����contect���ݶ��������ϸ��Ϣ��ʾ
����:2018.8.18
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
	snprintf(tmpStr, DETAIL_LEN, "User NAME:%s", contectinfo->name);
	printf("%s\n", tmpStr);

	memset(tmpStr, 0, DETAIL_LEN + 1);
	snprintf(tmpStr, DETAIL_LEN, "User ID Number:%s", contectinfo->id);
	printf("%s\n", tmpStr);

    printf("User Phone Number:\n");
    iTmp = 0;
    stPhoneList = contectinfo->telephone;
    while(NULL != stPhoneList)
    {
		memset(tmpStr, 0, DETAIL_LEN + 1);
		snprintf(tmpStr, DETAIL_LEN, "[%d] %s", iTmp++, stPhoneList->phoneNumber);
		printf("%s\n", tmpStr);
		stPhoneList = stPhoneList->next;
	}
	
}

/*-------------------------------------------------------
������:USI_DATE_DisplayContect
�������:contectinfo----ָ����Ҫ��ʾ��contect���ݶ���
�������:��
˵��:��ָ����contect���������ʽ��Ϣ��ʾ
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
������:USI_DATE_ModuleInit
�������:��
�������:��
˵��:��ʼ��ȫ�����ݽṹ
-------------------------------------------------------*/
VOID USI_DATE_ModuleInit()
{
    DEBUG_ON("starting init USI_DATE_Module");
	g_ListDoubleForContects.pstNext = &g_ListDoubleForContects;
	g_ListDoubleForContects.pstPrev = &g_ListDoubleForContects;
}

/*-------------------------------------------------------
������:USI_DATE_ModuleExit
�������:��
�������:��
˵��:Ŀǰ�˺��������κ����飬ֻ��Ϊ����USI_DATE_ModuleInit���
-------------------------------------------------------*/
VOID USI_DATE_ModuleExit()
{
	/*2018.06.17 �������һЩ��Ҫ������Ŀǰ�����κ�����*/
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
	itoa(g_total, contect->id, 10);
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
    /*2018.6.17 ���ڲ�����contect�ṹ�ӵ�����ʵ����ʽ�޸�Ϊ˫����*/
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

/*-------------------------------------------------------
������:USI_DATE_cleanContectList
�������:��
�������:��
˵��:����ڲ�����contect����
�޸�:2018.06.18�޸�����˫������ʽ��contect�ڲ����ݣ�֮ǰΪ������
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
������:USI_DATE_delSpecficContect
�������:key----ָ�����ݵ��ֶΣ����������������
         value----ָ�����ݵ��ֶ����ݣ�����ȷ����ִ�еĶ���
�������:��
˵��:ʵ�ֶ�ָ������Ԫ�ؽ���ɾ������
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

				//USI_TOOL_InsertListToTail(&curPos->listEntry, &g_ListDoubleForContects);

				printf("Do you want to delete it really?[N] / Y\n");
				iRet = getchar();
				fflush(stdin);  
				if ( 'N' == iRet || 'n' == iRet)
				{
				    print_debug("delete Contect is cancle.");
					return;
				}

                DEBUG_ON("start delete the obj {%s}", curPos->name);
                
				/*��Ҫɾ�����������еĵ�һ��Ԫ��*/
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
������:USI_DATE_updatePostionForContect
�������:��
�������:��
˵��:ˢ��contect��position�ֶ�����
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

/*-------------------------------------------------------
������:USI_DATE_printSpecficContect
�������:key---ָ�����ڲ�������value---ָ�����ڲ�������ֵ
�������:��
˵��:��ʾָ�����ڲ�������Ϣ
�޸�:2018.06.18�޸�����˫������ʽ��contect�ڲ����ݣ�֮ǰΪ������
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
������:USI_DATE_printContectList
�������:��
�������:��
˵��:�����ڲ�����contect����Ԫ��
�޸�:2018.06.17�޸�����˫������ʽ��contect�ڲ����ݣ�֮ǰΪ������
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
������:USI_DATE_checkNameIsSame
�������:srcname---��Ҫ���ҵ�Ŀ�������
�������:��1��ʾ���ڲ����ݽṹ���ҵ�Ŀ����󣬷�֮û���ҵ���
dstcontect---�ҵ��ľ����Ŀ�����contect
˵��:����ָ����Ŀ��������������ڲ�contect���ݽṹ�н��в���
�޸�:2018.06.17�޸�����˫������ʽ��contect�ڲ����ݣ�֮ǰΪ������
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
������:USI_DATE_ReleaseContect
�������:pSrcData----ָ����Ҫ�ͷŵĲ�������
�������:���� 0 ��ʾ�����ɹ������� 1 ��ʾ����ʧ��
˵��:��Ҫʵ�ֶ�ָ��������ΪCONTECT�Ķ�������ڴ�ռ��ͷ�
�޸�:2018.06.18�޸�����˫������ʽ��contect�ڲ����ݣ�֮ǰΪ������
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
������:USI_DATE_modifyContect
�������:pstcontect----�´������д洢��ϵ�˵�contect����file---�����洢�ļ��е���ϢԴ
�������:��
˵��:ʵ�ֶ�������ϵ����Ϣ�Ĺ��˹��ܣ������ϵ�˴��ڲ���¼��ĵ绰��Ϣ��ͬ��������Ϣ¼��
�޸�:2018.06.18�޸�����˫������ʽ��contect�ڲ����ݣ�֮ǰΪ������
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
		strcpy(srcname, file->bufName);
	}
	else
	{
		printf("input name:");
		gets(srcname);
	}
	iRet = USI_DATE_checkNameIsSame(srcname, &objcontect);
	/*¼���������Ϣ����Ѿ�������iRetֵΪ1������Ϊ0*/
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
		/*��ϵ�����ݽṹ����������������鵽�Ѿ���������ϵ�ˣ���Ҫ���´����漰����Ŀռ��ͷŵ�*/
        USI_DATE_ReleaseContect(pstcontect);
        
		tmpcur->next = NULL;
		tmpori = objcontect->telephone;
		while(tmpori->next) 
		{
			/*�����ͬһ���˵�����绰�����Ѿ����ڣ���ʾ������ڲ������Դ˺���ļ�¼*/
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
		strcpy(pstcontect->name, srcname);
		if (iFlag == DISABLE)
		{
			strcpy(pstcontect->telephone->phoneNumber, file->bufNumber);
		}
		else
		{
			printf("input phone Number:");
			gets(pstcontect->telephone->phoneNumber);
		}
	}
}

/*-------------------------------------------------------
������:USI_DATE_exportContect
�������:��
�������:��
˵��:��Ҫʵ���ڲ�����contect������Ϣת����ָ�����ı��ļ�PaperPhone.txt��
�޸�:2018.06.18�޸�����˫������ʽ��contect�ڲ����ݣ�֮ǰΪ������
-------------------------------------------------------*/
VOID USI_DATE_exportContect()
{
	CONTECT* curPos = NULL;
	USI_LISTENTRY *entry = NULL;
	USI_LISTENTRY *pstTmp = NULL;
	PHONE_LIST *tmpcur = NULL;
	g_fp = fopen("./PaperPhone.txt","w");
	if (g_fp == NULL)
	{
		print_debug("get file handle point failed.\n");
		return;
	}

	USI_TOOL_EmulatingList_For_Each(entry, &g_ListDoubleForContects)
	{
		curPos = USI_TOOL_GetInfo(entry, CONTECT, listEntry);

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
	/*Bug:�޸�������ļ������ļ���ʽ�����⣬Ĭ�ϸ�ʽΪtxt*/
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
    /*2017.9.1Bug:�޸�������ļ�������ʱ��������*/
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



