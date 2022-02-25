/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdragomi <vdragomi@42student.wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/24 18:00:49 by vdragomi          #+#    #+#             */
/*   Updated: 2022/02/25 16:59:14 by vdragomi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

// void	ft_append_handler(int *fd_outfile, t_command *cmd)
// {
// 	if (*fd_outfile != 1)
// 		close(*fd_outfile);
// 	*fd_outfile = out_redirect(cmd->comm_table[1], OUT_APPEND);
// }

// void	ft_in_infile_handler(int **fd_docks, int *fd_infile)
// {
// 	if (*fd_infile != 0 && *fd_infile != *fd_docks[0])
// 		close(*fd_infile);
// 	*fd_infile = *fd_docks[0];
// }

// void	ft_outfile_handler(int *fd_outfile, t_command *cmd)
// {
// 	if (*fd_outfile != 1)
// 		close(*fd_outfile);
// 	*fd_outfile = out_redirect(cmd->comm_table[1], OUT_WRITE);
// }

// int	ft_file_itterator(t_command *cmd, \
// 	int *fd_outfile, int *fd_infile, int **fd_docks)
// {
// 	if (cmd->cmd_type == FT_CMD_TYPE_REDIRECT)
// 	{
// 		if (!(ft_strcmp(cmd->comm_table[0], ">")))
// 			ft_outfile_handler(fd_outfile, cmd);
// 		else if (!(ft_strcmp(cmd->comm_table[0], ">>")))
// 			ft_append_handler(fd_outfile, cmd);
// 		else if (ft_strcmp(cmd->comm_table[0], "<") == 0)
// 			ft_infile_handler(*fd_docks, fd_infile, cmd);
// 		else if (!(ft_strcmp(cmd->comm_table[0], "<<")) && fd_docks != NULL)
// 			ft_in_infile_handler(fd_docks, fd_infile);
// 	}
// 	else
// 		return (1);
// 	return (0);
// }

// void	ft_file_checker(t_list **cmd_list, t_pipex **pipex)
// {
// 	t_command	*cmd;
// 	int			i;
// 	int			fd_infile;
// 	int			fd_outfile;
// 	int			*fd_docks;

// 	i = (*pipex)->i;
// 	fd_infile = (*pipex)->fd_in[0];
// 	fd_outfile = (*pipex)->fd_out[1];
// 	fd_docks = (*pipex)->fd_docks[(*pipex)->i];
// 	cmd = NULL;
// 	while (*cmd_list != NULL)
// 	{
// 		cmd = (t_command *)(*cmd_list)->content;
// 		if (cmd->index != i)
// 			break ;
// 		if (ft_file_itterator(cmd, &fd_outfile, &fd_infile, &fd_docks) == 1)
// 			break ;
// 		*cmd_list = (*cmd_list)->next;
// 	}
// 	ft_re_attach_docs(&fd_docks, &fd_infile, &fd_outfile);
// }

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
		ft_set_lasts(NULL, 0, ft_atoi(ft_itoa(WEXITSTATUS(status))), 2);
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
