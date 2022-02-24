/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdragomi <vdragomi@students.42wolfsburg.de +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/24 18:13:38 by vdragomi          #+#    #+#             */
/*   Updated: 2022/02/24 18:13:38 by vdragomi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

void	ft_close_fd(void)
{
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
}

void	ft_free_split(char **split)
{
	int	i;

	i = 0;
	while (split[i] != NULL)
	{
		free(split[i]);
		i++;
	}
	free(split);
}

void	ft_exit_on_error2(char *error_msg)
{
	perror(error_msg);
	ft_close_fd();
	free_global();
	exit(EXIT_FAILURE);
}

void	ft_exit_on_invalid_cmd(char **path_list, t_list **cmd_list, \
		t_content *content, t_list *elem)
{
	ft_free_split(path_list);
	ft_free_list(*cmd_list);
	free(content);
	free(elem);
	perror("Invalid Command");
	ft_close_fd();
	exit(EXIT_FAILURE);
}
