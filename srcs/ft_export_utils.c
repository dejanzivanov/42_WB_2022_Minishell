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

int	ft_single_export(char **args, int pid, int lreturn, int mode)
{
	t_list	*env_cpy;

	ft_set_lasts(args, pid, lreturn, mode);
	env_cpy = ft_copy_env();
	ft_print_sorted_copy(env_cpy);
	if (env_cpy)
		ft_free_linked_list(&env_cpy, FT_LIST_TYPE_ENV_VAR, 1);
	return (1);
}

int	ft_export_error_checker(char **args, int i, int pid)
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
				write(2, "minishell: export: `", 20);
				write(2, args[i], ft_strlen(args[i]));
				write(2, "': not a valid identifier\n", 26);
			}
			ft_set_lasts(args, pid, 1, 2);
			return (j);
		}
	}
	return (j);
}

void	ft_add_env_export(char **args, int i, int j, int valid)
{
	t_env_var	*env_var;

	if ((args[i][j] == '=' || args[i][j] == '\0') && valid)
	{
		env_var = (t_env_var *)malloc(sizeof(t_env_var));
		if (args[i][j] == '=')
		{
			env_var->name = ft_substr(args[i], 0, j + 1);
			env_var->value = ft_strdup(&(args[i][j + 1]));
		}
		else
		{
			env_var->name = ft_strdup(&(args[i][0]));
			env_var->value = NULL;
		}
		if (!ft_check_existing_env(&env_var))
			ft_lstadd_back(&(g_access.env), ft_lstnew(env_var));
	}
}
