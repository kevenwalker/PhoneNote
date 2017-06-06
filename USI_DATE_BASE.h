#ifndef USI_DATE_BASE
#define USI_DATE_BASE
#include "USI_GLABOL_Various.h"
#include "USI_LOG_Config.h"
#include "USI_TOOL_Bin.h"
#endif

#define NAME_LEN 33
#define PHONE_LEN 12
#define BUFFER_LEN 50

typedef struct TagPHONE_LIST{
	UINT8 phoneNumber[PHONE_LEN];
	struct TagPHONE_LIST *next;
}PHONE_LIST;

typedef struct contect{
	UINT8 name[NAME_LEN];
	UINT8 position;
	PHONE_LIST *telephone;
	UINT8 isValid;
	UINT8 isHead;
	struct contect* next;
}CONTECT;

CONTECT* USI_DATE_getNewContect();
VOID USI_DATE_printContectList();
VOID USI_DATE_modifyContect(CONTECT*);