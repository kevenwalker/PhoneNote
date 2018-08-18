#ifndef USI_GLABOL
#include "USI_GLABOL_Various.h"
#include "USI_LOG_Config.h"
#endif

#define True 1
#define False 0

#define EQUALBEFORE "ebefore"
#define EQUALAFTER    "eafter"

#define _InsertList(cur, prev, ori) \
	(cur)->pstNext = (ori);\
	(cur)->pstPrev = (prev);\
	(prev)->pstNext = (cur);\
	(ori)->pstPrev = (cur);

#define _DeleteList(cur, prev, next) \
    (prev)->pstNext = (next);\
    (next)->pstPrev = (prev);\
    (cur)->pstNext = (cur);\
    (cur)->pstPrev = (cur);

#define USI_TOOL_InsertListToHead(element, head) do{ \
	_InsertList(element, ((head)->pstPrev), head);\
}while(0)

#define USI_TOOL_DeleteList(element) do {\
	_DeleteList(element, ((element)->pstPrev), ((element)->pstNext));\
}while(0)

#define USI_TOOL_ListIsEmpty(head) \
	if(((head)->pstPrev == (head) ) || ((head)->pstNext == (head))) 
	
#define USI_TOOL_GetInfo(entry, type, member) \
		(type*)((unsigned long)(entry) - (unsigned long)(&(((type*)0)->member))) 

#define USI_TOOL_EmulatingList_For_Each(pos, head) \
	for(pos = (head)->pstNext, pstTmp = pos->pstNext; pos != (head); pos = pstTmp, pstTmp = pstTmp->pstNext)


UINT8* USI_TOOL_DealWithInputInfo(UINT8*);
VOID USI_TOOL_GetSpecificVarious(UINT8*, UINT, UINT8*);
INT USI_TOOL_CheckFirstParaIsValid(UINT8*);
UINT8* USI_TOOL_DeleteSpecificSubstring(UINT8 *, UINT8 *);
VOID USI_TOOL_GetSpecificString(UINT8*, UINT8* , UINT8*);
INT USI_TOOL_CheckIsFuzzySearch(UINT8*);
INT USI_TOOL_CheckAlphaForSercurity(UINT8*);


