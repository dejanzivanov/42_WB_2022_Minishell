/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pipex_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdragomi <vdragomi@42student.wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/25 16:59:17 by vdragomi          #+#    #+#             */
/*   Updated: 2022/02/25 16:59:27 by vdragomi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

void	ft_append_handler(int *fd_outfile, t_command *cmd)
{
	if (*fd_outfile != 1)
		close(*fd_outfile);
	*fd_outfile = out_redirect(cmd->comm_table[1], OUT_APPEND);
}

void	ft_in_infile_handler(int **fd_docks, int *fd_infile)
{
	if (*fd_infile != 0 && *fd_infile != *fd_docks[0])
		close(*fd_infile);
	*fd_infile = *fd_docks[0];
}

void	ft_outfile_handler(int *fd_outfile, t_command *cmd)
{
	if (*fd_outfile != 1)
		close(*fd_outfile);
	*fd_outfile = out_redirect(cmd->comm_table[1], OUT_WRITE);
}

int	ft_file_itterator(t_command *cmd, \
	int *fd_outfile, int *fd_infile, int **fd_docks)
{
	if (cmd->cmd_type == FT_CMD_TYPE_REDIRECT)
	{
		if (!(ft_strcmp(cmd->comm_table[0], ">")))
			ft_outfile_handler(fd_outfile, cmd);
		else if (!(ft_strcmp(cmd->comm_table[0], ">>")))
			ft_append_handler(fd_outfile, cmd);
		else if (ft_strcmp(cmd->comm_table[0], "<") == 0)
			ft_infile_handler(*fd_docks, fd_infile, cmd);
		else if (!(ft_strcmp(cmd->comm_table[0], "<<")) && fd_docks != NULL)
			ft_in_infile_handler(fd_docks, fd_infile);
	}
	else
		return (1);
	return (0);
}

void	ft_file_checker(t_list **cmd_list, t_pipex **pipex)
{
	t_command	*cmd;
	int			i;
	int			fd_infile;
	int			fd_outfile;
	int			*fd_docks;

	i = (*pipex)->i;
	fd_infile = (*pipex)->fd_in[0];
	fd_outfile = (*pipex)->fd_out[1];
	fd_docks = (*pipex)->fd_docks[(*pipex)->i];
	cmd = NULL;
	while (*cmd_list != NULL)
	{
		cmd = (t_command *)(*cmd_list)->content;
		if (cmd->index != i)
			break ;
		if (ft_file_itterator(cmd, &fd_outfile, &fd_infile, &fd_docks) == 1)
			break ;
		*cmd_list = (*cmd_list)->next;
	}
	ft_re_attach_docs(&fd_docks, &fd_infile, &fd_outfile);
}