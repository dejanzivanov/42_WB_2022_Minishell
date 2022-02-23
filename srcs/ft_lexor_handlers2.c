/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lexor_handlers2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzivanov <dzivanov@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/22 00:44:13 by dzivanov          #+#    #+#             */
/*   Updated: 2022/02/22 18:58:57 by dzivanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

void	ft_setup_lexor(t_lexor **lex)
{
	*lex = ft_calloc(sizeof(t_lexor), 1);
	(*lex)->i = 0;
	(*lex)->begining = 0;
	(*lex)->last = FT_SPACE;
	(*lex)->flag = 0;
}

void	ft_lex_handler(t_lexor **lex, char **current_str, char *args)
{
	if ((is_space((*lex)->last) && !is_space(args[(*lex)->i])))
	{
		(*lex)->begining = (*lex)->i;
	}
	else if (!is_special_char((*lex)->last) && is_special_char(args[(*lex)->i]))
	{
		ft_lex_space_handler(current_str, args, (*lex)->begining, (*lex)->i);
		(*lex)->begining = (*lex)->i;
	}
	else if (is_special_char((*lex)->last) && !is_special_char(args[(*lex)->i]))
	{
		ft_lex_operand_handler(current_str, args, (*lex)->begining, (*lex)->i);
		(*lex)->begining = (*lex)->i;
	}
	else if ((!is_space((*lex)->last) && is_space(args[(*lex)->i])))
	{
		ft_lex_space_handler(current_str, args, (*lex)->begining, (*lex)->i);
		(*lex)->begining = (*lex)->i;
	}
}

void	ft_lex_quote_handler(t_lexor **lex, char **current_str, char *args)
{
	if (args[(*lex)->i] == FT_SINGLE_QUOTE)
	{
		(*lex)->flag = ft_lex_single_quote_handler(current_str, args, \
		(*lex)->begining, (*lex)->i);
		if ((*lex)->flag == -1)
			return ;
		(*lex)->i = (*lex)->i + (*lex)->flag + 1;
		(*lex)->begining = (*lex)->i + 1;
	}
	if (args[(*lex)->i] == FT_DOUBLE_QUOTE)
	{
		(*lex)->flag = ft_lex_double_quote_handler(current_str, args, \
		(*lex)->begining, (*lex)->i);
		if ((*lex)->flag == -1)
			return ;
		(*lex)->i = (*lex)->i + (*lex)->flag + 1;
		(*lex)->begining = (*lex)->i + 1;
	}
	(*lex)->last = args[(*lex)->i];
}

void	ft_lex_exit(t_lexor **lex, char **current_str, char *args)
{
	if ((*lex)->flag != -1)
	{
		if (!is_space(args[(*lex)->i - 1]) && (*lex)->i > 0)
		{
			ft_lex_string_reminder_handler(current_str, args, \
			(*lex)->begining, (*lex)->i);
		}
	}
	if ((*lex)->flag == -1)
	{
		ft_free_list(g_access.lexor2parser);
		g_access.lexor2parser = NULL;
	}
	free(args);
	args = NULL;
	g_access.read_line2lexor = NULL;
	if (ft_lstsize(g_access.lexor2parser) == 0 && (*lex)->flag == 0)
	{
		ft_free_list(g_access.lexor2parser);
		g_access.lexor2parser = NULL;
		(*lex)->flag = -1;
	}
}
