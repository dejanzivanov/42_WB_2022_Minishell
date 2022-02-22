#include "../incl/minishell.h"

void ft_free_secure(void **s)
{
	if (*s != NULL)
		free(*s);
	*s = NULL;
}