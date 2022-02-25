/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdragomi <vdragomi@42student.wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/24 18:00:49 by vdragomi          #+#    #+#             */
/*   Updated: 2022/02/25 13:38:43 by vdragomi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"
 
void	ft_initialize_fds(int *fd_temp)
{
	dup2(STDIN_FILENO, fd_temp[0]);
	dup2(STDOUT_FILENO, fd_temp[1]);
}
 
int out_redirect(char* filename, int type)
{
	int fd;
	int premissions;
 
	if (type == OUT_WRITE)
		premissions = O_WRONLY | O_CREAT | O_TRUNC;
	else if (type == OUT_APPEND)
		premissions =  O_WRONLY | O_APPEND | O_CREAT ;
	else
	{
		ft_exit_on_error2("Wrong type 1\n");
		return -1;
	}
	fd = open(filename, premissions, 0777);
	if (fd == -1)
		ft_exit_on_error2("Error on opening input file3");
	return fd;
}
 
int in_redirect(char* filename, int type)
{
	int fd = 0;
	int premissions;
 
	premissions = O_RDONLY;
	if(type != IN_HEREDOC && type != IN_READFILE)
	{
		ft_exit_on_error2("Wrong type 2\n");
		return -1;
	}
	if (type == IN_READFILE)
	{
		fd = open(filename, premissions, 0777);
		if (fd == -1)
		{
			ft_exit_on_error2("Error on opening input file");
		}
	}
	return fd;
}
 
void	ft_file_checker(t_list **cmd_list, t_pipex **pipex)
{
	t_command *cmd;
 
	int i = (*pipex)->i;
	int fd_infile = (*pipex)->fd_in[0];
	int fd_outfile = (*pipex)->fd_out[1];
	int *fd_docks = (*pipex)->fd_docks[(*pipex)->i];
 
	cmd = NULL;
	while (*cmd_list != NULL)
	{
		cmd = (t_command *)(*cmd_list)->content;
		if (cmd->index != i) //index has to start from zero
			break;
		if (cmd->cmd_type == FT_CMD_TYPE_REDIRECT)
		{
			if (!(strcmp(cmd->comm_table[0], ">"))) //comment for system function
			{
				if(fd_outfile != 1)
					close(fd_outfile);
				fd_outfile = out_redirect(cmd->comm_table[1], OUT_WRITE);
			}
			else if (!(strcmp(cmd->comm_table[0], ">>"))) //comment for system function
			{
				if(fd_outfile != 1)
					close(fd_outfile);
				fd_outfile = out_redirect(cmd->comm_table[1], OUT_APPEND);
			}
			else if(strcmp(cmd->comm_table[0], "<") == 0 ) //comment for system function
			{
				if (fd_docks != NULL)
				{
					if(fd_infile != 0 || fd_infile != fd_docks[0])
						close(fd_infile);
				}
				else
				{
					if(fd_infile != 0)
						close(fd_infile);
				}
				fd_infile = in_redirect(cmd->comm_table[1], IN_READFILE);
			}
			else if(!(strcmp(cmd->comm_table[0], "<<")) && fd_docks != NULL) //comment for system function
			{
				if(fd_infile != 0 || fd_infile != fd_docks[0])
					close(fd_infile);
				fd_infile = fd_docks[0];
			}
		}
		else
			break;
		*cmd_list = (*cmd_list)->next;
	}
	if (fd_docks != NULL)
		if (fd_infile != fd_docks[0])
			close(fd_docks[0]);
	if (dup2(fd_infile, STDIN_FILENO) == -1)
		ft_exit_on_error2("File descriptor duplication failed 53");
	if (dup2(fd_outfile, STDOUT_FILENO) == -1)
		ft_exit_on_error2("File descriptor duplication failedi 54");
	close(fd_infile);
	close(fd_outfile);
}
 
void	ft_execute_child(t_list *cmd_list, char **envp, pid_t pid)
{
	t_command *cmd;
 
	if (cmd_list != NULL)
		cmd = (t_command *)(cmd_list->content);
	else
		return ;
 
	if (cmd->cmd_type == FT_CMD_TYPE_SYSTEM)
	{
		if (execve(cmd->path,cmd->comm_table, envp) == -1)
				ft_exit_on_error2("Command execution failed");
	}
 
	if (cmd->cmd_type == FT_CMD_TYPE_BUILT_IN)
	{
		ft_execve(cmd->comm_table, pid);
	}
}
 
void ft_free_pipex_and_reattach_pipes(t_pipex **pipex)
{
	int x = 0;
    int status;
    while (x <= (*pipex)->last_index)
    {
        waitpid((*pipex)->pidt[x], &status, 0);
		if (g_access.last_return)
			free(g_access.last_return);
        g_access.last_return = ft_itoa(WEXITSTATUS(status));
        x++;
    }
	dup2((*pipex)->fd_stream[0], STDIN_FILENO);
	dup2((*pipex)->fd_stream[1], STDOUT_FILENO);
	(*pipex)->i = 0;
	while( (*pipex)->i <= (*pipex)->last_index)
	{
		if((*pipex)->fd_docks[(*pipex)->i] != NULL)
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
 
void ft_pipe_attachment(t_pipex **pipex)
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
 
void ft_pipex_exit(t_pipex **pipex)
{
	(*pipex)->exit_value = ft_atoi(g_access.last_return);
	free_global();
	close(STDERR_FILENO);
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	exit((*pipex)->exit_value);
}

void ft_here_doc_handler(t_pipex **pipex)
{
	if (!strcmp((*pipex)->cmd->comm_table[0], "<<") && (*pipex)->cmd->cmd_type == FT_CMD_TYPE_REDIRECT)  //system function
	{
		if((*pipex)->fd_docks[(*pipex)->cmd->index] != NULL)
		{
			close((*pipex)->fd_docks[(*pipex)->cmd->index][0]);
			free((*pipex)->fd_docks[(*pipex)->cmd->index]);
		}
		(*pipex)->fd_docks[(*pipex)->cmd->index] = (int *)malloc(sizeof(int) * 2);
		if (pipe((*pipex)->fd_docks[(*pipex)->cmd->index]) == -1)
			ft_exit_on_error2("Pipe creation failed");
		pid_t pid = fork();
		if (!pid) //perform check on the here doc child or heredoc parent
			heredoc_child((*pipex)->fd_docks[(*pipex)->cmd->index], (*pipex)->fd_stream, (*pipex)->cmd->comm_table[1], "> ");
		else
			heredoc_parent((*pipex)->fd_docks[(*pipex)->cmd->index], pid);
	}
}
 
void ft_execute_bash_command(t_list **cmd_list, t_pipex **pipex)
{
	if ((*pipex)->i == 0)
		close((*pipex)->fd_in[1]);
	while (cmd_list != NULL)
	{
		(*pipex)->cmd = (t_command *)(*cmd_list)->content;
		if ((*pipex)->cmd->cmd_type != FT_CMD_TYPE_REDIRECT)
			break;
		(*cmd_list) = (*cmd_list)->next;
	}
	if ((*pipex)->cmd->cmd_type == FT_CMD_TYPE_BUILT_IN)
		ft_execve((*pipex)->cmd->comm_table, (*pipex)->pidt[(*pipex)->i]);
	close((*pipex)->fd_in[0]);
	close((*pipex)->fd_out[1]);
	if ((*pipex)->i == (*pipex)->last_index)
		close((*pipex)->fd_out[0]);
}
 
void ft_setup_pipex(t_list **cmd_list, t_pipex **pipex)
{
	*pipex = ft_calloc(sizeof(t_pipex), 1);
	(*pipex)->cmd_list_temp = NULL;
	(*pipex)->fd_stream[0] = 0;
	(*pipex)->fd_stream[1] = 0;
	ft_initialize_fds((*pipex)->fd_stream);
	(*pipex)->last_index = ((t_command *)ft_lstlast((*cmd_list))->content)->index;
	(*pipex)->cmd_list_temp = (*cmd_list);
	(*pipex)->fd_docks = ft_calloc((*pipex)->last_index + 1, sizeof(int *));
	(*pipex)->i = 0;
}
 
void ft_child_handler(t_list **cmd_list, t_pipex **pipex)
{
	ft_pipe_attachment(pipex);
	ft_file_checker(cmd_list, pipex);
	if((*pipex)->i == 0)
		close((*pipex)->fd_in[1]);
	(*pipex)->i = 0;
	while( (*pipex)->i <= (*pipex)->last_index)
	{
		if((*pipex)->fd_docks[(*pipex)->i] != NULL)
			free((*pipex)->fd_docks[(*pipex)->i]);
		(*pipex)->i++;
	}
	if ((*pipex)->fd_docks != NULL)
		free((*pipex)->fd_docks);
}
 
void ft_pipex_itterator(t_list **cmd_list, t_pipex **pipex, char **envp)
{
	while ((*cmd_list) != NULL)
	{
		(*pipex)->fd_in[0] = (*pipex)->fd_out[0];
		if (pipe((*pipex)->fd_out) == -1)
			ft_exit_on_error2("Pipe creation failed in line 124");
		(*pipex)->pidt[(*pipex)->i] = fork();
		if ((*pipex)->pidt[(*pipex)->i] == -1)
			ft_exit_on_error2("Pipe creation failed in line 127");
		else if((*pipex)->pidt[(*pipex)->i] == 0)
		{
			ft_child_handler(cmd_list, pipex);
			int temp = (*pipex)->pidt[(*pipex)->i];
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

int pipex(t_list *cmd_list, char** envp)
{
	t_pipex *pipex;
 
	ft_setup_pipex(&cmd_list, &pipex);
	while(pipex->cmd_list_temp != NULL)
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