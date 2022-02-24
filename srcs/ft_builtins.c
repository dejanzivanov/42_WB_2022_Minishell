/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtins.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdragomi <vdragomi@students.42wolfsburg.de +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/24 18:13:48 by vdragomi          #+#    #+#             */
/*   Updated: 2022/02/24 18:13:48 by vdragomi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

/**
	 @brief Bultin command: env.
	 @param args List of args
	 @return Always returns 1, to continue executing.
 */
int	minishell_env(char **args, pid_t pid)
{
	t_list	*ptr;

	ft_set_lasts(args, pid, 0, 3);
	if (ft_strncmp(args[0], "env", 3))
		return (0);
	ptr = g_access.env;
	if (pid == 0)
	{
		while (ptr != NULL)
		{
			if (((t_env_var *)(ptr->content))->value != NULL)
			{
				write(1, ((t_env_var *)(ptr->content))->name, \
					ft_strlen(((t_env_var *)(ptr->content))->name));
				write(1, ((t_env_var *)(ptr->content))->value, \
					ft_strlen(((t_env_var *)(ptr->content))->value));
				write(1, "\n", 1);
			}
			if (ptr->next != NULL)
				ptr = ptr->next;
			else
				break ;
		}
	}
	return (1);
}

/**
	 @brief Builtin command: exit.
	 @param args List of args.	Not examined.
	 @return Always returns 0, to terminate execution.
 */
int	minishell_exit(char **args, pid_t pid)
{
	int	counter;

	ft_set_lasts(args, pid, 0, 1);
	counter = ((t_command *)ft_lstlast(g_access.parser2exec)->content)->index;
	if (counter == 0 && pid != 0)
		return (ft_parent_exiter(pid, args));
	else
		ft_child_exiter(args);
	return (0);
}

/**
	@brief Builtin command: pwd.
	@param args List of args.	Not examined. (and no need to)
	@return Always returns 1, to continue execution.
 */
int	minishell_pwd(char **args, pid_t pid)
{
	char	*buf;
	int		i;

	i = 1;
	ft_set_lasts(args, pid, 0, 3);
	if (args[0] == NULL)
		return (0);
	if (pid == 0)
	{
		if (g_access.dp == NULL)
		{
			buf = getcwd(NULL, 0);
			while (getcwd(buf, i) == NULL)
				i++;
			write(1, buf, ft_strlen(buf));
			free(buf);
		}
		else
			write(1, g_access.dp, ft_strlen(g_access.dp));
		write(1, "\n", 1);
	}
	return (1);
}

/**
	@brief Builtin command: export.
	@param args List of args.	Not examined. (and no need to)
	@return Always returns 1, to continue execution.
 */
int	minishell_export(char **args, pid_t pid)
{
	int			len;
	int			j;
	int			i;
	int			valid;

	len = 0;
	while (args[len] != 0)
		len++;
	ft_set_lasts(args, pid, 0, 2);
	if (args[1] == NULL && pid == 0)
		return (ft_single_export(args, pid, 0, 1));
	i = 1;
	while (i < len)
	{
		j = ft_export_error_checker(args, i, pid);
		valid = 1;
		if (args[i][j] != '=')
			valid = 0;
		ft_add_env_export(args, i, j, valid);
		i++;
	}
	ft_set_lasts(args, pid, 0, 1);
	return (1);
}

/**
	@brief Builtin command: unset.
	@param args List of args.	Not examined. (and no need to)
	@return Always returns 1, to continue execution.
 */

int	minishell_unset(char **args, pid_t pid)
{
	t_list	*ptr;
	int		i;
	int		len;

	len = 0;
	i = 1;
	ft_set_lasts(args, pid, 0, 3);
	ptr = g_access.env;
	while (args[len] != 0)
		len++;
	while (i < len)
	{
		if (ft_unset_engine(args, i, ptr, pid) == 0 && i == len - 1)
			break ;
		i++;
	}
	return (1);
}
