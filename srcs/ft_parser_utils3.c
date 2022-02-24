/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parser_utils3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzivanov <dzivanov@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/24 17:22:36 by dzivanov          #+#    #+#             */
/*   Updated: 2022/02/24 17:31:26 by dzivanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

void	ft_add_command(t_parser **parser)
{
	(*parser)->cmd = (t_command *)malloc(sizeof(t_command));
	(*parser)->cmd->comm_table = (*parser)->cmd_line;
	(*parser)->cmd->path = NULL;
	(*parser)->cmd->index = (*parser)->index_counter;
	(*parser)->cmd->comm_len = (*parser)->cmd_len;
	(*parser)->cmd->cmd_type = 0;
	ft_command_check((*parser)->cmd->comm_table[0], \
	&(*parser)->cmd->path, &(*parser)->cmd->cmd_type);
	(*parser)->executor_element = ft_lstnew((void *)(*parser)->cmd);
	ft_lstadd_back(&(g_access.parser2exec), (*parser)->executor_element);
}

int	ft_parser_redirect_error_check(t_parser **parser)
{
	char	*str;

	str = "minshe11: syntax error near unexpected token `newline'1\n";
	if ((*parser)->lex_element == NULL)
	{
		(*parser)->return_flag = 2;
		if (g_access.last_return != NULL)
			free(g_access.last_return);
		g_access.last_return = ft_itoa(2);
		write(2, str, ft_strlen(str));
		if ((*parser)->cmd_line_red[0] != NULL)
		{
			free((*parser)->cmd_line_red[0]);
			(*parser)->cmd_line_red[0] = NULL;
		}
		free((*parser)->cmd_line_red);
		error_fun(&(g_access.parser2exec), &(g_access.lexor2parser));
		return (2);
	}
	return (0);
}

int	ft_parser_last_redirect_element(t_parser **parser)
{
	char	*str;

	str = "minishe11: syntax error near unexpected token'2\n";
	if (((t_word *)((*parser)-> \
	lex_element->content))->type == FT_SPECIAL_CHAR_STRING)
	{
		(*parser)->return_flag = 2;
		if (g_access.last_return != NULL)
			free(g_access.last_return);
		g_access.last_return = ft_itoa(2);
		write(2, str, ft_strlen(str));
		if ((*parser)->cmd_line_red[0] != NULL)
		{
			free((*parser)->cmd_line_red[0]);
			(*parser)->cmd_line_red[0] = NULL;
		}
		free((*parser)->cmd_line_red);
		error_fun(&(g_access.parser2exec), &(g_access.lexor2parser));
		return (2);
	}
	return (0);
}

int	ft_parser_redirect_handler(t_parser **parser)
{
	if (is_redirect(((t_word *)((*parser)->lex_element->content))->address))
	{
		(*parser)->cmd_line_red = (char **)ft_calloc(3, sizeof(char *));
		(*parser)->cmd_line_red[0] = ft_strdup(((t_word *) \
		((*parser)->lex_element->content))->address);
		(*parser)->lex_element = (*parser)->lex_element->next;
		if (ft_parser_redirect_error_check(parser) == 2)
			return (2);
		if (ft_parser_last_redirect_element(parser) == 2)
			return (2);
		ft_add_redirect_command(parser);
	}
	return (0);
}

int	ft_pipe_handler(t_parser **parser)
{
	char	*str;
	char	*str1;

	str = "minishe11: syntax error near unexpected token `|'3\n";
	str1 = "minishe11: syntax error near unexpected token `|'4\n";
	if ((*parser)->cmd_len == 0)
	{
		(*parser)->return_flag = 2;
		ft_set_lasts(NULL, 0, 2, 2);
		write(2, str, ft_strlen(str));
		error_fun(&(g_access.parser2exec), &(g_access.lexor2parser));
	}
	else
	{
		(*parser)->lex_element = (*parser)->lex_element->next;
		if ((*parser)->lex_element == NULL)
		{
			(*parser)->return_flag = 2;
			ft_set_lasts(NULL, 0, 2, 2);
			write(2, str1, ft_strlen(str1));
			error_fun(&(g_access.parser2exec), &(g_access.lexor2parser));
		}
	}
	return (0);
}
