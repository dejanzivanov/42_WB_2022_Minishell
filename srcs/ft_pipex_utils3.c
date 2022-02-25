/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pipex_utils3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzivanov <dzivanov@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/25 17:10:52 by dzivanov          #+#    #+#             */
/*   Updated: 2022/02/25 17:25:00 by dzivanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

void	ft_execute_child(t_list *cmd_list, char **envp, pid_t pid)
{
	t_command	*cmd;

	if (cmd_list != NULL)
		cmd = (t_command *)(cmd_list->content);
	else
		return ;
	if (cmd->cmd_type == FT_CMD_TYPE_SYSTEM)
	{
		if (execve(cmd->path, cmd->comm_table, envp) == -1)
			ft_exit_on_error2("Command execution failed");
	}
	if (cmd->cmd_type == FT_CMD_TYPE_BUILT_IN)
		ft_execve(cmd->comm_table, pid);
}

void	ft_free_pipex_and_reattach_pipes(t_pipex **pipex)
{
	int	x;
	int	status;

	x = 0;
	while (x <= (*pipex)->last_index)
	{
		waitpid((*pipex)->pidt[x], &status, 0);
		ft_set_lasts(NULL, 0, WEXITSTATUS(status), 2);
		x++;
	}
	dup2((*pipex)->fd_stream[0], STDIN_FILENO);
	dup2((*pipex)->fd_stream[1], STDOUT_FILENO);
	(*pipex)->i = 0;
	while ((*pipex)->i <= (*pipex)->last_index)
	{
		if ((*pipex)->fd_docks[(*pipex)->i] != NULL)
			free((*pipex)->fd_docks[(*pipex)->i]);
		(*pipex)->i++;
	}
	if ((*pipex)->fd_docks != NULL)
		free((*pipex)->fd_docks);
	if ((*pipex)->pidt != NULL)
		free((*pipex)->pidt);
	if ((*pipex) != NULL)
		free((*pipex));
}

void	ft_pipe_attachment(t_pipex **pipex)
{
	close((*pipex)->fd_out[0]);
	if ((*pipex)->i == 0)
		if (dup2((*pipex)->fd_stream[0], (*pipex)->fd_in[0]) == -1)
			ft_exit_on_error2("File descriptor duplication failed 90");
	if ((*pipex)->i == (*pipex)->last_index)
		if (dup2((*pipex)->fd_stream[1], (*pipex)->fd_out[1]) == -1)
			ft_exit_on_error2("File descriptor duplication failed 91");
	if (dup2((*pipex)->fd_in[0], STDIN_FILENO) == -1)
		ft_exit_on_error2("File descriptor duplication failed 92");
	if (dup2((*pipex)->fd_out[1], STDOUT_FILENO) == -1)
	{
		printf("\nErrno is %d\n", errno);
		ft_exit_on_error2("File descriptor duplication failedi 93");
		printf("Errno is %d", errno);
	}
}

void	ft_pipex_exit(t_pipex **pipex)
{
	(*pipex)->exit_value = ft_atoi(g_access.last_return);
	free_global();
	close(STDERR_FILENO);
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	exit((*pipex)->exit_value);
}

void	ft_here_doc_handler(t_pipex **pipex)
{
	pid_t	pid;

	if (!ft_strcmp((*pipex)->cmd->comm_table[0], "<<") \
	&& (*pipex)->cmd->cmd_type == FT_CMD_TYPE_REDIRECT)
	{
		if ((*pipex)->fd_docks[(*pipex)->cmd->index] != NULL)
		{
			close((*pipex)->fd_docks[(*pipex)->cmd->index][0]);
			free((*pipex)->fd_docks[(*pipex)->cmd->index]);
		}
		(*pipex)->fd_docks[(*pipex)->cmd->index] = \
			(int *)malloc(sizeof(int) * 2);
		if (pipe((*pipex)->fd_docks[(*pipex)->cmd->index]) == -1)
			ft_exit_on_error2("Pipe creation failed");
		pid = fork();
		if (!pid)
			heredoc_child((*pipex)->fd_docks[(*pipex)->cmd->index], \
			(*pipex)->fd_stream, (*pipex)->cmd->comm_table[1], "> ");
		else
			heredoc_parent((*pipex)->fd_docks[(*pipex)->cmd->index], pid);
	}
}
