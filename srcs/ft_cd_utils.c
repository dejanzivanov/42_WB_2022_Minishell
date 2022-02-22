#include "../incl/minishell.h"

void ft_cd_simlink(char *abs_path, char *current_path, int pid, int mode)
{
	if (env_value_finder("PWD") == NULL || ft_strlen(env_value_finder("PWD")) == 0)
		ft_update_create_env("OLDPWD", "", pid);
	else
		ft_update_create_env("OLDPWD", current_path, pid);
	chdir(abs_path);
	if (mode == 1)
	{
		if (g_access.dp != NULL)
			free(g_access.dp);
		g_access.dp = ft_strdup(abs_path);
	}
	else if (mode == 2)
	{
		if (g_access.dp != NULL)
		{
			free(g_access.dp);
			g_access.dp = NULL;
		}
	}
	if (g_access.pwd != NULL)
		free(g_access.pwd);
	g_access.pwd = ft_strdup(abs_path);
	ft_update_env("PWD", abs_path);
}

/**
	 @brief Bultin command support function: updates PWD
	 @param path The path towards the directory, as imputted by user
	 @return void functions;
	 @todo add "//" situation, where the chdir takes us to "/", but pwd should be updated as "//". Any other number of slashes must
	 		result in a single "/"
	 @todo modify PATH as imputted by user; example: we have Minishell; user imputs "minishell"; chdir works and path is 
	 		'....user/minishell' instead of '......user/Minishell'
	 @todo if we use cd "/etc" (or any variant of any below-mounting-point value), 
	 	instead of updating PWD to private/etc, we must update it to /etc (or any below mounting-point folder used)
 */
void	ft_update_PWD(void)
{
	char *path;

	path = NULL;
	ft_set_global_pwd(&path);
	if (env_value_finder("PWD") == NULL)
		ft_set_global_pwd(&g_access.pwd);
	else
		ft_update_env("PWD=", path);
	if (path != NULL)
		free(path);
}

char	*ft_handle_cd(char *address, t_list *ptr, pid_t pid)
{
	if (address == NULL)
		return (env_value_finder("HOME"));
	else if ((!ft_strncmp(address, "~", 1) && ft_strlen(address) < 2)|| (!ft_strncmp(address, "--", 2) && ft_strlen(address) < 3))
	{
		if (env_value_finder("HOME") == NULL && (!ft_strncmp(address, "~", 1) && ft_strlen(address) < 2))
			return(g_access.home);
		else
			return (env_value_finder("HOME"));
	}
	else if (!ft_strncmp(address, "-", 1) && ft_strlen(address) < 2)
	{
		while (ptr != NULL)
		{
			if (ft_strncmp(((t_env_var *)(ptr->content))->name, "OLDPWD", 6) == 0) //IF OLDPWD EXISTS, WE RETURN env_value_finder("OLDPWD") <------
			{
				if (pid == 0)
				{
					write(1, env_value_finder("OLDPWD"), ft_strlen(env_value_finder("OLDPWD")));
					write(1, "\n", 1);
				}
				return (env_value_finder("OLDPWD"));
			}
			ptr = ptr->next;
		}
		return (";");
	}
	if (!ft_strncmp(address, "---", 3))
	{
		return ("L");
	}
	else if (!ft_strncmp(address, "---", 3) && ft_strlen(address) > 3)
	{
		return ("L");
	}
	return (address);
}

int ft_cd_error_handler(char *str, pid_t pid, char **path, char **temp)
{
	if (pid == 0)
		write(2, str, ft_strlen(str));
	if (*path != NULL)
	{
		free(*path);
		*path = NULL;
	}
	free(temp[0]);
	free(temp[1]);
	free(g_access.last_return);
	g_access.last_return = ft_itoa(1);
	return (1);
}

void ft_update_dir(char *arg1, char *path)
{
	struct stat *buf;
	char *symlink;
	char *symlink_dir;

	if (arg1 == NULL)
		return;
	if (!ft_strncmp(arg1, "-", ft_strlen(arg1)))
		arg1 = env_value_finder("PWD");	//needs to be adjusted for symlink!
	buf = ft_calloc(sizeof(struct stat), 1);
	symlink = ft_strrchr(arg1, '/');
	if (lstat(arg1, buf) == -1)
		perror("minishell");
	if (S_ISLNK(buf->st_mode))
	{
		if (symlink == NULL)
		{
			ft_set_global_pwd(&g_access.dp);
			g_access.dp = ft_strjoin(g_access.dp,ft_strjoin("/", arg1));
		}
		else
		{
			symlink_dir = ft_substr(arg1, 0, symlink - arg1);
			chdir(symlink_dir);
			ft_set_global_pwd(&symlink_dir);
			free(g_access.dp);
			g_access.dp = ft_strjoin(symlink_dir, symlink);
		}
	}
	else
	{
		if(g_access.dp != NULL)
			free(g_access.dp);
		g_access.dp = NULL;
	}
	if (chdir(path) != 0)
	{
		free(g_access.last_return);
		g_access.last_return = ft_itoa(1);
	}
	free(buf);
}

/**************************************************/

void ft_rtoa_path(char *rel_path, char **abs_path)
{
	char *path_helper;
	char **arg_split;
	char **split_ptr;
	char *path_ptr;
	char *path_helper_free;
	char *buf;
	int i = 0;

	buf = NULL;
	if (ft_strnstr(rel_path, ".", ft_strlen(rel_path)) == NULL)
	{
		if (rel_path[0] == '/')
		{
			*abs_path = ft_strdup(rel_path);
			return;
		}
		if (g_access.dp == NULL)
		{
			buf = getcwd(NULL, 0);
			while(getcwd(buf, i) == NULL)
				i++;
			*abs_path = ft_strjoin_with_free(ft_strjoin_with_free(buf, "/"), rel_path);
		}
		else
			*abs_path = ft_strjoin_with_free(ft_strjoin(g_access.dp, "/"), rel_path);
		return;
	}
	path_helper = NULL;
	if (g_access.dp != NULL)
		path_helper = ft_strdup(g_access.dp);
	else if (g_access.pwd != NULL)
		path_helper = ft_strdup(g_access.pwd);
	else
		ft_set_global_pwd(&path_helper);
	arg_split = ft_split(rel_path, '/');
	split_ptr = arg_split;
	while(*split_ptr != NULL)
	{
		if(!ft_strncmp(*split_ptr, "..", 2) && ft_strlen(*split_ptr) == 2)
		{
			path_ptr = ft_strrchr(path_helper, '/');
			if(path_ptr != NULL)
			{
				path_helper_free = path_helper;
				path_helper = ft_substr(path_helper, 0, ft_strlen(path_helper) - ft_strlen(path_ptr));
				if (ft_strlen(path_helper) == 0)
				{
					path_helper = ft_strjoin_with_free(path_helper, "/");
					if(path_helper_free)
					{
						free(path_helper_free);
						path_helper_free = NULL;
					}
					break;
				}
				if (path_helper_free)
				{
					free(path_helper_free);
					path_helper_free = NULL;
				}

			}
		}
		else if (!ft_strncmp(*split_ptr, ".", 1) && ft_strlen(*split_ptr) == 1)
		{
			split_ptr++;
			continue;
		}
		else
		{
			path_helper = ft_strjoin_with_dfree(path_helper, ft_strjoin("/", *split_ptr));
		}
		split_ptr++;
	}
	ft_free_split(arg_split);
	*abs_path = path_helper;
}
