#include "USI_TOOL_Bin.h"

#define USI_TOOL_CheckPoint(point)\
	((point) == NULL ? exit(1) : 0)


#define USI_TOOL_CpyString(srcstart, srcend, dst)\
	while(srcstart<srcend) {\
		*dst = *srcstart;dst++;srcstart++;}


#define ROOTADMIN "admin"

/*-------------------------------------------------------
������:USI_TOOL_CheckAlphaForSercurity
�������:buffer----������Ҫ������ַ�������
�������:���� 0 ��ʾ�޷Ƿ��ַ������� 1 ��ʾ�зǷ��ַ�
˵��:��Ҫʵ�ֶ���������а����Ƿ��ַ����м��
��Ҫ�漰�ķǷ��ַ��� .*@#!$&~^\\
��������ʱ��:2017.11.10
-------------------------------------------------------*/
INT USI_TOOL_CheckAlphaForSercurity(UINT8* buffer)
{
	INT iRet = 0;
	INT iLenbuf = 0;
	INT iLenMatch = 0;
	INT iLoopa = 0;
	INT iLoopb = 0;
	INT iFlag = 0;
	UINT8 *MatchStr = ".*@#!$&~^\\";

	iLenbuf = strlen(buffer);
	iLenMatch = strlen(MatchStr);

	if (!iLenbuf)
	{
		return 1;
	}
	
	for (iLoopa = 0; iLoopa < iLenbuf; iLoopa++)
	{
		for (iLoopb = 0; iLoopb < iLenMatch; iLoopb++)
		{
			if (buffer[iLoopa] == MatchStr[iLoopb])
			{
				DEBUG_ON("get the buffer alpha {%c} for match string.", buffer[iLoopa]);
				return 1;
			}
		}
	}
	return 0;
}

INT USI_TOOL_CheckFirstParaIsValid(UINT8* buffer)
{
	INT iRet = 0;
	INT iconut = 0;
	UINT8 *tmpend = buffer;
	UINT8 *tmpstart = buffer;
	UINT8 *tmpcmp = NULL;
	UINT8 *tmpcmpbuffer = NULL;
	USI_TOOL_CheckPoint(buffer);
	
	while(*tmpend)
	{
		if (*tmpend == ' ')
		{
			break;
		}
		tmpend++;
	}

	iconut = tmpend - tmpstart;
	tmpcmp = (UINT8*)malloc(iconut + 1);
	tmpcmpbuffer = tmpcmp;
	if (tmpcmp == NULL)
	{
		print_debug("alloc space for cmp admin is failed");
	}
	memset(tmpcmpbuffer, 0 ,iconut + 1);
	USI_TOOL_CpyString(tmpstart, tmpend, tmpcmpbuffer);
	if (strcmp(tmpcmp, ROOTADMIN) != 0)
	{
		print_debug("can not match admin");
		free(tmpcmp);
		return 1;
	}
	free(tmpcmp);
	return 0;

}

VOID USI_TOOL_GetSpecificVarious(UINT8* buffer, UINT Pos, UINT8* dstbuf)
{
	INT tmpcount = 0;
	UINT8 *ptmpcur = NULL;
	UINT8 *ptmppre = NULL;
	UINT8 *ptmpdstbuf = NULL;
	USI_TOOL_CheckPoint(buffer);
	USI_TOOL_CheckPoint(dstbuf);

    ptmpdstbuf = dstbuf;

	ptmpcur = buffer;
	ptmppre = buffer;
	while(*ptmpcur)
	{
		if (*ptmpcur == ' ' || *(ptmpcur + 1) == 0)
		{
			tmpcount++;
			if (*(ptmpcur + 1) == 0)
			{
				ptmpcur++;
			}
			if (tmpcount == Pos)
			{
                while(ptmppre != ptmpcur)
                {
					*ptmpdstbuf = *ptmppre;
					ptmppre++;
					ptmpdstbuf++;
                }
				*ptmpdstbuf = 0;
				return;
			}
			else
			{
			/*2017.09.09bug:�˺���ʵ�ַ�ʽ�ϴ������⣬�޸���ȡָ��������ʵ��Դ���ݲ�һ������(ʵ��С��Ӧ��ȡ�Ĳ�������)*/
				if (*ptmpcur != 0)
				{
					ptmppre = ptmpcur + 1;
				}
				else
				{
					break;
				}
			}
		}
		ptmpcur++;
	}
	print_debug("can not find para in the command by input the pos.");
	*dstbuf = 0;
}

USI_VOID USI_TOOL_PrunSpace(UINT8* buffer)
{
	INT Flag = 1;
	UINT8* tmpbuffer = NULL;
	UINT8 tmpbuffersize = 0;
	UINT8 currentbuffer = 0;
	UINT8 currenttmpbuffer = 0;
	USI_TOOL_CheckPoint(buffer);

	tmpbuffersize = strlen(buffer);
	tmpbuffer = (UINT8*)malloc(tmpbuffersize + 1);
	memset(tmpbuffer, 0, tmpbuffersize + 1);
	for (; buffer[currentbuffer]; currentbuffer++)
	{
		if (buffer[currentbuffer] == ' ' && Flag == 0)
		{
			tmpbuffer[currenttmpbuffer++] = buffer[currentbuffer];
			Flag = 1;
		}
		if (buffer[currentbuffer] != ' ')
		{
			tmpbuffer[currenttmpbuffer++] = buffer[currentbuffer];
			Flag = 0;
		}
	}
	if (tmpbuffer[currenttmpbuffer - 1] == ' ')
	{
		tmpbuffer[currenttmpbuffer - 1] = 0;
	}
	strcpy(buffer, tmpbuffer);
	free(tmpbuffer);
	tmpbuffer = NULL;
}
 
USI_VOID USI_TOOL_GetInputSource(UINT8* buffer)
{
	USI_TOOL_CheckPoint(buffer);
	printf("<command>");
	if (NULL == gets(buffer)) 
	{                         
		print_debug("get input info is failed\n");
		exit(1);
	}
}

USI_VOID USI_TOOL_LowerAlphat(UINT8 *buffer)
{
	UINT8 iRet = 0;
	USI_TOOL_CheckPoint(buffer);
	for (; iRet < strlen(buffer); iRet++)
	{
		if (buffer[iRet] >= 'A' && buffer[iRet] <= 'Z')
		{
			buffer[iRet] += 32;
		}
	}
}

UINT8* USI_TOOL_DealWithInputInfo(UINT8 *buffer)
{
	USI_TOOL_CheckPoint(buffer);
	USI_TOOL_GetInputSource(buffer);
	USI_TOOL_LowerAlphat(buffer);
	USI_TOOL_PrunSpace(buffer);
	return buffer;
}

UINT8* USI_TOOL_DeleteSpecificSubstring(UINT8 *parentStr, UINT8 *subStr)
{
	INT8 *cRet = NULL;
	if (parentStr == NULL || subStr == NULL)
	{
		print_debug("get the para parentStr or subStr is NULL");
		return NULL;
	}	
	cRet = strstr(parentStr, subStr);
	if (cRet == NULL)
	{
		print_debug("can't find the substring in parent string");
		return NULL;
	}
	/*�Ӵ�����ĸ���е���ʼλ��ֻ����ĸ�����׵�ַ*/
	if (cRet != parentStr)
	{
		print_debug("the subtring position is err");
		return NULL;
	}
	*(parentStr + strlen(parentStr) - 1) = 0;
	return (parentStr + strlen(subStr) + 1);
}


VOID USI_TOOL_GetSpecificString(UINT8 *Str, UINT8* dstStr, UINT8 *opera)
{
	UINT8* curpos = Str;
	
	if (Str == NULL || dstStr == NULL || strlen(Str) == 0)
	{
		print_debug("input the Str{%s} StrLen{%d} dstStr{%p} opera{%s} is error", Str, strlen(Str), dstStr, opera);
		*dstStr = 0;
		return;
	}
	if (strstr(Str, "=") == NULL)
	{
		print_debug("input the para {%s} can not find the =", Str);
		*dstStr = 0;
		return;
	}
	if (strcmp(opera, EQUALBEFORE) == 0)
	{
	/*2017.09.08bug:para�����ڽ��뵽�������գ�ԭ�����ں�����!=�жϷ���д��=!�����ƻ�����Σ��ڶ��ε��ñ��*/
		while(*curpos != '=')
		{
			*dstStr = *curpos;
			curpos++;
			dstStr++;
		}
		*dstStr = 0;
	}
    else
    {
    	if (strcmp(opera, EQUALAFTER) == 0)
		{
			while(*curpos != '=')
			{
				curpos++;
			}
			curpos++;
			if (*curpos == 0)
			{
				print_debug("can not get after = the para");
				*dstStr = 0;
				return;
			}
			while(*curpos)
			{
				*dstStr = *curpos;
				curpos++;
				dstStr++;
			}
			*dstStr = 0;
		}
		else
		{
			print_debug("input the opera {%s} is invalid", opera);
			*dstStr = 0;
			return;
		}
    }
}

/*-------------------------------------------------------
������:USI_TOOL_CheckIsFuzzySearch
�������:Str----������Ҫ������ַ�������
�������:���� Ture ��ʾģ������ģʽ������ False ��ʾ��ģ������ģʽ
˵��:��Ҫʵ�ֶ���������а���ͨ���*����ʶ���봦��
��������ʱ��:2017.09.09
-------------------------------------------------------*/
INT USI_TOOL_CheckIsFuzzySearch(UINT8 *Str)
{
	if (Str == NULL || strlen(Str) == 0)
	{
		print_debug("[USI_TOOL_CheckIsFuzzySearch]input para is error {%s}.", Str);
		return False;
	}
	while(*Str)
	{
		if(*Str == '*')
		{
			*Str = 0;
			return True;
		}
		Str++;
	}
	return False;
}

