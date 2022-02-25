/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pipex_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzivanov <dzivanov@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/25 16:53:24 by vdragomi          #+#    #+#             */
/*   Updated: 2022/02/25 17:28:58 by dzivanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

void	ft_initialize_fds(int *fd_temp)
{
	dup2(STDIN_FILENO, fd_temp[0]);
	dup2(STDOUT_FILENO, fd_temp[1]);
}

int	out_redirect(char *filename, int type)
{
	int	fd;
	int	premissions;

	if (type == OUT_WRITE)
		premissions = O_WRONLY | O_CREAT | O_TRUNC;
	else if (type == OUT_APPEND)
		premissions = O_WRONLY | O_APPEND | O_CREAT ;
	else
	{
		ft_exit_on_error2("Wrong type 1\n");
		return (-1);
	}
	fd = open(filename, premissions, 0777);
	if (fd == -1)
		ft_exit_on_error2("Error on opening input file3");
	return (fd);
}

int	in_redirect(char *filename, int type)
{
	int	fd;
	int	premissions;

	fd = 0;
	premissions = O_RDONLY;
	if (type != IN_HEREDOC && type != IN_READFILE)
	{
		ft_exit_on_error2("Wrong type 2\n");
		return (-1);
	}
	if (type == IN_READFILE)
	{
		fd = open(filename, premissions, 0777);
		if (fd == -1)
		{
			ft_exit_on_error2("Error on opening input file");
		}
	}
	return (fd);
}

void	ft_infile_handler(int *fd_docks, int *fd_infile, t_command *cmd)
{
	if (fd_docks != NULL)
	{
		if (*fd_infile != 0 || *fd_infile != fd_docks[0])
			close(*fd_infile);
	}
	else
	{
		if (*fd_infile != 0)
			close(*fd_infile);
	}
	*fd_infile = in_redirect(cmd->comm_table[1], IN_READFILE);
}

void	ft_re_attach_docs(int **fd_docks, int *fd_infile, int *fd_outfile)
{
	if (*fd_docks != NULL)
		if (*fd_infile != *fd_docks[0])
			close(*fd_docks[0]);
	if (dup2(*fd_infile, STDIN_FILENO) == -1)
		ft_exit_on_error2("File descriptor duplication failed 53");
	if (dup2(*fd_outfile, STDOUT_FILENO) == -1)
		ft_exit_on_error2("File descriptor duplication failedi 54");
	close(*fd_infile);
	close(*fd_outfile);
}
