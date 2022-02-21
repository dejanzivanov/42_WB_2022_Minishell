#include "../incl/minishell.h"

int	is_space(char ch)
{
	if (ch == FT_SPACE)
		return (1);
	return (0);
}

int	is_special_char(char ch)
{
	if (ch == FT_PIPE || ch == FT_GREATER || ch == FT_LESSER)
		return (1);
	return (0);
}

char	*ft_env_helper(t_env_var *temp_env)
{
	if (!(ft_strncmp(temp_env->name, "PWD=", ft_strlen(temp_env->name))) \
	&& g_access.dp != NULL)
	{
		if (*(temp_env->value) != '\0')
			return (g_access.dp);
	}
	else
		return (temp_env->value);
	return ("");
}

char	*ft_env_path(void)
{
	if (g_access.dp != NULL)
		return (g_access.dp);
	else
		return (g_access.pwd);
	return ("");
}

void	ft_comment_helper(int i, char **args)
{
	char	*temp;

	temp = ft_substr(*args, 0, i);
	free(*args);
	*args = temp;
}
