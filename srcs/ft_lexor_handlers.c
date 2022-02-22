/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lexor_handlers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzivanov <dzivanov@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/21 23:16:31 by dzivanov          #+#    #+#             */
/*   Updated: 2022/02/21 23:17:58 by dzivanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

/*
**	@brief Function used for handling space cgaracters from standard input
**	@param char **current_str, char *args, int i, int begining.
**	@return No Return Value
**	@todo /
*/

void	ft_lex_space_handler(char **current_str, char *args, int begining, \
int i)
{
	*current_str = join2current_str(*current_str, \
	ft_substr(args, begining, i - begining));
	if (ft_strlen(*current_str))
		add_string(&(g_access.lexor2parser), *current_str);
	free(*current_str);
	*current_str = NULL;
	free(*current_str);
	*current_str = NULL;
}

/*
**	@brief Function used for handling operands like pipe, less or greater than,
**	@param char **current_str, char *args, int i, int begining.
**	@return No Return Value
**	@todo /
*/

void	ft_lex_operand_handler(char **current_str, char *args, int begining, \
int i)
{
	*current_str = join2current_str(*current_str, \
	ft_substr(args, begining, i - begining));
	if (ft_strlen(*current_str))
		add_specialchar_string(&(g_access.lexor2parser), *current_str);
	free(*current_str);
	*current_str = NULL;
	free(*current_str);
	*current_str = NULL;
}

/*
**	@brief Function used for handling single quotes,
**	@param char **current_str, char *args, int i, int begining.
**	@return Return value is integer
**	@todo /
*/

int	ft_lex_single_quote_handler(char **current_str, char *args, int begining, \
int i)
{
	int	flag;

	flag = 0;
	*current_str = join2current_str(*current_str, ft_substr(args, begining, \
	i - begining));
	flag = q_handler(&(args[i + 1]), current_str, FT_SINGLE_QUOTE);
	if (flag == -1)
	{
		free(*current_str);
		*current_str = NULL;
		printf("minishe11: Unfinished single quote\n");
		if (g_access.last_return != NULL)
			free(g_access.last_return);
		g_access.last_return = ft_itoa(3);
		return (flag);
	}
	return (flag);
}

/*
**	@brief Function used for handling double quotes,
**	@param char **current_str, char *args, int i, int begining.
**	@return Return value is integer
**	@todo /
*/

int	ft_lex_double_quote_handler(char **current_str, char *args, int begining, \
int i)
{
	int	flag;

	flag = 0;
	*current_str = join2current_str(*current_str, ft_substr(args, begining, \
	i - begining));
	flag = q_handler(&(args[i + 1]), current_str, FT_DOUBLE_QUOTE);
	if (flag == -1)
	{
		free(*current_str);
		*current_str = NULL;
		printf("minishe11: Unfinished double quote\n");
		if (g_access.last_return != NULL)
			free(g_access.last_return);
		g_access.last_return = ft_itoa(3);
		return (flag);
	}
	return (flag);
}

/*
**	@brief Function used for handling rest of the string at the end of the input.,
**	@param char **current_str, char *args, int i, int begining.
**	@return No Return Value
**	@todo /
*/

void	ft_lex_string_reminder_handler(char **current_str, char *args, \
int begining, int i)
{
	*current_str = join2current_str(*current_str, ft_substr(args, begining, \
	i - begining));
	if (ft_strlen(*current_str))
	{
		if (is_special_char(args[begining]))
			add_specialchar_string(&(g_access.lexor2parser), *current_str);
		else
			add_string(&(g_access.lexor2parser), *current_str);
	}
	free(*current_str);
	*current_str = NULL;
}
