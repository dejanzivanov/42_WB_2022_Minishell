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

	path = getenv("PATH");
	split = ft_split(ft_strchr(path, '/'), ':');
	temp_path = NULL;
	i = 0;
	while (split[i])
	{
		temp_path = ft_strjoin(split[i], ft_strjoin("/", str));
		if (access(temp_path, F_OK) == 0)
		{
			*cmd_path = temp_path;
			ft_free_split(split);
			return (0);
		}
		else if (access(temp_path, F_OK) == -1)
			free((void *)temp_path);
		i++;
	}
	ft_free_split(split);
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
static char** add_to_line(char **line, char *new_str, int *line_len)
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
	ft_lstclear(list, ft_free_parser);
	ft_free_lex_list(*lexor_list);
	*lexor_list = NULL;
}

static char *create_char_str(char c, int len)
{
	int	counter;
	char* str;

	counter = 0;
	str = (char *)ft_calloc(len + 1, sizeof(char));
	while (counter < len)
	{
		str[counter] = c;
		counter++;
	}
	return str;
}

static void ft_string_handler(t_list *lex_element, char	***cmd_line, int *cmd_len)
{
	char *str;

	if (ft_strlen((char *)((t_word *)(lex_element->content))->address) != 0)
	{
		str = ft_strdup((char *)((t_word *)(lex_element->content))->address);
		*cmd_line = add_to_line(*cmd_line, str, cmd_len);
	}
}

int	parser(void)
{
	int		index_counter;
	t_list	*lex_element;
	t_list	*executor_element;
	char	first_redirect;
	char	**cmd_line;
	int		cmd_len;
	char	*str;
	t_command	*cmd;

	index_counter = 0;
	lex_element = *g_access.lexor2parser;
	cmd = NULL;
	first_redirect = 0;
	while (1)
	{
		cmd_line = 0;
		cmd_len = 0;
		if (lex_element == NULL)
			break;
		while (1)
		{	
			if (lex_element == NULL)
				break;
			if (first_redirect != 0)
			{
				if (((t_word *)(lex_element->content))->type == FT_STRING || *((char *)((t_word *)(lex_element->content))->address) != first_redirect)
					str = create_char_str(first_redirect, 1);
				else
				{
					str = create_char_str(first_redirect, 2);
					lex_element = lex_element->next;
				}
				cmd_line = add_to_line(cmd_line, str, &cmd_len);
				first_redirect = 0;
			}
			if (((t_word *)(lex_element->content))->type == FT_CHAR)
			{
				if (*((char *)((t_word *)(lex_element->content))->address) == FT_GREATER || *((char *)((t_word *)(lex_element->content))->address) == FT_LESSER || first_redirect != 0)
				{
					if (first_redirect == 0)
					{
						first_redirect = *((char *)((t_word *)(lex_element->content))->address);
						if (cmd_len != 0)
							break;
						if (*((char *)((t_word *)((lex_element->next)->content))->address) == first_redirect)
						{
							str = create_char_str(first_redirect, 2);
							lex_element = lex_element->next;
						}
						else
							str = create_char_str(first_redirect, 1);
					}
					cmd_line = add_to_line(cmd_line, str, &cmd_len);
					first_redirect = 0;
				}
				if (*((char *)((t_word *)(lex_element->content))->address) == FT_PIPE)
				{
					if (cmd_len == 0)
						error_fun(g_access.parser2exec, g_access.lexor2parser);
					break ;
				}
			}
			if (((t_word *)(lex_element->content))->type == FT_STRING)
				ft_string_handler(lex_element, &cmd_line, &cmd_len);
			lex_element = lex_element->next;
		}
		cmd = (t_command *)malloc(sizeof(t_command));
		cmd->comm_table = cmd_line;
		cmd->path = NULL;
		cmd->index = index_counter;
		cmd->comm_len = cmd_len;
		cmd->cmd_type = 0;
		ft_command_check(cmd->comm_table[0], &cmd->path, &cmd->cmd_type);
		if (FT_PARSER_COMMENT)
			printf("Path if: %s\n", cmd->path);
		executor_element = ft_lstnew((void * ) cmd);
		ft_lstadd_back(g_access.parser2exec, executor_element);
		index_counter++;
		if (lex_element == NULL)
			break;
		else
			lex_element = lex_element->next;
	}
	ft_free_lex_list(*g_access.lexor2parser);
	*g_access.lexor2parser = NULL;
	if (FT_PARSER_COMMENT)
		print_list_parse(*g_access.parser2exec);
	return (0);
}

