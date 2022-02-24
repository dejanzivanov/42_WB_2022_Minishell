#include "../incl/minishell.h"

static void	ft_exec_set(char **exec_pwd, char *executable)
{
	*exec_pwd = ft_calloc(ft_strlen(executable)-ft_strlen("/minishell") + 1, sizeof(char));
	ft_strlcpy(*exec_pwd,executable, ft_strlen(executable) - ft_strlen("/minishell") + 1);
	chdir(*exec_pwd);
}

static int ft_simlink_handler(struct stat **buf, char **path_substr, \
	char **path_substr_free, char **str_ptr)
{
	if (S_ISLNK((*buf)->st_mode))
	{
		ft_free_secure((void *)buf);
		ft_free_secure((void *)path_substr);
		ft_free_secure((void *)path_substr_free);
		return(1);
	}
	else
	{
		ft_free_secure((void *)buf);
		*str_ptr = NULL;
		*str_ptr = ft_strrchr(*path_substr, '/');
		if (*str_ptr != NULL)
		{
			*path_substr_free = *path_substr;
			*path_substr = ft_substr(*path_substr, 0, ft_strlen(*path_substr) - ft_strlen(*str_ptr));
			ft_free_secure((void *)path_substr_free);
		}
		else
			ft_free_secure((void *)path_substr);
	}
	return (0);
}

void	ft_update_shell_env(char *executable)
{
	char *current_pwd;
	char *exec_pwd;
	t_list *ptr;

	ptr = g_access.env;
	current_pwd = NULL;
	exec_pwd = NULL;
	while (ptr)
	{
		if (!ft_strncmp(((t_env_var*)(ptr->content))->name, "SHELL=", 6))
		{
			ft_set_global_pwd(&current_pwd);
			ft_exec_set(&exec_pwd, executable);
			ft_free_secure((void *)&exec_pwd);
			ft_set_global_pwd(&exec_pwd);
			chdir(current_pwd);
			ft_free_secure((void *)&((t_env_var*)(ptr->content))->value);
			((t_env_var*)(ptr->content))->value = ft_strjoin(exec_pwd, "/minishell");
			ft_free_secure((void *)&current_pwd);
			ft_free_secure((void *)&exec_pwd);
			break;
		}
		ptr = ptr->next;
	}
}

static void	ft_symlink_print(char *arg, int pid)
{
	if (pid == 0)
	{
		write(2, "minishell: cd: ", 15);
		write(2, arg, ft_strlen(arg));
		perror(" ");
	}
}

int ft_check_symlink(char *path, char *arg, pid_t pid)
{
	struct stat *buf;
	char *path_substr;
	char *str_ptr;
	char *path_substr_free;

	str_ptr = NULL;
	path_substr = ft_strdup(path);
	path_substr_free = NULL;
	while (path_substr != NULL)
	{
		buf = ft_calloc(sizeof(struct stat), 1);
		if (ft_strlen(path_substr) != 0)
		{
			if (lstat(path_substr, buf) == -1)
			{
				ft_symlink_print(arg, pid);
				ft_symlink_initiate_free(&buf, &path_substr, &path_substr_free, 1);
				return (-1);
			}
		}
		if (ft_simlink_handler(&buf, &path_substr, &path_substr_free, &str_ptr) == 1)
			return (SYMLINK);
	}
	ft_symlink_initiate_free(&buf, &path_substr, NULL, 2);
	return (NOT_SYMLINK);
}