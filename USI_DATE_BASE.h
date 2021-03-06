#ifndef USI_DATE_BASE
#define USI_DATE_BASE
#include "USI_GLABOL_Various.h"
#include "USI_LOG_Config.h"
#include "USI_TOOL_Bin.h"


#define NAME_LEN 33
#define ID_LEN 6
#define PHONE_LEN 12
#define BUFFER_LEN 50

#define SUBCOMNAME "name"
#define SUBCOMID   "id" //待实现

#define FORMAT_INDEX      "NO."
#define FORMAT_PHONENUM   "Phone Number:"
#define FORMAT_PHONENAME  "Person Name:"

typedef struct TagList_Entry USI_LISTENTRY;

struct TagList_Entry{
	USI_LISTENTRY *pstPrev;
	USI_LISTENTRY *pstNext;
};

typedef struct TagPHONE_LIST{
	UINT8 phoneNumber[PHONE_LEN];
	struct TagPHONE_LIST *next;
}PHONE_LIST;

typedef struct contect{
	UINT8 name[NAME_LEN];
	INT8  id[ID_LEN];
	UINT8 position;
	PHONE_LIST *telephone;
	UINT8 isValid;
	UINT8 isHead;
	struct contect* next;
	/*2018.06.16 将contect组织到双向链表中，此为链表入口*/
    USI_LISTENTRY listEntry; 
}CONTECT;

typedef struct TagFILE_INFO{
	UINT8 bufName[NAME_LEN];
	UINT8 bufNumber[PHONE_LEN];
}FILE_INFO;

VOID USI_DATE_ModuleInit();
VOID USI_DATE_ModuleExit();
CONTECT* USI_DATE_getNewContect();
VOID USI_DATE_printSpecficContect(UINT8*, UINT8*);
VOID USI_DATE_delSpecficContect(UINT8*, UINT8*);
VOID USI_DATE_updatePositionForContect();
VOID USI_DATE_printContectList();
VOID USI_DATE_modifyContect(CONTECT*, FILE_INFO*);
VOID USI_DATE_cleanContectList();
VOID USI_DATE_exportContect();
INT USI_DATE_importContect(UINT8 *);

#endif

