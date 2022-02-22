#include "../incl/minishell.h"

void	ft_update_create_env(char *env, char *value, pid_t pid)
{
	t_list *ptr;
	char *args[3];

	ptr = g_access.env;
	while (ptr != NULL)
	{
		if (ft_strncmp(((t_env_var *)(ptr->content))->name, env, ft_strlen(env)) == 0)
		{
			if ((ft_strlen(((t_env_var *)(ptr->content))->name) - 1) == ft_strlen(env))
			{
				if (((t_env_var *)(ptr->content))->value != NULL)
					free(((t_env_var *)(ptr->content))->value);
				((t_env_var *)(ptr->content))->value = ft_strdup(value);
				return ;
			}
		}
		ptr = ptr->next;
	}
	args[0] = "export";
	args[1] = ft_strjoin_with_free(ft_strjoin(env, "="), value);
	args[2] = NULL;
	minishell_export(args, pid);
	if (args[1] != NULL)
		free(args[1]);
}

