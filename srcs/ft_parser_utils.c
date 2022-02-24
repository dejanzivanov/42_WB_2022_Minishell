#include "../incl/minishell.h"

/*
**  @brief Free linked list of with t_command in content
**  @param input: t_list pointer to first element in linked list
*/

void	ft_free_parser(void *parser)
{
	int i;
	t_command *cmd;

	i = 0;
	cmd = (t_command *)parser;
    while (i < cmd->comm_len)
    {
        free(cmd->comm_table[i]);
        i++;
    }
	if (cmd->path != NULL)
		free(cmd->path);
	free(cmd->comm_table);
	free(parser);

}

int	ft_free_parse_struct(t_parser *parser)
{
	int	ret_value;

	ret_value = parser->return_flag;
	if (parser->return_flag == 0)
	{
		ft_free_lex_list(g_access.lexor2parser);
		if (parser->lex_element)
		{
			free(parser->lex_element);
			parser->lex_element = NULL;
		}
		g_access.lexor2parser = NULL;
	}
	if (parser != NULL)
		free(parser);
	parser = NULL;
	return (ret_value);
}

void	ft_parser_error_handler(t_parser **parser)
{
	(*parser)->return_flag = 2;
	ft_set_lasts(NULL, 0, 2, 2);
	write(2, "minishe11: syntax error near unexpected token'5\n", 49);
	error_fun(&(g_access.parser2exec), &(g_access.lexor2parser));
}

int	ft_cmd_limit_handler(t_parser **parser)
{
	if ((*parser)->cmd_len > PARSER_TABLE_LEN_LIMIT)
	{
		(*parser)->return_flag = 2;
		ft_set_lasts(NULL, 0, 2, 2);
		write(2, "minishe11: argument overflow6\n", 31);
		error_fun(&(g_access.parser2exec), &(g_access.lexor2parser));
		return (2);
	}
	return (0);
}

void	ft_pipe_limit_handler(t_parser **parser)
{
	if ((*parser)->index_counter > PIPE_LIMIT)
	{
		(*parser)->return_flag = 2;
		ft_set_lasts(NULL, 0, 2, 2);
		write(2, "minishe11: pipe limit reached7\n", 32);
		error_fun(&(g_access.parser2exec), &(g_access.lexor2parser));
	}
}