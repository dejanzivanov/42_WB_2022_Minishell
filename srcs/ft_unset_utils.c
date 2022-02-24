/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdragomi <vdragomi@students.42wolfsburg.de +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/24 17:41:58 by vdragomi          #+#    #+#             */
/*   Updated: 2022/02/24 17:44:20 by vdragomi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

int	ft_validity_setter(char **args, int i, int pid)
{
	int	j;

	j = 0;
	while (args[i][j] != '=' && args[i][j] != '\0')
	{
		if (j == 0 && (args[i][j] == '_' || ft_isalpha(args[i][j])))
			j++;
		else if (j > 0 && (args[i][j] == '_' || ft_isalnum(args[i][j])))
			j++;
		else
		{
			if (pid == 0)
			{
				write(2, "minishell: unset: `", 20);
				write(2, args[i], ft_strlen(args[i]));
				write(2, "': not a valid identifier\n", 26);
			}
			ft_set_lasts(args, pid, 1, 2);
			return (0);
		}
	}
	return (1);
}

int	ft_unsetter_first_elem(char **args, int i, t_list *ptr, int pid)
{
	if (ft_strlen(args[i]) == \
		ft_strlen(((t_env_var *)(ptr->content))->name) - 1)
	{
		if (!ft_strncmp(args[i], \
			((t_env_var *)(ptr->content))->name, ft_strlen(args[i])))
		{
			g_access.env = ptr->next;
			ft_lstdelone(ptr, delone);
			ft_set_lasts(args, pid, 0, 1);
			return (0);
		}
	}
	return (1);
}

int	ft_unsetter(char **args, int i, t_list *ptr, int pid)
{
	t_list	*temp;

	if (ft_strlen(args[i]) == \
		ft_strlen(((t_env_var *)(ptr->next->content))->name) - 1)
	{
		if (!ft_strncmp(args[i], \
			((t_env_var *)(ptr->next->content))->name, ft_strlen(args[i])))
		{
			if (ptr->next->next != NULL)
			{
				temp = ptr->next;
				ptr->next = ptr->next->next;
				ft_lstdelone(temp, delone);
				return (0);
			}
			else
			{
				ft_lstdelone(ptr->next, delone);
				ptr->next = NULL;
				ft_last_arg(args, pid);
				return (0);
			}
		}
	}
	return (1);
}

int	ft_unset_engine(char **args, int i, t_list *ptr, int pid)
{
	int	valid;

	valid = ft_validity_setter(args, i, pid);
	ptr = g_access.env;
	if (!ft_strncmp(args[i], "PWD", ft_strlen(args[i])))
		ft_free_secure((void *)&g_access.pwd);
	if (ft_strncmp(args[i], "_", ft_strlen(args[i]) != 0))
	{
		while (ptr->next != NULL && valid == 1)
		{
			if (ft_unsetter_first_elem(args, i, ptr, pid) == 0)
				return (0);
			if (ft_unsetter(args, i, ptr, pid) == 0)
				return (0);
			ptr = ptr->next;
		}
	}
	return (1);
}
