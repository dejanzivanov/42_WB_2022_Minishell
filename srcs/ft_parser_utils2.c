/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parser_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzivanov <dzivanov@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/24 17:22:36 by dzivanov          #+#    #+#             */
/*   Updated: 2022/02/24 17:26:39 by dzivanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

void	ft_str_ha(t_list *lex_element, char	***cmd_line, int *cmd_len)
{
	char	*str;

	if (ft_strlen(((t_word *)(lex_element->content))->address) != 0)
	{
		str = ft_strdup(((t_word *)(lex_element->content))->address);
		*cmd_line = add_to_line(*cmd_line, str, cmd_len);
	}
}

/*
**  @brief Chechk if string is same as one of redirects
*/

int	is_redirect(char *str)
{
	if (!ft_strcmp(str, ">") || !ft_strcmp(str, "<") || \
	!ft_strcmp(str, ">>") || !ft_strcmp(str, "<<"))
		return (1);
	return (0);
}

/*
**  @brief Chechk if string is same as pip
*/

int	is_pipe(char *str)
{
	if (!ft_strcmp(str, "|"))
		return (1);
	return (0);
}

int	ft_setup_parser(t_parser **parser)
{
	*parser = ft_calloc(sizeof(t_parser), 1);
	(*parser)->index_counter = 0;
	(*parser)->lex_element = g_access.lexor2parser;
	(*parser)->executor_element = NULL;
	(*parser)->cmd_line = NULL;
	(*parser)->cmd_line_red = NULL;
	(*parser)->cmd_len = 0;
	(*parser)->cmd = NULL;
	(*parser)->return_flag = 0;
	return (0);
}

void	ft_add_redirect_command(t_parser **parser)
{
	(*parser)->cmd_line_red[1] = ft_strdup(((t_word *)((*parser)-> \
	lex_element->content))->address);
	(*parser)->cmd = (t_command *)malloc(sizeof(t_command));
	(*parser)->cmd->comm_table = (*parser)->cmd_line_red;
	(*parser)->cmd->path = NULL;
	(*parser)->cmd->index = (*parser)->index_counter;
	(*parser)->cmd->comm_len = 3;
	(*parser)->cmd->cmd_type = FT_CMD_TYPE_REDIRECT;
	(*parser)->executor_element = ft_lstnew((void *)(*parser)->cmd);
	ft_lstadd_back(&(g_access.parser2exec), (*parser)->executor_element);
}
