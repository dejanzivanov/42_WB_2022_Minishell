/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdragomi <vdragomi@students.42wolfsburg.de +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/24 17:47:26 by vdragomi          #+#    #+#             */
/*   Updated: 2022/02/24 17:47:26 by vdragomi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

t_global	g_access;
/*
**	line 32 is dealing with EOF (Ctrl + D)
**	line 33 is dealing with Enter (empty input)
*/

static int	ft_ctrld_handler(void)
{
	if (g_access.read_line2lexor == NULL)
	{
		write(2, "exit\n", 5);
		return (1);
	}
	return (0);
}

int	main(int argc, char **argv, char**envp)
{
	ft_signal_setup();
	if (argc > 1)
		printf("Invalid number of arguments for %s with %s\n", argv[0], envp[0]);
	ft_initiator(envp, argv[0]);
	while (1)
	{
		g_access.inter = 1;
		g_access.read_line2lexor = readline(CRED "minishe11 1.0> " RESET);
		g_access.inter = 0;
		if (ft_ctrld_handler() == 1)
			break ;
		if (*(g_access.read_line2lexor) == 0)
			continue ;
		add_history(g_access.read_line2lexor);
		if (lexor() != -1)
		{
			if (parser() == 2)
				continue ;
			if (executor(envp) == 0)
				break ;
		}
	}
	free_global();
	return (0);
}
