/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parser_utils1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzivanov <dzivanov@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/24 17:22:36 by dzivanov          #+#    #+#             */
/*   Updated: 2022/02/24 17:25:50 by dzivanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

int	path_finder(char *str, char **cmd_path)
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

int	ft_command_check(char *str, char **cmd_path, int *cmd_type)
{
	int	err;

	*cmd_path = NULL;
	if (!ft_strcmp(str, "pwd") || !ft_strcmp(str, "cd") || \
	!ft_strcmp(str, "echo") || !ft_strcmp(str, "export") || \
	!ft_strcmp(str, "unset") || !ft_strcmp(str, "env") || \
	!ft_strcmp(str, "exit"))
	{
		*cmd_type = FT_CMD_TYPE_BUILT_IN;
		return (0);
	}
	else if (!ft_strcmp(str, "<<") || !ft_strcmp(str, "<") || \
	!ft_strcmp(str, ">>") || !ft_strcmp(str, ">"))
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

void	ft_free_lex_list(t_list *head)
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

char	**add_to_line(char **line, char *new_str, int *line_len)
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
	return (new_line);
}

/*
** 	Handles errors
*/

void	error_fun(t_list **list, t_list **lexor_list)
{
	ft_lstclear(list, ft_free_parser);
	ft_free_lex_list(*lexor_list);
	*lexor_list = NULL;
}
