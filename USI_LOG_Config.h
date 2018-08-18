#ifndef USI_LOG
#define USI_LOG
#include "USI_GLABOL_Various.h"
#include <stdarg.h>

/*2017.11.10:��־��ӡ��Ϣ�����Ӻ�����������Ϣ*/
#define DEBUG_ON(fmt,...) \
	USI_LOG_TRANS(fmt,__FUNCTION__,__LINE__,##__VA_ARGS__);
#define print_debug(fmt,...) \
	USI_LOG_TRANS(fmt,__FUNCTION__,__LINE__,##__VA_ARGS__);

VOID USI_LOG_CreateLog();
#endif