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

CONTECT* g_RemoveListContects = NULL;
/*2018.06.17 �޸�ȫ��contect���ݽṹΪ˫����֮ǰΪ������*/
USI_LISTENTRY g_ListDoubleForContects;
/*2018.08.31 ������ϵ�˶����ɾ������*/
USI_LISTENTRY g_ListRemovingForContects;



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

	g_ListRemovingForContects.pstNext = &g_ListRemovingForContects;
	g_ListRemovingForContects.pstPrev = &g_ListRemovingForContects;
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


CONTECT* USI_DATE_getNewContect()
{
	CONTECT* createdNode = NULL;
	createdNode = USI_DATE_CreateNewContect();
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
�޸�:2018.08.31 �Ż��޸�֧������˫����
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

            /*����Ҫɾ���Ķ�������ɾ�������ȴ���ɾ��*/
			USI_TOOL_DeleteList(entry);
			USI_TOOL_InsertListToHead(&curPos->listEntry, &g_ListRemovingForContects);				
		}
	}
	
    /*�����ɾ������Ϊ�����ʾû���ҵ���ɾ������*/
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
	    /*�����Ҫ�Զ������ȡ��ɾ����������Ҫ����Ӵ�ɾ��������ɾ������������ӽ�����������*/
		USI_TOOL_EmulatingList_For_Each(entry, &g_ListRemovingForContects)
		{
			USI_TOOL_DeleteList(entry);
			curPos = USI_TOOL_GetInfo(entry, CONTECT, listEntry);
			USI_TOOL_InsertListToHead(&curPos->listEntry, &g_ListDoubleForContects);
		}

	    print_debug("delete Contect is cancle.");
		return;
	}

	/*���ҵ��Ķ������ɾ��*/
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
		strcpy_s(srcname, strlen(file->bufName) + 1, file->bufName);
	}
	else
	{
		printf("input name:");
		gets_s(srcname, NAME_LEN);
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
			strcpy_s(tmpcur->phoneNumber, strlen(file->bufNumber) + 1, file->bufNumber);
		}
		else
		{
			printf("input phone Number:");
			gets_s(tmpcur->phoneNumber, PHONE_LEN);
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
    /*2017.9.1Bug:�޸�������ļ�������ʱ��������*/
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



