/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdragomi <vdragomi@students.42wolfsburg.de +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/24 17:45:15 by vdragomi          #+#    #+#             */
/*   Updated: 2022/02/24 17:45:15 by vdragomi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

char	**ft_split_path(char **env)
{
	int		i;
	char	**split;

	i = 0;
	while (env[i] != NULL)
	{
		if (ft_memcmp(env[i], "PATH=", 5) == 0)
		{
			split = ft_split(ft_strchr(env[i], '/'), ':');
			return (split);
		}
		i++;
	}
	exit(EXIT_FAILURE);
}

char	*ft_get_cmd_path(const char *cmd, char **path_list)
{
	int		j;
	char	*path;

	j = 0;
	path = NULL;
	while (path_list[j] != NULL)
	{
		if (ft_check_cmd_path(&path, path_list, j, cmd) != NULL)
		{
			return (path);
		}
		j++;
	}
	return (path);
}

char	*ft_check_cmd_path(char **path, char **split, int j, const char *cmd)
{
	*path = ft_strjoin_with_free(ft_strjoin(split[j], "/"), cmd);
	if (access(*path, F_OK) == 0)
	{
		errno = 0;
		return (*path);
	}
	else
	{
		free(*path);
		*path = NULL;
		return (*path);
	}
}

char	*ft_strjoin_with_free(char *s1, char const *s2)
{
	char	*return_s;

	if (!s1 || !s2)
		return (NULL);
	return_s = ft_calloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2)) + 1, 1);
	if (return_s == NULL)
		return (NULL);
	ft_memcpy(return_s, s1, ft_strlen(s1));
	ft_memcpy(return_s + ft_strlen(s1), s2, ft_strlen(s2));
	if (s1 != NULL)
		free (s1);
	s1 = NULL;
	return (return_s);
}
