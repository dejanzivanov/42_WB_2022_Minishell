#include "../incl/minishell.h"

static void	ft_print_sort_helper(t_list *ptr)
{
	int	i;

	i = 0;
	write(1, "declare -x ", 11);
	write(1, ((t_env_var *)(ptr->content))->name, \
		ft_strlen(((t_env_var *)(ptr->content))->name));
	if (((t_env_var *)(ptr->content))->value != NULL)
	{
		write(1, "\"", 1);
		i = 0;
		while (((t_env_var *)(ptr->content))->value[i] != '\0')
		{
			if (((t_env_var *)(ptr->content))->value[i] == '$' || \
				(((t_env_var *)(ptr->content))->value)[i] == '\"')
				write(1, "\\", 1);
			write(1, &((t_env_var *)(ptr->content))->value[i], 1);
			i++;
		}
		write(1, "\"", 1);
	}
	write(1, "\n", 1);
}

void	ft_print_sorted_copy(t_list *env_cpy)
{
	t_list	*ptr;

	ptr = env_cpy;
	while (ptr)
	{
		if (ft_strncmp(((t_env_var *)(ptr->content))->name, "_=", 2))
			ft_print_sort_helper(ptr);
		ptr = ptr->next;
	}
}

int	ft_single_export(void)
{
	t_list	*env_cpy;

	env_cpy = ft_copy_env();
	ft_print_sorted_copy(env_cpy);
	if (env_cpy)
		ft_free_linked_list(&env_cpy, FT_LIST_TYPE_ENV_VAR, 1);
	return (1);
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
