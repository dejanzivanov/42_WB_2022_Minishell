#include "../incl/minishell.h"

void	make_cmd_list(t_list **cmd_list)
{
	t_command	*temp;

	temp = (t_command *)malloc(sizeof(t_command));
	temp->comm_len = 3;
	temp->comm_table = (char **)malloc(sizeof(char *) * temp->comm_len);
	temp->comm_table[0] = "ls";
	temp->comm_table[1] = "-lah";
	temp->comm_table[2] = NULL;
	temp->index = 0;
	temp->cmd_type = FT_CMD_TYPE_SYSTEM;
	temp->path = "/usr/bin/ls";
	ft_lstadd_back(cmd_list, ft_lstnew(temp));
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! the last line should be executable by execve !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
}

int	executor(char **envp)
{
	pipex(g_access.parser2exec, envp);
/* 	print_list_parse(g_access.parser2exec); */
	ft_lstclear(&g_access.parser2exec, ft_free_parser);
	return (1);
}
