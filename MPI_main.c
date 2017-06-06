#include "MPI_main.h"

INT main()
{
    USI_LOG_CreateLog();
	print_debug("start software");
	while (1)
    {
		USI_GUI_Operating();
	}
	return 0;
}
