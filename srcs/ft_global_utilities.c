/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_global_utilities.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdragomi <vdragomi@42student.wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/21 11:59:20 by vdragomi          #+#    #+#             */
/*   Updated: 2022/02/21 20:45:38 by vdragomi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

static void ft_elem_copy1(t_env_var **env_var, t_list **env_cpy, t_list **ptr_cpy)
{
	if (ft_strcmp((*env_var)->name, ((t_env_var *)((*ptr_cpy)->content))->name) < 0)
		ft_lstadd_front(env_cpy, ft_lstnew(*env_var));
	else if (ft_strcmp((*env_var)->name, \
		((t_env_var *)(ft_lstlast(*env_cpy)->content))->name) > 0)
		ft_lstadd_back(env_cpy, ft_lstnew(*env_var));
}

void	ft_elem_copy(t_env_var **env_var, t_list **env_cpy, t_list **ptr_cpy)
{
	t_list *tmp;

	tmp = NULL;
	if (ft_strcmp((*env_var)->name, ((t_env_var *)((*ptr_cpy)->content))->name) < 0)
		ft_lstadd_front(env_cpy, ft_lstnew(*env_var));
	else if (ft_strcmp((*env_var)->name, ((t_env_var *)(ft_lstlast(*env_cpy)->content))->name) > 0)
		ft_lstadd_back(env_cpy, ft_lstnew(*env_var));
	else
	{
		while ((*ptr_cpy)->next != NULL)
		{
			if (ft_strcmp((*env_var)->name, ((t_env_var *)((*ptr_cpy)->content))->name) > 0 && ft_strcmp((*env_var)->name, ((t_env_var *)((*ptr_cpy)->next->content))->name) < 0)
			{
				tmp =ft_lstnew(*env_var);
				tmp->next = (*ptr_cpy)->next;
				(*ptr_cpy)->next = tmp;
				return ;
			}
			*ptr_cpy=(*ptr_cpy)->next;
		}
	}
}

t_list *ft_copy_env(void)
{
	t_list *env_cpy;
	t_list *ptr_env;
	t_list *ptr_cpy;
	t_env_var *env_var;

	ptr_env = g_access.env;
	env_cpy = NULL;
	while (ptr_env)
	{
		ptr_cpy = env_cpy;
		env_var = (t_env_var *)malloc(sizeof(t_env_var));
		env_var->name = ft_strdup(((t_env_var *)(ptr_env->content))->name);
		env_var->value = ft_strdup(((t_env_var *)(ptr_env->content))->value);
		if (ft_lstsize(env_cpy) == 0)
		{
			printf("TESTING1\n");
			ft_lstadd_back(&(env_cpy), ft_lstnew(env_var));
		}
		else if (ft_lstsize(env_cpy) == 1)
		{
			printf("TESTING2\n");
			ft_elem_copy1(&env_var, &env_cpy, &ptr_cpy);
		}
		else
		{
			printf("TESTING3\n");
			ft_elem_copy(&env_var, &env_cpy, &ptr_cpy);
		}
		ptr_env = ptr_env->next;
	}
	return(env_cpy);
}

void	ft_last_arg(char **args, pid_t pid)
{
	int	i;

	i = 0;
	if (!*args)
		ft_update_create_env("_", "", pid);
	while (args[i] != NULL)
		i++;
	ft_update_create_env("_", args[i - 1], pid);
}

/*
**	if mode = 1 => we change only the $_
**	if mode = 2 => we change only the $?
**	if mode = 3 => we change both the $? and $_
*/

void	ft_set_lasts(char **args, int pid, int lreturn, int mode)
{
	if (!(mode == 1 || mode == 2 || mode == 3))
		write(2, "ERROR ON SET LAST\n", 18);
	if (mode == 1)
		ft_last_arg(args, pid);
	else if (mode == 2)
	{
		if (g_access.last_return != NULL)
			free(g_access.last_return);
		g_access.last_return = ft_itoa(lreturn);
	}
	else if (mode == 3)
	{
		if (g_access.last_return != NULL)
			free(g_access.last_return);
		g_access.last_return = ft_itoa(lreturn);
		ft_last_arg(args, pid);
	}
}
