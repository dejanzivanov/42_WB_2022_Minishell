/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_path_mac.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdragomi <vdragomi@students.42wolfsburg.de +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/24 18:18:26 by vdragomi          #+#    #+#             */
/*   Updated: 2022/02/24 18:18:26 by vdragomi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

static int	ft_check_path(t_list **ptr)
{
	while (*ptr)
	{
		if (!ft_strncmp(((t_env_var *)((*ptr)->content))->name, "PATH=", 5))
		{
			if (((t_env_var *)((*ptr)->content))->value == NULL || \
				ft_strlen(((t_env_var *)((*ptr)->content))->value) == 0)
				return (0);
			else
				return (1);
		}
		*ptr = (*ptr)->next;
	}
	return (0);
}

static void	ft_path_setter(char *s, int fd, char **path)
{
	char	*temp;

	while (s != NULL)
	{
		if (s[ft_strlen(s) - 1] == '\n')
			temp = ft_substr(s, 0, ft_strlen(s) - 1);
		else
			temp = ft_strdup(s);
		free(s);
		if (ft_strlen(*path) != 0)
			*path = \
		ft_strjoin_with_dfree(temp, ft_strjoin_with_scnd_free(":", *path));
		else
		{
			*path = ft_strdup(temp);
			if (temp != NULL)
				free(temp);
		}
		s = get_next_line(fd);
	}
}

static void	ft_path_adder(char *path, t_list **ptr, t_env_var **env_var)
{
	if (*ptr != NULL)
	{
		if (((t_env_var *)((*ptr)->content))->value != NULL)
			free(((t_env_var *)((*ptr)->content))->value);
		((t_env_var *)((*ptr)->content))->value = ft_strdup(path);
	}
	else
	{
		*env_var = (t_env_var *)malloc(sizeof(t_env_var));
		(*env_var)->name = ft_strdup("PATH=");
		(*env_var)->value = ft_strdup(path);
		ft_lstadd_back(&(g_access.env), ft_lstnew(*env_var));
	}
}

void	ft_get_path(void)
{
	int			fd;
	char		*s;
	char		*path;
	t_list		*ptr;
	t_env_var	*env_var;

	ptr = g_access.env;
	if (ft_check_path(&ptr) == 1)
		return ;
	fd = open("/etc/paths", O_RDONLY);
	s = get_next_line(fd);
	path = ft_strdup("");
	ft_path_setter(s, fd, &path);
	close(fd);
	ft_path_adder(path, &ptr, &env_var);
	free(path);
}
