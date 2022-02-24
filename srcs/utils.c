/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdragomi <vdragomi@students.42wolfsburg.de +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/24 17:55:31 by vdragomi          #+#    #+#             */
/*   Updated: 2022/02/24 17:55:31 by vdragomi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

/*
**  @brief Compares two string
**  @param s1: First string to compare
**  @param s2: Second string to compare
**  @return Returns value diffrent from 0 if strings diffrent
*/
int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i] == s2[i] && s1[i] != '\0' && s2[i] != '\0')
		i++;
	return (s1[i] - s2[i]);
}

void	ft_set_global_pwd(char **env)
{
	int	i;

	i = 1;
	if (*env)
		free(*env);
	*env = getcwd(NULL, 0);
	while (getcwd(*env, i) == NULL)
		i++;
}

int	ft_count_arguments(t_list *cmd_list)
{
	int	counter;

	counter = 0;
	while (cmd_list != NULL)
	{
		counter++;
		cmd_list = cmd_list->next;
	}
	return (counter);
}

int	ft_execve(char **args, pid_t pid)
{
	t_list	*ptr;

	ptr = g_access.builtins;
	while (ptr)
	{
		if (!ft_strncmp(args[0], ((t_builtin_content *)(ptr->content))->cmd, \
		(ft_strlen(((t_builtin_content *)(ptr->content))->cmd))))
		{
			return \
			(((t_builtin_content *)ptr->content)->minishell_fct(args, pid));
		}
		ptr = ptr->next;
	}
	return (0);
}
