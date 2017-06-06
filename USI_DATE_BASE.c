#include "USI_DATE_BASE.h"

#define HEADFLAG 0 
#define NOTHEAD 1

#define insertList(element, list, feature) \
	while (list->feature) \
		list = list->feature; \
	list->feature = element;

CONTECT* g_ListContects = NULL;
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
������:USI_DATE_ReleaseContect
�������:pSrcData----ָ����Ҫ�ͷŵĲ�������
�������:���� 0 ��ʾ�����ɹ������� 1 ��ʾ����ʧ��
˵��:��Ҫʵ�ֶ�ָ��������ΪCONTECT�Ķ�������ڴ�ռ��ͷ�
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
	
/*���һ:����Ҫ�ͷŵĶ���������ı�ͷλ��*/
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
		/*�����:����Ҫ�ͷŵĶ���������ı���λ��*/
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
			/*�����:����Ҫ�ͷŵĶ���������ı�βλ��*/
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


VOID USI_DATE_modifyContect(CONTECT* para)
{
	USI_INT iRet = 0;
	UINT8 srcname[NAME_LEN];
	PHONE_LIST *tmpcur = NULL;
	PHONE_LIST *tmpori = NULL;
	CONTECT *objcontect = NULL;
	printf("input name:");
	gets(srcname);
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
        USI_DATE_ReleaseContect(para);
		
		tmpcur->next = NULL;
		tmpori = objcontect->telephone;
		while(tmpori->next) 
		{
			tmpori = tmpori->next;
		}
		tmpori->next = tmpcur;
		printf("input phone Number:");
		gets(tmpcur->phoneNumber);
		return;
	}
	else
	{
		strcpy(para->name, srcname);
		printf("input phone Number:");
		gets(para->telephone->phoneNumber);
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
	    fprintf(g_fp, "NO.%d\n", curPos->position);
		fprintf(g_fp, "Person Name: %s\n", curPos->name);
		while(tmpcur)
		{
			fprintf(g_fp, "Phone Number: %s\n", tmpcur->phoneNumber);
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

VOID USI_DATE_importContect(UINT8 *ucFilename)
{
	UINT8 *tmpFilename = NULL;
	UINT8 tmpbuffer[BUFFER_LEN];
	
	if (ucFilename == NULL || strlen(ucFilename) == 0)
	{
		print_debug("import file name may err");
		return;
	}
	tmpFilename = (UINT8*)malloc(strlen(ucFilename) + 1);
	if (NULL == tmpFilename)
	{
		print_debug("alloc tmpFilename space is err");
		return;
	}
	memset(tmpFilename, 0, strlen(ucFilename) + 1);
	sprintf(tmpFilename, "%s", ucFilename);
	g_fp = fopen(tmpFilename,"r");
	free(tmpFilename);

    memset(tmpbuffer, 0, BUFFER_LEN);
	fscanf(g_fp, "%s", tmpbuffer);
	/*ֻ������˴��ļ��ж�ȡ���ݣ���δ�Զ�ȡ�������ݽ��д�������һ������ʵ��*/
}



