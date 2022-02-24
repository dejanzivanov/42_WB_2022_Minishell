/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzivanov <dzivanov@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/24 17:12:46 by dzivanov          #+#    #+#             */
/*   Updated: 2022/02/24 17:12:53 by dzivanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

void	ft_print_parser_error(char *str)
{
	write(2, "minishe11: ", 12);
	write(2, str, ft_strlen(str));
	write(2, ": No such file or directory\n", 28);
}

void	ft_clear_env_check(char **str, char *temp_path)
{
	ft_print_parser_error(*str);
	ft_set_lasts(NULL, 0, 127, 2);
	ft_free_secure((void *)&temp_path);
}

int	ft_env_itter(char **temp_path, char ***split, char ***cmd_path)
{
	if (access(*temp_path, F_OK) == 0)
	{
		errno = 0;
		**cmd_path = ft_strdup(*temp_path);
		ft_free_split(*split);
		ft_free_secure((void *)&(*temp_path));
		return (2);
	}
	else
		ft_free_secure((void *)&(*temp_path));
	return (0);
}

void	ft_setting_path_and_split(char **path, char ***split)
{
	*path = env_value_finder("PATH");
	*split = ft_split(ft_strchr(*path, '/'), ':');
}

int	ft_access_check(char ***cmd_path, char **str)
{
	if (access(*str, F_OK) == 0)
	{
		**cmd_path = ft_strdup(*str);
		return (1);
	}
	return (0);
}
