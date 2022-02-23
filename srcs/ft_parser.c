#include "../incl/minishell.h"


/*
**  @brief Finds and assignes path to searched command name
*/
static int path_finder(char *str, char **cmd_path)
{
	char	*path;
	char	**split;
	char	*temp_path;
	int		i;

	if (access(str, F_OK) == 0)
	{
		*cmd_path = ft_strdup(str);
		return (0);
	}
	path = env_value_finder("PATH");
	split = ft_split(ft_strchr(path, '/'), ':');
	temp_path = NULL;
	i = 0;
	if (split != NULL)
	{
		while (split[i])
		{
			temp_path = ft_strjoin_with_scnd_free(split[i], ft_strjoin("/", str));
			if (access(temp_path, F_OK) == 0)
			{
				errno = 0;
				*cmd_path = ft_strdup(temp_path);
				ft_free_split(split);
				if(temp_path != NULL)
					free(temp_path);
				temp_path = NULL;
				return (0);
			}
			else
			{
				if (temp_path != NULL)
					free((void *)temp_path);
				temp_path = NULL;
			}
			i++;
		}
		ft_free_split(split);
	}
	write(2, "minishell: ", 12);
	write(2, str, ft_strlen(str));
	write(2, ": No such file or directory\n", 28);
	if (g_access.last_return != NULL)
		free(g_access.last_return);
	g_access.last_return = ft_itoa(127);
	if(temp_path != NULL)
		free(temp_path);
	return (-1);
}

/*
**  @brief Checks command name and assign command variables properly
*/
static int ft_command_check(char *str, char **cmd_path, int *cmd_type)
{
	int err;
	*cmd_path = NULL;
	if (!ft_strcmp(str, "pwd") || !ft_strcmp(str, "cd") || !ft_strcmp(str, "echo")  || !ft_strcmp(str, "export") || !ft_strcmp(str, "unset") ||
		!ft_strcmp(str, "env") || !ft_strcmp(str, "exit") )
 	{
		*cmd_type = FT_CMD_TYPE_BUILT_IN;
		return (0);
 	}
	else if(!ft_strcmp(str, "<<") || !ft_strcmp(str, "<") || !ft_strcmp(str, ">>") || !ft_strcmp(str, ">"))
	{
		*cmd_type = FT_CMD_TYPE_REDIRECT;
		return (0);
	}
	err = path_finder(str, cmd_path);
	if (err == 0)
		*cmd_type = FT_CMD_TYPE_SYSTEM;
	else if (err == -1)
		*cmd_type = FT_CMD_TYPE_ERROR;
	return (err);
}

/*
**  Use the lex one
*/
static void	ft_free_lex_list(t_list *head)
{
	t_list	*tmp;

	while (head != NULL)
	{
		tmp = head;
		head = head->next;
		free(((t_word *)(tmp->content))->address);
		free(tmp->content);
		free(tmp);
	}
}

/*
** 	Adds new argument to command table
*/
static char **add_to_line(char **line, char *new_str, int *line_len)
{
	char	**new_line;
	int		counter;

	new_line = (char **)malloc(sizeof(char *) * ((*line_len) + 1));
	counter = 0;
	while (counter < *line_len)
	{
		new_line[counter] = line[counter];
		counter++;
	}
	new_line[counter] = new_str;
	(*line_len)++;
	if (line != NULL)
		free(line);
	return new_line;
}

/*
** 	Handles errors
*/
void error_fun(t_list **list, t_list **lexor_list)
{
	if (FT_PARSER_COMMENT)
		printf("!!!!!!!!!!!!!!!!!!!!!!!PARSER ERROR!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
	ft_lstclear(list, ft_free_parser);
	ft_free_lex_list(*lexor_list);
	*lexor_list = NULL;
}


static void ft_string_handler(t_list *lex_element, char	***cmd_line, int *cmd_len)
{
	char *str;

	if (ft_strlen(((t_word *)(lex_element->content))->address) != 0)
	{
		str = ft_strdup(((t_word *)(lex_element->content))->address);
		*cmd_line = add_to_line(*cmd_line, str, cmd_len);
	}
}

/*
**  @brief Chechk if string is same as one of redirects
*/
int is_redirect(char *str)
{
	if (!ft_strcmp(str, ">") || !ft_strcmp(str, "<") || !ft_strcmp(str, ">>") || !ft_strcmp(str, "<<"))
		return (1);
	return (0);
}


/*
**  @brief Chechk if string is same as pip
*/
int is_pipe(char *str)
{
	if (!ft_strcmp(str, "|"))
		return (1);
	return (0);
}

int ft_setup_parser(t_parser **parser)
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
	return 0;
}

void ft_add_redirect_command(t_parser **parser)
{
	(*parser)->cmd_line_red[1] = ft_strdup(((t_word *)((*parser)->lex_element->content))->address);
	(*parser)->cmd = (t_command *)malloc(sizeof(t_command));
	(*parser)->cmd->comm_table = (*parser)->cmd_line_red;
	(*parser)->cmd->path = NULL;
	(*parser)->cmd->index = (*parser)->index_counter;
	(*parser)->cmd->comm_len = 3;
	(*parser)->cmd->cmd_type = FT_CMD_TYPE_REDIRECT;
	(*parser)->executor_element = ft_lstnew((void * ) (*parser)->cmd);
	ft_lstadd_back(&(g_access.parser2exec), (*parser)->executor_element);
}

int	parser(void)
{
	t_parser *parser;
	int ret_val;

	ret_val = ft_setup_parser(&parser);

	while (parser->return_flag == 0)
	{
		parser->cmd_line = 0;
		parser->cmd_len = 0;
		if (parser->lex_element == NULL || parser->return_flag != 0)
			break;
		while (parser->return_flag == 0)
		{
			if (parser->lex_element == NULL)
				break;
			if (((t_word *)(parser->lex_element->content))->type == FT_SPECIAL_CHAR_STRING)
			{
				if (is_redirect(((t_word *)(parser->lex_element->content))->address))
				{
					parser->cmd_line_red = (char **)ft_calloc(3, sizeof(char *));
					parser->cmd_line_red[0] = ft_strdup(((t_word *)(parser->lex_element->content))->address);
					parser->lex_element = parser->lex_element->next;
					if(parser->lex_element == NULL)
					{
						parser->return_flag = 2;
						if (g_access.last_return != NULL)
							free(g_access.last_return);
						g_access.last_return = ft_itoa(2);
						write(2, "minshe11: syntax error near unexpected token `newline'1\n", 58);
						if (parser->cmd_line_red[0] != NULL)
						{
							free(parser->cmd_line_red[0]);
							parser->cmd_line_red[0] = NULL;
						}
						free(parser->cmd_line_red);
						error_fun(&(g_access.parser2exec), &(g_access.lexor2parser));
						break;
					}
					if(((t_word *)(parser->lex_element->content))->type == FT_SPECIAL_CHAR_STRING)
					{
						parser->return_flag = 2;
						if (g_access.last_return != NULL)
							free(g_access.last_return);
						g_access.last_return = ft_itoa(2);
						write(2, "minishe11: syntax error near unexpected token'2\n", 49);
						if (parser->cmd_line_red[0] != NULL)
						{
							free(parser->cmd_line_red[0]);
							parser->cmd_line_red[0] = NULL;
						}
						free(parser->cmd_line_red);
						error_fun(&(g_access.parser2exec), &(g_access.lexor2parser));
						break;
					}
					ft_add_redirect_command(&parser);

				}
				else  if (is_pipe(((t_word *)(parser->lex_element->content))->address))
				{
					if (parser->cmd_len == 0)
					{
						parser->return_flag = 2;
						if (g_access.last_return != NULL)
							free(g_access.last_return);
						g_access.last_return = ft_itoa(2);
						write(2, "minishe11: syntax error near unexpected token `|'3\n", 52);
						error_fun(&(g_access.parser2exec), &(g_access.lexor2parser));
					}
					else
					{
						parser->lex_element = parser->lex_element->next;
						if (parser->lex_element == NULL)
						{
							parser->return_flag = 2;
							if (g_access.last_return != NULL)
								free(g_access.last_return);
							g_access.last_return = ft_itoa(2);
							write(2, "minishe11: syntax error near unexpected token `|'4\n", 52);
						error_fun(&(g_access.parser2exec), &(g_access.lexor2parser));
						}
					}
					break ;
				}
				else
				{
					parser->return_flag = 2;
					if (g_access.last_return != NULL)
						free(g_access.last_return);
					g_access.last_return = ft_itoa(2);
					write(2, "minishe11: syntax error near unexpected token'5\n", 49);
					error_fun(&(g_access.parser2exec), &(g_access.lexor2parser));
					break ;
				}
			}
			else if (((t_word *)(parser->lex_element->content))->type == FT_STRING)
				ft_string_handler(parser->lex_element, &parser->cmd_line, &parser->cmd_len);
			if (parser->cmd_len > PARSER_TABLE_LEN_LIMIT)
			{
				parser->return_flag = 2;
				if (g_access.last_return != NULL)
					free(g_access.last_return);
				g_access.last_return = ft_itoa(2);
				write(2, "minishe11: argument overflow6\n", 31);
				error_fun(&(g_access.parser2exec), &(g_access.lexor2parser));
				break;
			}
			parser->lex_element = parser->lex_element->next;
		}
		parser->cmd_line = add_to_line(parser->cmd_line, NULL, &parser->cmd_len);
		if (parser->return_flag == 0)
		{
			if(parser->cmd_len > 1)
			{
				parser->cmd = (t_command *)malloc(sizeof(t_command));
				parser->cmd->comm_table = parser->cmd_line;
				parser->cmd->path = NULL;
				parser->cmd->index = parser->index_counter;
				parser->cmd->comm_len = parser->cmd_len;
				parser->cmd->cmd_type = 0;

				ft_command_check(parser->cmd->comm_table[0], &parser->cmd->path, &parser->cmd->cmd_type);
				if (FT_PARSER_COMMENT)
					printf("Path if: %s\n", parser->cmd->path);
				parser->executor_element = ft_lstnew((void * ) parser->cmd);
				ft_lstadd_back(&(g_access.parser2exec), parser->executor_element);
			}
			else
				ft_free_split(parser->cmd_line);

			parser->index_counter++;
			if (parser->index_counter > PIPE_LIMIT)
			{
				parser->return_flag = 2;
				if (g_access.last_return != NULL)
					free(g_access.last_return);
				g_access.last_return = ft_itoa(2);
				write(2, "minishe11: pipe limit reached7\n", 32);
				error_fun(&(g_access.parser2exec), &(g_access.lexor2parser));
			}
		}
		else
			ft_free_split(parser->cmd_line);
	}
	if (parser->return_flag == 0)
	{
		ft_free_lex_list(g_access.lexor2parser);
		if(parser->lex_element)
		{
			free(parser->lex_element);
			parser->lex_element = NULL;
		}
		g_access.lexor2parser = NULL;
	}
	if (FT_PARSER_COMMENT)
		print_list_parse(g_access.parser2exec);
	ret_val = parser->return_flag;
	if(parser != NULL)
		free(parser);
	parser = NULL;
	return (ret_val);
}
