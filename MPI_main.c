#include "MPI_main.h"

INT main()
{
    DEBUG_ON("start software");
    USI_LOG_CreateLog();
	USI_DATE_ModuleInit();
	while (1)
    {
		USI_GUI_Operating();
	}
	USI_DATE_ModuleExit();
	return 0;
}
