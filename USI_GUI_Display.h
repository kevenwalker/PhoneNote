#ifndef USI_TOOL
#define USI_TOOL
#include "USI_TOOL_Bin.h"
#include "USI_DATE_BASE.h"
#include "USI_LOG_Config.h"
#endif

#ifndef USI_GLABOL
#define USI_GLABOL
#include "USI_GLABOL_Various.h"
#endif

#define MAINCOMPOS 2 /*命令除去admin后的第一个主命令参数*/
#define SUBCOMPOS   3 /*命令除去admin后的第二个主命令参数*/

VOID USI_GUI_Operating();
