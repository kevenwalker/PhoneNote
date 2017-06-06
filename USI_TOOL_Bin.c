#include "USI_TOOL_Bin.h"

#define True 1
#define False 0

#define USI_TOOL_CheckPoint(point)\
	((point) == NULL ? exit(1) : 0)

#define USI_TOOL_CpyString(srcstart, srcend, dst)\
	while(srcstart<srcend) {\
		*dst = *srcstart;dst++;srcstart++;}
		

#define ROOTADMIN "admin"

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
				ptmppre = ptmpcur + 1;
			}
		}
		ptmpcur++;
	}
	print_debug("can not find para in the command by input the pos\n");
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


