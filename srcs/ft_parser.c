// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   ft_parser.c                                        :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: dzivanov <dzivanov@student.42wolfsburg.de> +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2022/02/23 02:11:57 by dzivanov          #+#    #+#             */
// /*   Updated: 2022/02/23 16:00:20 by dzivanov         ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */

#include "../incl/minishell.h"

// /*
// **  @brief Finds and assignes path to searched command name
// */

// void	ft_print_parser_error(char *str)
// {
// 	write(2, "minishe11: ", 12);
// 	write(2, str, ft_strlen(str));
// 	write(2, ": No such file or directory\n", 28);
// }

// void	ft_clear_env_check(char **str, char *temp_path)
// {
// 	ft_print_parser_error(*str);
// 	ft_set_lasts(NULL, 0, 127, 2);
// 	ft_free_secure((void *)&temp_path);
// }

// int	ft_env_itter(char **temp_path, char ***split, char ***cmd_path)
// {
// 	if (access(*temp_path, F_OK) == 0)
// 	{
// 		errno = 0;
// 		**cmd_path = ft_strdup(*temp_path);
// 		ft_free_split(*split);
// 		ft_free_secure((void *)&(*temp_path));
// 		return (2);
// 	}
// 	else
// 		ft_free_secure((void *)&(*temp_path));
// 	return (0);
// }

// void	ft_setting_path_and_split(char **path, char ***split)
// {
// 	*path = env_value_finder("PATH");
// 	*split = ft_split(ft_strchr(*path, '/'), ':');
// }

// int	ft_access_check(char ***cmd_path, char **str)
// {
// 	if (access(*str, F_OK) == 0)
// 	{
// 		**cmd_path = ft_strdup(*str);
// 		return (1);
// 	}
// 	return (0);
// }

// static int	path_finder(char *str, char **cmd_path)
// {
// 	char	*path;
// 	char	**split;
// 	char	*temp;
// 	int		i;

// 	if (ft_access_check(&cmd_path, &str))
// 		return (0);
// 	ft_setting_path_and_split(&path, &split);
// 	temp = NULL;
// 	i = 0;
// 	if (split != NULL)
// 	{
// 		while (split[i])
// 		{
// 			temp = ft_strjoin_with_scnd_free(split[i], ft_strjoin("/", str));
// 			if (ft_env_itter(&temp, &split, &cmd_path))
// 				return (0);
// 			i++;
// 		}
// 		ft_free_split(split);
// 	}
// 	ft_clear_env_check(&str, temp);
// 	return (-1);
// }

// /*
// **  @brief Checks command name and assign command variables properly
// */

// static int	ft_command_check(char *str, char **cmd_path, int *cmd_type)
// {
// 	int	err;

// 	err = 0;
// 	*cmd_path = NULL;
// 	if (!ft_strcmp(str, "pwd") || !ft_strcmp(str, "cd") ||
// 	!ft_strcmp(str, "echo") || !ft_strcmp(str, "export") ||
// 	!ft_strcmp(str, "unset") || !ft_strcmp(str, "env") ||
// 	!ft_strcmp(str, "exit"))
// 	{
// 		*cmd_type = FT_CMD_TYPE_BUILT_IN;
// 		return (0);
// 	}
// 	else if (!ft_strcmp(str, "<<") || !ft_strcmp(str, "<") || !ft_strcmp(str, ">>") || !ft_strcmp(str, ">"))
// 	{
// 		*cmd_type = FT_CMD_TYPE_REDIRECT;
// 		return (0);
// 	}
// 	err = path_finder(str, cmd_path);
// 	if (err == 0 && ft_strcmp(str, "/") != 0)
// 		*cmd_type = FT_CMD_TYPE_SYSTEM;
// 	else if (err == -1)
// 		*cmd_type = FT_CMD_TYPE_ERROR;
// 	return (err);
// }

// /*
// **  Use the lex one
// */
// static void	ft_free_lex_list(t_list *head)
// {
// 	t_list	*tmp;

// 	while (head != NULL)
// 	{
// 		tmp = head;
// 		head = head->next;
// 		free(((t_word *)(tmp->content))->address);
// 		free(tmp->content);
// 		free(tmp);
// 	}
// }

// /*
// ** 	Adds new argument to command table
// */
// static char **add_to_line(char **line, char *new_str, int *line_len)
// {
// 	char	**new_line;
// 	int		counter;

// 	new_line = (char **)malloc(sizeof(char *) * ((*line_len) + 1));
// 	counter = 0;
// 	while (counter < *line_len)
// 	{
// 		new_line[counter] = line[counter];
// 		counter++;
// 	}
// 	new_line[counter] = new_str;
// 	(*line_len)++;
// 	if (line != NULL)
// 		free(line);
// 	return new_line;
// }

// /*
// ** 	Handles errors
// */
// static void error_fun(t_list **list, t_list **lexor_list)
// {
// 	ft_lstclear(list, ft_free_parser);
// 	ft_free_lex_list(*lexor_list);
// 	*lexor_list = NULL;
// }


// static void ft_string_handler(t_list *lex_element, char	***cmd_line, int *cmd_len)
// {
// 	char *str;

// 	if (ft_strlen(((t_word *)(lex_element->content))->address) != 0)
// 	{
// 		str = ft_strdup(((t_word *)(lex_element->content))->address);
// 		*cmd_line = add_to_line(*cmd_line, str, cmd_len);
// 	}
// }

// /*
// **  @brief Chechk if string is same as one of redirects
// */
// int is_redirect(char *str)
// {
// 	if (!ft_strcmp(str, ">") || !ft_strcmp(str, "<") || !ft_strcmp(str, ">>") || !ft_strcmp(str, "<<"))
// 		return (1);
// 	return (0);
// }


// /*
// **  @brief Chechk if string is same as pip
// */
// int is_pipe(char *str)
// {
// 	if (!ft_strcmp(str, "|"))
// 		return (1);
// 	return (0);
// }

// void ft_parser_error_handler(char **cmd_line_red, int mod, t_struktur **struktur)
// {
// 	ft_set_lasts(NULL, 0, 2, 2);
// 	if(mod == 0)
// 		write(2, "minshe11: syntax error near unexpected token `neewline'\n", 56);
// 	if(mod == 1)
// 		write(2, "minishe11: syntax error near unexpected token'\n", 47);
// 	ft_free_secure((void *)&cmd_line_red[0]);
// 	free(cmd_line_red);
// 	error_fun(&(g_access.parser2exec), &(g_access.lexor2parser));
// 	(*struktur)->return_flag = 2;
// }

// int ft_parser_general_error_handler(int mod, t_struktur **struktur)
// {
// 	ft_set_lasts(NULL, 0, 2, 2);
// 	if (mod == 0)
// 		write(2, "minishe11: syntax error near unexpected tokeen'\n", 48);
// 	if(mod == 1)
// 		write(2, "minishe11: argument overflow\n", 29);
// 	if (mod == 2)
// 		write(2, "minishe11: pipe limit reached\n", 30);
// 	error_fun(&(g_access.parser2exec), &(g_access.lexor2parser));
// 	return (*struktur)->return_flag = 2;
// }

// int	ft_pipe_handler(int cmd_len, t_list **lex_element)
// {
// 	if (cmd_len == 0)
// 	{
// 		ft_set_lasts(NULL, 0, 2, 2);
// 		write(2, "minishe11: syntax error near unexpected tokeen `|'\n", 52);
// 		error_fun(&(g_access.parser2exec), &(g_access.lexor2parser));
// 		return (2);
// 	}
// 	else
// 	{
// 		*lex_element = (*lex_element)->next;
// 		if (*lex_element == NULL)
// 		{
// 			ft_set_lasts(NULL, 0, 2, 2);
// 			write(2, "minishe11: syntax error near unexpected token `|'\n", 51);
// 			error_fun(&(g_access.parser2exec), &(g_access.lexor2parser));
// 			return (2);
// 		}
// 	}
// 	return (0);
// }

// void ft_free_lex_list_handler(int return_flag, t_struktur **struktur)
// {
// 	if (return_flag == 0 || return_flag == 2)
// 	{
// 		ft_free_lex_list(g_access.lexor2parser);
// 		(*struktur)->lex_element = NULL;
// 		g_access.lexor2parser = NULL;
// 		ft_free_secure((void *)struktur);
// 	}
// }

void ft_add_redirect_element(char ***cmd_line_red, int index_counter)
{
	t_list	*executor_element;
	t_command	*cmd;

	cmd = NULL;
	executor_element = NULL;

	cmd = (t_command *)malloc(sizeof(t_command));
	cmd->comm_table = *cmd_line_red;
	cmd->path = NULL;
	cmd->index = index_counter;
	cmd->comm_len = 3;
	cmd->cmd_type = FT_CMD_TYPE_REDIRECT;
	executor_element = ft_lstnew((void * ) cmd);
	ft_lstadd_back(&(g_access.parser2exec), executor_element);
}

void ft_add_command_element(char ***cmd_line, int index_counter, int cmd_len)
{
	t_list	*executor_element;
	t_command	*cmd;

	cmd = NULL;
	executor_element = NULL;

	cmd = (t_command *)malloc(sizeof(t_command));
	cmd->comm_table = *cmd_line;
	cmd->path = NULL;
	cmd->index = index_counter;
	cmd->comm_len = cmd_len;
	cmd->cmd_type = 0;
	ft_command_check(cmd->comm_table[0], &cmd->path, &cmd->cmd_type);
	executor_element = ft_lstnew((void * ) cmd);
	ft_lstadd_back(&(g_access.parser2exec), executor_element);
}



// void ft_parser_generation(t_struktur **struktur)
// {
// 	if ((*struktur)->return_flag == 0)
// 	{
// 		if((*struktur)->cmd_len > 1)
// 			ft_add_command_element((*struktur)->cmd_line, (*struktur)->index_counter, (*struktur)->cmd_len);
// 		else
// 			ft_free_split((*struktur)->cmd_line);
// 		(*struktur)->index_counter++;
// 		if ((*struktur)->index_counter > PIPE_LIMIT)
// 			(*struktur)->return_flag = ft_parser_general_error_handler(2, struktur);
// 	}
// 	else
// 		ft_free_split((*struktur)->cmd_line);
// }

// int ft_parser_redirect_check(t_struktur **struktur)
// {
// 	if (is_redirect(((t_word *)((*struktur)->lex_element->content))->address))
// 	{
// 		(*struktur)->cmd_line_red = (char **)ft_calloc(3, sizeof(char *));
// 		(*struktur)->cmd_line_red[0] = ft_strdup(((t_word *)((*struktur)->lex_element->content))->address);
// 		(*struktur)->lex_element = (*struktur)->lex_element->next;
// 		if((*struktur)->lex_element == NULL)
// 		{
// 			ft_parser_error_handler((*struktur)->cmd_line_red, 0, struktur);
// 			return (2);
// 		}
// 		if(((t_word *)((*struktur)->lex_element->content))->type == FT_SPECIAL_CHAR_STRING)
// 		{
// 			ft_parser_error_handler((*struktur)->cmd_line_red, 1, struktur);
// 			return (0);
// 		}
// 		(*struktur)->cmd_line_red[1] = ft_strdup(((t_word *)((*struktur)->lex_element->content))->address);
// 		ft_add_redirect_element(&(*struktur)->cmd_line_red, (*struktur)->index_counter);
// 	}
// return (0);
// }

// int ft_parser_pipe_check(t_struktur **struktur)
// {
// 	if (is_pipe(((t_word *)((*struktur)->lex_element->content))->address))
// 	{
// 		(*struktur)->return_flag = ft_pipe_handler((*struktur)->cmd_len, &(*struktur)->lex_element);
// 		return (2);
// 	}
// 	return (0);
// }

// int ft_parser_itteration(t_struktur **struktur)
// {
// 	if (((t_word *)((*struktur)->lex_element->content))->type == FT_SPECIAL_CHAR_STRING)
// 	{
// 		if(ft_parser_redirect_check(struktur))
// 			return (2) ;
// 		else if (ft_parser_pipe_check(struktur))
// 			return (2) ;
// 		else
// 		{
// 			(*struktur)->return_flag = ft_parser_general_error_handler(0, struktur);
// 			return (2) ;
// 		}
// 	}
// 	else if (((t_word *)((*struktur)->lex_element->content))->type == FT_STRING)
// 	{
// 		ft_string_handler((*struktur)->lex_element, &(*struktur)->cmd_line, &(*struktur)->cmd_len);
// 	}
// 	if ((*struktur)->cmd_len > PARSER_TABLE_LEN_LIMIT)
// 	{
// 		(*struktur)->return_flag = ft_parser_general_error_handler(1, struktur);
// 		return (2) ;
// 	}
// 	return (0);
// }

// int	parser(void)
// {
// 	int return_value;
// 	t_struktur *struktur;

// 	return_value = ft_setup_parser(&struktur);
// 	while (struktur->return_flag == 0)
// 	{
// 		struktur->cmd_line = 0;
// 		struktur->cmd_len = 0;
// 		if (struktur->lex_element == NULL || struktur->return_flag != 0)
// 			break ;
// 		while (struktur->return_flag == 0)
// 		{
// 			if (struktur->lex_element == NULL)
// 				break ;
// 			if (ft_parser_itteration(&struktur))
// 				break;
// 			struktur->lex_element = struktur->lex_element->next;
// 		}
// 		struktur->cmd_line = add_to_line(struktur->cmd_line, NULL, &struktur->cmd_len);
// 		ft_parser_generation(&struktur);
// 	}
// 	return_value = struktur->return_flag;
// 	ft_free_lex_list_handler(return_value, &struktur);
// 	return (return_value);
// }


#include "../incl/minishell.h"


/*
**  @brief Finds and assignes path to searched command name
*/
// static int path_finder(char *str, char **cmd_path)
// {
// 	char	*path;
// 	char	**split;
// 	char	*temp_path;
// 	int		i;

// 	if (access(str, F_OK) == 0)
// 	{
// 		*cmd_path = ft_strdup(str);
// 		return (0);
// 	}
// 	path = env_value_finder("PATH");
// 	split = ft_split(ft_strchr(path, '/'), ':');
// 	temp_path = NULL;
// 	i = 0;
// 	if (split != NULL)
// 	{
// 		while (split[i])
// 		{
// 			temp_path = ft_strjoin_with_scnd_free(split[i], ft_strjoin("/", str));
// 			if (access(temp_path, F_OK) == 0)
// 			{
// 				errno = 0;
// 				*cmd_path = ft_strdup(temp_path);
// 				ft_free_split(split);
// 				if(temp_path != NULL)
// 					free(temp_path);
// 				temp_path = NULL;
// 				return (0);
// 			}
// 			else
// 			{
// 				if (temp_path != NULL)
// 					free((void *)temp_path);
// 				temp_path = NULL;
// 			}
// 			i++;
// 		}
// 		ft_free_split(split);
// 	}
// 	write(2, "minishell: ", 12);
// 	write(2, str, ft_strlen(str));
// 	write(2, ": No such file or directory\n", 28);
// 	if(temp_path != NULL)
// 		free(temp_path);
// 	return (-1);
// }

void	ft_print_parser_error(char *str)
{
	write(2, "minishe11: ", 12);
	write(2, str, ft_strlen(str));
	write(2, ": No such file or dingustory\n", 28);
}

void	ft_clear_env_check(char **str, char *temp_path)
{
	ft_print_parser_error(*str);
	ft_set_lasts(NULL, 0, 127, 2);
	ft_free_secure((void *)&temp_path);
}

int	ft_env_itter(char **temp_path, char ***split, char ***cmd_path)
{
	if (access(*temp_path, F_OK) == 0)
	{
		errno = 0;
		**cmd_path = ft_strdup(*temp_path);
		ft_free_split(*split);
		ft_free_secure((void *)&(*temp_path));
		return (2);
	}
	else
		ft_free_secure((void *)&(*temp_path));
	return (0);
}

void	ft_setting_path_and_split(char **path, char ***split)
{
	*path = env_value_finder("PATH");
	*split = ft_split(ft_strchr(*path, '/'), ':');
}

int	ft_access_check(char ***cmd_path, char **str)
{
	if (access(*str, F_OK) == 0)
	{
		**cmd_path = ft_strdup(*str);
		return (1);
	}
	return (0);
}

static int	path_finder(char *str, char **cmd_path)
{
	char	*path;
	char	**split;
	char	*temp;
	int		i;

	if (ft_access_check(&cmd_path, &str))
		return (0);
	ft_setting_path_and_split(&path, &split);
	temp = NULL;
	i = 0;
	if (split != NULL)
	{
		while (split[i])
		{
			temp = ft_strjoin_with_scnd_free(split[i], ft_strjoin("/", str));
			if (ft_env_itter(&temp, &split, &cmd_path))
				return (0);
			i++;
		}
		ft_free_split(split);
	}
	ft_clear_env_check(&str, temp);
	return (-1);
}

/*
**  @brief Checks command name and assign command variables properly
*/
int ft_command_check(char *str, char **cmd_path, int *cmd_type)
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
static void error_fun(t_list **list, t_list **lexor_list)
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

int	ft_setup_parser(t_parser **parser)
{
	*parser = ft_calloc(sizeof(t_parser), 1);
	(*parser)->index_counter = 0;
	(*parser)->lex_element = g_access.lexor2parser;
	(*parser)->return_flag = 0;
	return (0);
}

int	parser(void)
{
	// int		index_counter;
	// t_list	*lex_element;
	// char	**cmd_line;
	// char	**cmd_line_red;
	// int		cmd_len;
	// int 	return_flag;

	t_parser *parser;
	int ret_value;

	ret_value = ft_setup_parser(&parser);


	// index_counter = 0;
	// lex_element = g_access.lexor2parser;
	// return_flag = 0;
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
						write(2, "minshe11: syntax error near unexpected token `newline'1\n", 57);
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
					ft_add_redirect_element(&parser->cmd_line_red, parser->index_counter);
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
					write(2, "minishe11: syntax error near unexpected token else'5\n", 54);
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
				ft_add_command_element(&parser->cmd_line, parser->index_counter, parser->cmd_len);
			else
				ft_free_split(parser->cmd_line);

			parser->index_counter++;
			if (parser->index_counter > PIPE_LIMIT)
			{
				parser->return_flag = 2;
				if (g_access.last_return != NULL)
					free(g_access.last_return);
				g_access.last_return = ft_itoa(2);
				write(2, "minishe11: pipe limit reached7\n", 31);
				error_fun(&(g_access.parser2exec), &(g_access.lexor2parser));
			}
		}
		else
			ft_free_split(parser->cmd_line);
	}
	ret_value = parser->return_flag;
	ft_free_lex_list_handler(parser->return_flag, &parser);
	return (ret_value);
}

void ft_free_lex_list_handler(int return_flag, t_parser **parser)
// void ft_free_lex_list_handler(int return_flag)
{
	if (return_flag == 0 || return_flag == 2)
	{
		ft_free_lex_list(g_access.lexor2parser);
		(*parser)->lex_element = NULL;
		g_access.lexor2parser = NULL;
		ft_free_secure((void *)parser);
	}
}