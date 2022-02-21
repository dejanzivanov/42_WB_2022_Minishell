/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_global_utilities.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdragomi <vdragomi@42student.wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/21 11:59:20 by vdragomi          #+#    #+#             */
/*   Updated: 2022/02/21 16:00:39 by vdragomi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"


void	ft_last_arg(char **args, pid_t pid)
{
	int	i;

	i = 0;
	if (!*args)
		ft_update_create_env("_", "", pid);
	while (args[i] != NULL)
		i++;
	ft_update_create_env("_", args[i - 1], pid);
}

/*
**	if mode = 1 => we change only the $_
**	if mode = 2 => we change only the $?
**	if mode = 3 => we change both the $? and $_
*/

void	ft_set_lasts(char **args, int pid, int lreturn, int mode)
{
	if (!(mode == 1 || mode == 2 || mode == 3))
		write(2, "ERROR ON SET LAST\n", 18);
	if (mode == 1)
		ft_last_arg(args, pid);
	else if (mode == 2)
	{
		if (g_access.last_return != NULL)
			free(g_access.last_return);
		g_access.last_return = ft_itoa(lreturn);
	}
	else if (mode == 3)
	{
		if (g_access.last_return != NULL)
			free(g_access.last_return);
		g_access.last_return = ft_itoa(lreturn);
		ft_last_arg(args, pid);
	}
}

void ft_free_secure(void **s)
{
      if (*s != NULL)
        free(*s);
      *s = NULL;
}