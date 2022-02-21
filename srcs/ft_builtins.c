#include "../incl/minishell.h"

// 3:30 PM] Margarete M: Ctrl-D on bash writes an "exit"
// [3:36 PM] Margarete M: and last_return needs to be passt after leaving minishell like this
// [3:38 PM] Vlad D: I see that now
// 3:50 PM] Margarete M: and ctrl-c and ctrl-d should also print a new line
// 3:50 PM] Margarete M: and ctrl-c and ctrl-d should also print a new line

/**
	 @brief Bultin command: env.
	 @param args List of args
	 @return Always returns 1, to continue executing.
 */
int	minishell_env(char **args, pid_t pid)
{
	t_list *ptr;

	ft_set_lasts(args, pid, 0, 3);
	if (ft_strncmp(args[0], "env", 3))
		return (0);
	ptr = g_access.env;
	if (pid == 0)
	{
		while (ptr != NULL)
		{
			if (((t_env_var*)(ptr->content))->value != NULL)
			{
				write(1, ((t_env_var*)(ptr->content))->name, \
					ft_strlen(((t_env_var*)(ptr->content))->name));
				write(1, ((t_env_var*)(ptr->content))->value, \
					ft_strlen(((t_env_var*)(ptr->content))->value));
				write(1, "\n", 1);
			}
			if (ptr->next != NULL)
				ptr = ptr->next;
			else
				break;
		}
	}
	return (1);
}



/**
	 @brief Builtin command: exit.
	 @param args List of args.	Not examined.
	 @return Always returns 0, to terminate execution.
	 @todo free linked lists etc properly; fix arguments that need to be passed!
 */
int	minishell_exit(char **args, pid_t pid)
{
	int counter;
	int len;

	len = 0;
	// ft_last_arg(args, pid);
	ft_set_lasts(args, pid, 0, 1);
	counter = ((t_command *)ft_lstlast(g_access.parser2exec)->content)->index;
	if (counter == 0 && pid != 0)
		return (ft_parent_exiter(pid, args));
	else
		ft_child_exiter(args);
	return (0);
}

/**
	@brief Builtin command: pwd.
	@param args List of args.	Not examined. (and no need to)
	@return Always returns 1, to continue execution.
 */
int minishell_pwd(char **args, pid_t pid)
{
	char *buf;
	int i = 1;

	// ft_last_arg(args, pid);
	// free(g_access.last_return);
	// g_access.last_return = ft_itoa(0);
	ft_set_lasts(args, pid, 0, 3);
	if (args[0] == NULL) //in order to prevent compiler errors
		return (0);

	if (pid == 0)
	{
		if (g_access.dp == NULL)
		{
			buf = getcwd(NULL, 0);
			while(getcwd(buf, i) == NULL)
				i++;
			write(1, buf, ft_strlen(buf));
			free(buf);
		}
		else
			write(1, g_access.dp, ft_strlen(g_access.dp));
		write(1, "\n", 1);
	}
	return (1);
}

/**
	@brief Builtin command: export.
	@param args List of args.	Not examined. (and no need to)
	@return Always returns 1, to continue execution.
	@todo Check for export directly after minishell execution on MAC. Is OLDPWD printed? If yes, insert exception.
 */
int minishell_export(char **args, pid_t pid)
{
	t_env_var *env_var;
	int len;
	int j;
	int i;
	int valid;

	len = 0;
	while (args[len] != 0)
		len++;
	valid = 1;

	// free(g_access.last_return);
	// g_access.last_return = ft_itoa(0);
	ft_set_lasts(args, pid, 0, 2);
	if (args[1] == NULL && pid == 0)
	{
		// ft_last_arg(args, pid);
		ft_set_lasts(args, pid, 0, 1);
		return (ft_single_export());
	}
	j = 0;
	i = 1;
	while (i < len)
	{
		j = 0;
		valid = 1;
		while (args[i][j] != '=' && args[i][j] != '\0')
		{
			if (j == 0 && (args[i][j] == '_' || ft_isalpha(args[i][j])))
				j++;
			else if (j > 0 && (args[i][j] == '_' || ft_isalnum(args[i][j])))
				j++;
			else
			{
				valid = 0;
				if (pid == 0)
				{
					write(2, "minishell: export: `", 20); //bash: export: `4hehe=he': not a valid identifier
					write(2 , args[i], ft_strlen(args[i]));
					write(2, "': not a valid identifier\n", 26);
				}
				// free(g_access.last_return);
				// g_access.last_return = ft_itoa(1);
				ft_set_lasts(args, pid, 1, 2);
				break;
				j++;
			}
		}
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
		i++;
	}
	ft_set_lasts(args, pid, 0, 1);
	return (1);
}

/**
	@brief Builtin command: unset.
	@param args List of args.	Not examined. (and no need to)
	@return Always returns 1, to continue execution.
 */
int minishell_unset(char **args, pid_t pid)
{
	t_list *ptr;
	t_list *temp;
	int valid;

	valid = 1;
	temp = NULL;
	// ft_last_arg(args, pid);
	// free(g_access.last_return);
	// g_access.last_return = ft_itoa(0);
	ft_set_lasts(args, pid, 0, 3);
	ptr = g_access.env;
	int len = 0;
	while (args[len] != 0)
		len++;
	valid = 1;
	int i = 1;
	while (i < len)
	{
		int j = 0;
		valid = 1;
		ptr = g_access.env;
		while (args[i][j] != '=' && args[i][j] != '\0')
		{
			if (j == 0 && (args[i][j] == '_' || ft_isalpha(args[i][j])))
				j++;
			else if (j > 0 && (args[i][j] == '_' || ft_isalnum(args[i][j])))
				j++;
			else
			{
				valid = 0;
				if (pid == 0)
				{
					write(2, "minishell: unset: `", 20);
					write(2 , args[i], ft_strlen(args[i]));
					write(2, "': not a valid identifier\n", 26);

				}
				// free(g_access.last_return);
				// g_access.last_return = ft_itoa(1);
				ft_set_lasts(args, pid, 1, 2);
				break;
				j++;
			}
		}
		if (!ft_strncmp(args[i], "PWD", ft_strlen(args[i])))
			ft_free_secure((void *)&g_access.pwd);
		if (ft_strlen(args[i]) == ft_strlen(((t_env_var*)(ptr->content))->name) - 1 && valid)
		{
			if (!ft_strncmp(args[i], ((t_env_var*)(ptr->content))->name, ft_strlen(args[i])))
			{
				if (ft_strncmp(args[i], "_", 1))
				{
					g_access.env = ptr->next;
					ft_lstdelone(ptr, delone);
				}
				else
					ft_set_lasts(args, pid, 0, 1);
			}
		}
		while(ptr->next != NULL && valid)
		{
			if (ft_strlen(args[i]) == ft_strlen(((t_env_var*)(ptr->next->content))->name) - 1)
			{
				if (!ft_strncmp(args[i], ((t_env_var*)(ptr->next->content))->name, ft_strlen(args[i])))
				{
					if (ptr->next->next != NULL)
					{
						if (ft_strncmp(args[i], "_", 1))
						{
							temp = ptr->next;
							ptr->next = ptr->next->next;
							ft_lstdelone(temp, delone);
						}
						else
							// ft_last_arg(args, pid);
							ft_set_lasts(args, pid, 0, 1);
						break;
					}
					else
					{
						if (ft_strncmp(args[i], "_", 1))
						{
							ft_lstdelone(ptr->next, delone);
							ptr->next = NULL;
						}
						else
							// ft_last_arg(args, pid);
							ft_set_lasts(args, pid, 0, 1);
						break;
					}
				}
			}
			ptr = ptr->next;
		}
		i++;
	}
	return (1);
}

