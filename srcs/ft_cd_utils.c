#include "../incl/minishell.h"

/**
	 @brief Bultin command support function: updates PWD
	 @param path The path towards the directory, as imputted by user
	 @return void functions;
**/

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
		arg1 = env_value_finder("PWD");
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

static char *	ft_abs_path_creator(char *rel_path)
{
	char *res;
	char *buf;

	buf = NULL;
	if (rel_path[0] == '/')
	{
		res = ft_strdup(rel_path);
		return (res);
	}
	if (g_access.dp == NULL)
	{
		ft_set_global_pwd(&buf);
		res = ft_strjoin_with_free(ft_strjoin_with_free(buf, "/"), rel_path);
	}
	else
		res = ft_strjoin_with_free(ft_strjoin(g_access.dp, "/"), rel_path);
	return (res);
}

static char *	ft_path_helper_creator()
{
	char *res;

	res = NULL;
	if (g_access.dp != NULL)
		res = ft_strdup(g_access.dp);
	else if (g_access.pwd != NULL)
		res = ft_strdup(g_access.pwd);
	else
		ft_set_global_pwd(&res);
	return (res);
}

static int ft_engine_path(char **path_helper, char **split_ptr)
{
	char *path_ptr;
	char *path_helper_free;

	if(!ft_strncmp(*split_ptr, "..", 2) && ft_strlen(*split_ptr) == 2)
	{
		path_ptr = ft_strrchr(*path_helper, '/');
		if(path_ptr != NULL)
		{
			path_helper_free = *path_helper;
			*path_helper = ft_substr(*path_helper, 0, ft_strlen(*path_helper) - ft_strlen(path_ptr));
			if (ft_strlen(*path_helper) == 0)
			{
				*path_helper = ft_strjoin_with_free(*path_helper, "/");
				ft_free_secure((void *) &path_helper_free);
				// break;
				return (-1);
			}
			if (path_helper_free)
				ft_free_secure((void *) &path_helper_free);
		}
	}
	else if (!ft_strncmp(*split_ptr, ".", 1) && ft_strlen(*split_ptr) == 1)
	{
		// split_ptr++;
		// continue;
		return (1);
	}
	else
		*path_helper = ft_strjoin_with_dfree(*path_helper, ft_strjoin("/", *split_ptr));
	return (0);
	// split_ptr++;
	// return (0);
}

void ft_rtoa_path(char *rel_path, char **abs_path)
{
	char *path_helper;
	char **arg_split;
	char **split_ptr;
	int var;
	// char *path_ptr;
	// char *path_helper_free;

	if (ft_strnstr(rel_path, ".", ft_strlen(rel_path)) == NULL)
	{
		*abs_path = ft_abs_path_creator(rel_path);
		return ;
	}
	path_helper = ft_path_helper_creator();
	arg_split = ft_split(rel_path, '/');
	split_ptr = arg_split;
	while(*split_ptr != NULL)
	{
		var = ft_engine_path(&path_helper, split_ptr);
		if (var == -1)
			break ;
		// else if (var == 0)
			// path_helper = ft_strjoin_with_dfree(path_helper, ft_strjoin("/", *split_ptr));
		else
		{
			split_ptr++;
			// continue ;
		}
		// printf("PATH HELPER = %s\n", path_helper);
		// split_ptr++;
		// if(!ft_strncmp(*split_ptr, "..", 2) && ft_strlen(*split_ptr) == 2)
		// {
		// 	path_ptr = ft_strrchr(path_helper, '/');
		// 	if(path_ptr != NULL)
		// 	{
		// 		path_helper_free = path_helper;
		// 		path_helper = ft_substr(path_helper, 0, ft_strlen(path_helper) - ft_strlen(path_ptr));
		// 		if (ft_strlen(path_helper) == 0)
		// 		{
		// 			path_helper = ft_strjoin_with_free(path_helper, "/");
		// 			ft_free_secure((void *) &path_helper_free);
		// 			break;
		// 		}
		// 		if (path_helper_free)
		// 			ft_free_secure((void *) &path_helper_free);
		// 	}
		// }
		// else if (!ft_strncmp(*split_ptr, ".", 1) && ft_strlen(*split_ptr) == 1)
		// {
		// 	split_ptr++;
		// 	continue;
		// }
		// else
		// 	path_helper = ft_strjoin_with_dfree(path_helper, ft_strjoin("/", *split_ptr));
		// split_ptr++;
	}
	ft_free_split(arg_split);
	*abs_path = path_helper;
}
