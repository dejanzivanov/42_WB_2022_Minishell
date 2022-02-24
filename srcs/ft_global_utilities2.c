/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_global_utilities2.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdragomi <vdragomi@students.42wolfsburg.de +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/24 18:18:44 by vdragomi          #+#    #+#             */
/*   Updated: 2022/02/24 18:18:44 by vdragomi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

void	ft_checking(t_env_var **env_var, t_list **ptr_cpy)
{
	t_list	*tmp;

	tmp = ft_lstnew(*env_var);
	tmp->next = (*ptr_cpy)->next;
	(*ptr_cpy)->next = tmp;
	return ;
}

int	ft_check_existing_env(t_env_var **env_var)
{
	t_list	*ptr;

	ptr = g_access.env;
	while (ptr)
	{
		if (!ft_strncmp((*env_var)->name, ((t_env_var *)(ptr->content))->name, \
			ft_strlen((*env_var)->name)))
		{
			free(((t_env_var *)(ptr->content))->value);
			((t_env_var *)(ptr->content))->value = ft_strdup((*env_var)->value);
			free((*env_var)->name);
			free((*env_var)->value);
			free(*env_var);
			*env_var = NULL;
			return (1);
		}
		ptr = ptr->next;
	}
	return (0);
}
