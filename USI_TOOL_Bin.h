#ifndef USI_GLABOL
#include "USI_GLABOL_Various.h"
#include "USI_LOG_Config.h"
#endif

#define True 1
#define False 0

#define EQUALBEFORE "ebefore"
#define EQUALAFTER    "eafter"

UINT8* USI_TOOL_DealWithInputInfo(UINT8*);
VOID USI_TOOL_GetSpecificVarious(UINT8*, UINT, UINT8*);
INT USI_TOOL_CheckFirstParaIsValid(UINT8*);
UINT8* USI_TOOL_DeleteSpecificSubstring(UINT8 *, UINT8 *);
VOID USI_TOOL_GetSpecificString(UINT8*, UINT8* , UINT8*);
INT USI_TOOL_CheckIsFuzzySearch(UINT8*);
INT USI_TOOL_CheckAlphaForSercurity(UINT8*);


