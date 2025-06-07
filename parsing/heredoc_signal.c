#include "parsing.h"



int	here_doc_static(int type, int status)
{
	static int	exit_status = 0;

	if (type == SET)
		exit_status = status;
	return (exit_status);
}


int	signal_static(int type, int status)
{
	static int	exit_status = 0;

	if (type == SET)
		exit_status = status;
	return (exit_status);
}