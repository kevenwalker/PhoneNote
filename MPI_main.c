#include "MPI_main.h"

INT main()
{
    USI_LOG_CreateLog();
	DEBUG_ON("start software");
	while (1)
    {
		USI_GUI_Operating();
	}
	return 0;
}
