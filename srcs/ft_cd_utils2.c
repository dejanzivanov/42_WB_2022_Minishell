/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdragomi <vdragomi@students.42wolfsburg.de +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/24 17:48:07 by vdragomi          #+#    #+#             */
/*   Updated: 2022/02/24 17:48:07 by vdragomi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

static void	ft_args_setter(char *args[], char *env, char *value)
{
	args[0] = "export";
	args[1] = ft_strjoin_with_free(ft_strjoin(env, "="), value);
	args[2] = NULL;
}

void	ft_update_create_env(char *env, char *value, pid_t pid)
{
	t_list	*ptr;
	char	*args[3];

	ft_args_setter(args, env, value);
	ptr = g_access.env;
	while (ptr != NULL)
	{
		if (ft_strncmp(((t_env_var *)(ptr->content))->name, \
			env, ft_strlen(env)) == 0)
		{
			if \
	((ft_strlen(((t_env_var *)(ptr->content))->name) - 1) == ft_strlen(env))
			{
				if (((t_env_var *)(ptr->content))->value != NULL)
					free(((t_env_var *)(ptr->content))->value);
				((t_env_var *)(ptr->content))->value = ft_strdup(value);
				ft_free_secure((void *)(&args[1]));
				return ;
			}
		}
		ptr = ptr->next;
	}
	minishell_export(args, pid);
	ft_free_secure((void *)(&args[1]));
}

void	ft_set_current_path(char **current_path)
{
	if (g_access.dp != NULL)
		*current_path = ft_strdup(g_access.dp);
	else
		ft_set_global_pwd(current_path);
}

void	ft_cd_simlink(char *abs_path, char *current_path, int pid, int mode)
{
	if (env_value_finder("PWD") == NULL \
	|| ft_strlen(env_value_finder("PWD")) == 0)
		ft_update_create_env("OLDPWD", "", pid);
	else
		ft_update_create_env("OLDPWD", current_path, pid);
	chdir(abs_path);
	if (mode == 1)
	{
		if (g_access.dp != NULL)
			free(g_access.dp);
		g_access.dp = ft_strdup(abs_path);
	}
	else if (mode == 2)
	{
		if (g_access.dp != NULL)
		{
			free(g_access.dp);
			g_access.dp = NULL;
		}
	}
	if (g_access.pwd != NULL)
		free(g_access.pwd);
	g_access.pwd = ft_strdup(abs_path);
	ft_update_env("PWD", abs_path);
}

int	ft_cd_error_handler(char *str, pid_t pid, char **path, char **temp)
{
	if (pid == 0)
		write(2, str, ft_strlen(str));
	if (*path != NULL)
	{
		free(*path);
		*path = NULL;
	}
	free(temp[0]);
	free(temp[1]);
	free(g_access.last_return);
	g_access.last_return = ft_itoa(1);
	return (1);
}
