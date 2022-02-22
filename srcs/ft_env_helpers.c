/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzivanov <dzivanov@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/22 02:17:49 by dzivanov          #+#    #+#             */
/*   Updated: 2022/02/22 02:18:53 by dzivanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

void	ft_env_quote_handler(char **args, t_env **env)
{
	if ((*args)[(*env)->i] == FT_SINGLE_QUOTE)
		if ((*env)->d_quote_flag % 2 == 0)
			(*env)->s_quote_flag++;
	if ((*args)[(*env)->i] == FT_DOUBLE_QUOTE)
		if ((*env)->s_quote_flag % 2 == 0)
			(*env)->d_quote_flag++;
}

void	ft_env_string_handler(char **args, t_env **env)
{
	(*env)->temp1 = ft_substr(*args, (*env)->i + 1, (*env)->j - (*env)->i - 1);
	(*env)->temp0 = ft_getenv((*env)->temp1);
	free((*env)->temp1);
	(*env)->temp0 = env_var_formater((*env)->temp0);
	(*args)[(*env)->i] = '\0';
	(*env)->temp1 = ft_strjoin(*args, (*env)->temp0);
	free((*env)->temp0);
	(*env)->temp0 = ft_strjoin((*env)->temp1, &((*args)[(*env)->j]));
	(*env)->i = ft_strlen((*env)->temp1);
	free(*args);
	*args = (*env)->temp0;
	free((*env)->temp1);
	(*env)->i--;
}

int	ft_env_char_check(char **args, t_env **env)
{
	if ((*args)[(*env)->j] == '*' || (*args)[(*env)->j] == '@' || \
		(*args)[(*env)->j] == '#' || (*args)[(*env)->j] == '?' || \
		(*args)[(*env)->j] == '-' || (*args)[(*env)->j] == '$' || \
		(*args)[(*env)->j] == '!')
	{
		return (1);
	}
	return (0);
}

int	ft_env_dollar_handler(char **args, t_env **env)
{
	if ((*args)[(*env)->i] == FT_DOLLAR_SIGN)
	{
		if ((*env)->s_quote_flag % 2 == 0)
		{
			(*env)->j = (*env)->i + 1;
			while (ft_isalnum((*args)[(*env)->j]) || (*args)[(*env)->j] == '_')
			{
				(*env)->j++;
				if (ft_isdigit((*args)[(*env)->j - 1]))
					break ;
			}
			if ((*env)->j == (*env)->i + 1)
			{
				if (ft_env_char_check(args, env) == 1)
					(*env)->j++;
				else
				{
					(*env)->i++;
					return (2);
				}
			}
			ft_env_string_handler(args, env);
		}
	}
	return (0);
}

void	ft_env_check(char **args)
{
	t_env	*env;

	ft_setup_env_check(&env);
	while ((*args)[env->i])
	{
		ft_env_quote_handler(args, &env);
		if (ft_env_dollar_handler(args, &env) == 2)
			continue ;
		env->i++;
	}
	if (env)
		free(env);
}
