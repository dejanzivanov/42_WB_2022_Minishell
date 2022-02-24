/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_executor.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdragomi <vdragomi@students.42wolfsburg.de +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/24 18:16:08 by vdragomi          #+#    #+#             */
/*   Updated: 2022/02/24 18:16:08 by vdragomi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

int	executor(char **envp)
{
	pipex(g_access.parser2exec, envp);
	ft_lstclear(&g_access.parser2exec, ft_free_parser);
	return (1);
}
