/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parser.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzivanov <dzivanov@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/24 17:16:14 by dzivanov          #+#    #+#             */
/*   Updated: 2022/02/24 17:35:32 by dzivanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

void	ft_move_elements(t_parser **parser)
{
	(*parser)->cmd_line_red = (char **)ft_calloc(3, sizeof(char *));
	(*parser)->cmd_line_red[0] = \
	ft_strdup(((t_word *)((*parser)->lex_element->content))->address);
	(*parser)->lex_element = (*parser)->lex_element->next;
}

int	ft_parser_itterator(t_parser **pars)
{
	if (((t_word *)((*pars)->lex_element->content))->type == 1)
	{
		if (is_redirect(((t_word *)((*pars)->lex_element->content))->address))
		{
			ft_move_elements(pars);
			if (ft_parser_redirect_error_check(pars) == 2)
				return (2);
			if (ft_parser_last_redirect_element(pars) == 2)
				return (2);
			ft_add_redirect_command(pars);
		}
		else if (is_pipe(((t_word *)((*pars)->lex_element->content))->address))
		{
			ft_pipe_handler(pars);
			return (2);
		}
		else
		{
			ft_parser_error_handler(pars);
			return (2);
		}
	}
	else if (((t_word *)((*pars)->lex_element->content))->type == FT_STRING)
		ft_str_ha((*pars)->lex_element, &(*pars)->cmd_line, &(*pars)->cmd_len);
	return (0);
}

void	ft_parser_main_loop(t_parser *parser)
{
	while (parser->return_flag == 0)
	{
		if (parser->lex_element == NULL)
			break ;
		if (ft_parser_itterator(&parser) == 2)
			break ;
		if (ft_cmd_limit_handler(&parser) == 2)
			break ;
		parser->lex_element = parser->lex_element->next;
	}
	parser->cmd_line = add_to_line(parser->cmd_line, NULL, &parser->cmd_len);
	if (parser->return_flag == 0)
	{
		if (parser->cmd_len > 1)
			ft_add_command(&parser);
		else
			ft_free_split(parser->cmd_line);
		parser->index_counter++;
		ft_pipe_limit_handler(&parser);
	}
	else
		ft_free_split(parser->cmd_line);
}

int	parser(void)
{
	t_parser	*parser;
	int			ret_val;

	ret_val = ft_setup_parser(&parser);
	while (parser->return_flag == 0)
	{
		parser->cmd_line = 0;
		parser->cmd_len = 0;
		if (parser->lex_element == NULL || parser->return_flag != 0)
			break ;
		ft_parser_main_loop(parser);
	}
	ret_val = ft_free_parse_struct(parser);
	return (ret_val);
}
