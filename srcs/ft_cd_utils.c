#include "../incl/minishell.h"

static char	*ft_abs_path_creator(char *rel_path)
{
	char	*res;
	char	*buf;

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

static char	*ft_path_helper_creator(void)
{
	char	*res;

	res = NULL;
	if (g_access.dp != NULL)
		res = ft_strdup(g_access.dp);
	else if (g_access.pwd != NULL)
		res = ft_strdup(g_access.pwd);
	else
		ft_set_global_pwd(&res);
	return (res);
}

static int	ft_engine_path_helper(char **path_helper, char **path_helper_free)
{
	*path_helper = ft_strjoin_with_free(*path_helper, "/");
	ft_free_secure((void *) path_helper_free);
	return (-1);
}

static int	ft_engine_path(char **path_helper, char **split_ptr)
{
	char	*path_ptr;
	char	*path_helper_free;

	if (!ft_strncmp(*split_ptr, "..", 2) && ft_strlen(*split_ptr) == 2)
	{
		path_ptr = ft_strrchr(*path_helper, '/');
		if (path_ptr != NULL)
		{
			path_helper_free = *path_helper;
			*path_helper = ft_substr(*path_helper, 0, \
				ft_strlen(*path_helper) - ft_strlen(path_ptr));
			if (ft_strlen(*path_helper) == 0)
				return (ft_engine_path_helper(path_helper, &path_helper_free));
			if (path_helper_free)
				ft_free_secure((void *) &path_helper_free);
		}
	}
	else if (!ft_strncmp(*split_ptr, ".", 1) && ft_strlen(*split_ptr) == 1)
		return (1);
	else
		*path_helper = \
			ft_strjoin_with_dfree(*path_helper, ft_strjoin("/", *split_ptr));
	return (0);
}

void	ft_rtoa_path(char *rel_path, char **abs_path)
{
	char	*path_helper;
	char	**arg_split;
	char	**split_ptr;
	int		var;

	if (ft_strnstr(rel_path, ".", ft_strlen(rel_path)) == NULL)
	{
		*abs_path = ft_abs_path_creator(rel_path);
		return ;
	}
	path_helper = ft_path_helper_creator();
	arg_split = ft_split(rel_path, '/');
	split_ptr = arg_split;
	while (*split_ptr != NULL)
	{
		var = ft_engine_path(&path_helper, split_ptr);
		if (var == -1)
			break ;
		else
			split_ptr++;
	}
	ft_free_split(arg_split);
	*abs_path = path_helper;
}
