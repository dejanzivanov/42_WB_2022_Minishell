/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdragomi <vdragomi@42student.wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/24 18:00:49 by vdragomi          #+#    #+#             */
/*   Updated: 2022/02/25 18:14:46 by vdragomi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

void	ft_execute_bash_command(t_list **cmd_list, t_pipex **pipex)
{
	if ((*pipex)->i == 0)
		close((*pipex)->fd_in[1]);
	while (*cmd_list != NULL)
	{
		(*pipex)->cmd = (t_command *)(*cmd_list)->content;
		if ((*pipex)->cmd->cmd_type != FT_CMD_TYPE_REDIRECT)
			break ;
		(*cmd_list) = (*cmd_list)->next;
	}
	close((*pipex)->fd_in[0]);
	close((*pipex)->fd_out[1]);
	if ((*pipex)->i == (*pipex)->last_index)
		close((*pipex)->fd_out[0]);
	if ((*pipex)->cmd->cmd_type == FT_CMD_TYPE_BUILT_IN)
		ft_execve((*pipex)->cmd->comm_table, (*pipex)->pidt[(*pipex)->i]);
}

void	ft_setup_pipex(t_list **cmd_list, t_pipex **pipex)
{
	*pipex = ft_calloc(sizeof(t_pipex), 1);
	(*pipex)->cmd_list_temp = NULL;
	(*pipex)->fd_stream[0] = 0;
	(*pipex)->fd_stream[1] = 0;
	ft_initialize_fds((*pipex)->fd_stream);
	(*pipex)->last_index = \
	((t_command *)ft_lstlast((*cmd_list))->content)->index;
	(*pipex)->cmd_list_temp = (*cmd_list);
	(*pipex)->fd_docks = ft_calloc((*pipex)->last_index + 1, sizeof(int *));
	(*pipex)->i = 0;
}

void	ft_child_handler(t_list **cmd_list, t_pipex **pipex)
{
	ft_pipe_attachment(pipex);
	ft_file_checker(cmd_list, pipex);
	if ((*pipex)->i == 0)
		close((*pipex)->fd_in[1]);
	(*pipex)->i = 0;
	while ((*pipex)->i <= (*pipex)->last_index)
	{
		if ((*pipex)->fd_docks[(*pipex)->i] != NULL)
			free((*pipex)->fd_docks[(*pipex)->i]);
		(*pipex)->i++;
	}
	if ((*pipex)->fd_docks != NULL)
		free((*pipex)->fd_docks);
}

void	ft_pipex_itterator(t_list **cmd_list, t_pipex **pipex, char **envp)
{
	int	temp;

	while ((*cmd_list) != NULL)
	{
		(*pipex)->fd_in[0] = (*pipex)->fd_out[0];
		if (pipe((*pipex)->fd_out) == -1)
			ft_exit_on_error2("Pipe creation failed in line 124");
		(*pipex)->pidt[(*pipex)->i] = fork();
		if ((*pipex)->pidt[(*pipex)->i] == -1)
			ft_exit_on_error2("Pipe creation failed in line 127");
		else if ((*pipex)->pidt[(*pipex)->i] == 0)
		{
			ft_child_handler(cmd_list, pipex);
			temp = (*pipex)->pidt[(*pipex)->i];
			if ((*pipex)->pidt != NULL)
				free((*pipex)->pidt);
			ft_execute_child(*cmd_list, envp, temp);
			ft_pipex_exit(pipex);
		}
		else
			ft_execute_bash_command(cmd_list, pipex);
		(*pipex)->i++;
		if ((*cmd_list))
			(*cmd_list) = (*cmd_list)->next;
	}
}

int	pipex(t_list *cmd_list, char **envp)
{
	t_pipex	*pipex;

	ft_setup_pipex(&cmd_list, &pipex);
	while (pipex->cmd_list_temp != NULL)
	{
		pipex->cmd = (t_command *)pipex->cmd_list_temp->content;
		ft_here_doc_handler(&pipex);
		pipex->cmd_list_temp = pipex->cmd_list_temp->next;
	}
	if (pipe(pipex->fd_out) == -1)
		ft_exit_on_error2("Pipe creation failed in line 116");
	pipex->fd_in[1] = pipex->fd_out[1];
	pipex->pidt = ft_calloc(pipex->last_index + 1, sizeof(int *));
	ft_pipex_itterator(&cmd_list, &pipex, envp);
	ft_free_pipex_and_reattach_pipes(&pipex);
	return (0);
}
