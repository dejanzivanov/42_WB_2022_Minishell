/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzivanov <dzivanov@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/25 17:35:33 by dzivanov          #+#    #+#             */
/*   Updated: 2022/02/25 17:35:33 by dzivanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/incl/libft.h"
// # include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <unistd.h>
// # include <stdlib.h>
# include <errno.h>
// # include <string.h>
// # include <sys/wait.h>
// # include <stdarg.h>
# include <fcntl.h>
// # include <sys/types.h>
// # include <signal.h>
# include <dirent.h>
# include <sys/stat.h>

# define CRED "\001\e[0;31m\002"
# define RESET   "\001\e[0m\002"
# define FT_SPECIAL_CHAR_STRING 1
# define FT_STRING 2
# define FT_TAB 9
# define FT_SPACE 32
# define FT_SINGLE_QUOTE 39
# define FT_DOUBLE_QUOTE 34
# define FT_DOLLAR_SIGN 36
# define FT_HASHTAG	35
# define FT_GREATER 62
# define FT_LESSER 60
# define FT_QUESTION_MARK 63 //check in future
# define FT_MINUS 45
# define FT_EQUAL 61
# define FT_PIPE 124
# define FT_WILDCARD 42
# define FT_L_PAR 40
# define FT_R_PAR 41
# define FT_AMPERSAND 38
# define FT_TILDE 126
# define FT_EXCL_MARK 33
# define FT_UNDERSCORE 95
# define FT_PARSER_COMMENT 0
# define FT_LEXOR_COMMENT 0

# define FT_CMD_TYPE_ERROR -1
# define FT_CMD_TYPE_SYSTEM 0
# define FT_CMD_TYPE_BUILT_IN 1
# define FT_CMD_TYPE_REDIRECT 2

# define FT_LIST_TYPE_WORD 0
# define FT_LIST_TYPE_COMMAND 1
# define FT_LIST_TYPE_BUILTIN_CONTENT 2
# define FT_LIST_TYPE_ENV_VAR 3

# define BUFFER_SIZE 1
# define OUT_WRITE 0
# define OUT_APPEND 1
# define IN_READFILE 2
# define IN_HEREDOC 3

# define SYMLINK 1
# define NOT_SYMLINK 0

# define PARSER_TABLE_LEN_LIMIT 1000
# define PIPE_LIMIT 1000

typedef struct s_word
{
	char	*address;
	int		type;
}			t_word;

typedef struct s_command
{
	char	**comm_table ;
	int		comm_len;
	int		index;
	char	*path;
	int		cmd_type;
}			t_command;

typedef struct s_builtin_content
{
	char	*cmd;
	int		(*minishell_fct)(char **args, pid_t pid);
	int		index;
}	t_builtin_content;

typedef struct s_global
{
	int		signals;
	t_list	*env;
	t_list	*builtins;
	t_list	*parser2exec;
	t_list	*lexor2parser;
	char	*read_line2lexor;
	char	*last_return;
	char	*pwd;
	char	*home;
	char	*dp;
	int		inter;
}				t_global;

typedef struct s_env_var
{
	char	*name;
	char	*value;
}				t_env_var;

typedef struct s_lexor
{
	int		i;
	int		begining;
	char	last;
	int		flag;
}	t_lexor;

typedef struct s_env
{
	int		i;
	int		j;
	int		s_quote_flag;
	int		d_quote_flag;
	char	*temp1;
	char	*temp0;
}	t_env;

typedef struct s_parser
{
	int			index_counter;
	t_list		*lex_element;
	t_list		*executor_element;
	char		**cmd_line;
	char		**cmd_line_red;
	int			cmd_len;
	t_command	*cmd;
	int			return_flag;
}	t_parser;

typedef struct s_pipex
{
	int			fd_stream[2];
	pid_t		*pidt;
	t_command	*cmd;
	t_list		*cmd_list_temp;
	int			**fd_docks;
	int			i;
	int			fd_in[2];
	int			fd_out[2];
	int			last_index;
	int			exit_value;
}	t_pipex;

extern t_global	g_access;

t_builtin_content	*ft_init_builtin_content(char *cmd, \
int (*minishell_fct)(char **args, pid_t pid), int i);
void				ft_free_split(char **split);
void				free_global(void);
void				ft_signal_setup(void);
int					lexor(void);
int					parser(void);
int					executor(char **envp);
void				ft_initiator(char **envp, char *executable);
void				ft_init_builtins(void);
int					minishell_cd(char **args, pid_t pid);
int					minishell_env(char **args, pid_t pid);
int					minishell_echo(char **args, pid_t pid);
int					minishell_exit(char **args, pid_t pid);
int					minishell_pwd(char **args, pid_t pid);
int					minishell_export(char **args, pid_t pid);
int					minishell_unset(char **args, pid_t pid);
int					minishell_execute(void);
int					minishell_launch(char **args);

//INIT UTILS
void				ft_get_home(void);
void				prerror(char *msg);
void				ft_update_shell_env(char *executable);
int					ft_check_symlink(char *path, char *arg, pid_t pid);
void				ft_symlink_initiate_free(struct stat **buf, \
char **path_substr, char **path_substr_free, int mode);

// BUILTIN UTILS
void				ft_update_env(char *to_search, char *to_replace);
char				*env_value_finder(char *name);
void				delone(void *content);

// ECHO UTILS
void				echo_print(char **str, int starter, int size, int flag);
int					echo_flag(char *str);
void				ft_echo_printer(char **args, int len, int flag);

// CD UTILS
void				ft_update_create_env(char *env, char *value, pid_t pid);
void				ft_update_PWD(void);
int					ft_cd_error_handler(char *str, \
pid_t pid, char **path, char **temp);
void				ft_update_dir(char *arg1, char *path);
void				ft_rtoa_path(char *rel_path, char **abs_path);
void				ft_cd_simlink(char *abs_path, char *current_path, \
int pid, int mode);
void				ft_set_current_path(char **current_path);

//env utils
void				ft_env_quote_handler(char **args, t_env **env);
void				ft_env_string_handler(char **args, t_env **env);
int					ft_env_char_check(char **args, t_env **env);
int					ft_env_dollar_handler(char **args, t_env **env);
void				ft_env_check(char **args);
void				ft_setup_env_check(t_env **env);
char				*ft_getenv(char *str);
char				*env_var_formater(char *env_var);

//parser env utils
void				ft_print_parser_error(char *str);
void				ft_clear_env_check(char **str, char *temp_path);
int					ft_env_itter(char **temp_path, char ***split, \
char ***cmd_path);
void				ft_setting_path_and_split(char **path, char ***split);
int					ft_access_check(char ***cmd_path, char **str);

//parser utils
int					path_finder(char *str, char **cmd_path);
int					ft_command_check(char *str, char **cmd_path, int *cmd_type);
void				ft_free_lex_list(t_list *head);
char				**add_to_line(char **line, char *new_str, int *line_len);
void				ft_str_ha(t_list *lex_element, char	***cmd_line, \
int *cmd_len);
int					is_redirect(char *str);
int					is_pipe(char *str);
int					ft_setup_parser(t_parser **parser);
void				ft_add_redirect_command(t_parser **parser);
void				ft_add_command(t_parser **parser);
int					ft_parser_redirect_error_check(t_parser **parser);
int					ft_parser_last_redirect_element(t_parser **parser);
int					ft_parser_redirect_handler(t_parser **parser);
int					ft_pipe_handler(t_parser **parser);
int					ft_free_parse_struct(t_parser *parser);
void				ft_parser_error_handler(t_parser **parser);
int					ft_cmd_limit_handler(t_parser **parser);
void				ft_pipe_limit_handler(t_parser **parser);

// EXIT UTILS
int					ft_digit_check(char *argv);
long long int		ft_atoll(const char *str);
int					ft_get_index(void);
int					ft_exit_error_handler(char *str1, char *str2, char *str3, \
int exit_value);
void				ft_exit(int exit_value);
int					ft_parent_exiter(int pid, char **args);
void				ft_child_exiter(char **args);

//EXPORT UTILS
void				ft_print_sorted_copy(t_list *env_cpy);
int					ft_single_export(char **args, int pid, int lreturn, \
int mode);
int					ft_check_existing_env(t_env_var **env_var);
int					ft_export_error_checker(char **args, int i, int pid);
void				ft_add_env_export(char **args, int i, int j, int valid);

// LEXOR UTILS
char				*join2current_str(char *current_str, char *add_on);
void				ft_free_list(t_list *head);
void				print_element(void *input);
void				print_list(t_list *el);
void				add_string(t_list **list, char	*str);
int					is_special_char(char ch);
void				ft_setup_lexor(t_lexor **lex);
void				ft_lex_handler(t_lexor **lex, char **current_str, \
char *args);
void				ft_lex_quote_handler(t_lexor **lex, char **current_str, \
char *args);
void				ft_lex_exit(t_lexor **lex, char **current_str, char *args);
void				add_specialchar_string(t_list **list, char *str);
int					q_handler(char *str, char **current_str, char q_char);
void				errorfun(void);
void				ft_lex_string_reminder_handler(char **current_str, \
char *args, int begining, int i);
int					ft_lex_double_quote_handler(char **current_str, char *args, \
int begining, int i);
int					ft_lex_single_quote_handler(char **current_str, char *args, \
int begining, int i);
void				ft_lex_operand_handler(char **current_str, char *args, \
int begining, int i);
void				ft_lex_space_handler(char **current_str, char *args, \
int i, int begining);

// LEXOR UTILS
void				ft_free_parser(void *parser);
void				print_list_parse(t_list *el);
char				*join2current_str(char *current_str, char *add_on);
int					is_special_char(char ch);
int					is_space(char ch);
char				*ft_env_helper(t_env_var *temp_env);
char				*ft_env_path(void);
void				ft_comment_helper(int i, char **args);

//PARSER UTILS
void				ft_free_lex_list_handler(int return_flag);
void				error_fun(t_list **list, t_list **lexor_list);

//UNSET UTILS
int					ft_validity_setter(char **args, int i, int pid);
int					ft_unsetter(char **args, int i, t_list *ptr, int pid);
int					ft_unset_engine(char **args, int i, t_list *ptr, int pid);
int					ft_free_parse_struct(t_parser *parser);

// UTILS
int					ft_strcmp(char *s1, char *s2);
void				ft_free_split(char **split);
int					ft_free_linked_list(t_list **lst, int type, int full);
void				ft_set_global_pwd(char **env);
int					ft_count_arguments(t_list *cmd_list);
int					ft_execve(char **args, pid_t pid);
//GNL
int					get_next_line_prev(int fd, char **line);
char				*get_next_line(int fd);
void				ft_memmove_till_newline(char *l_to_m);
int					ft_return_prep(int bytes, char **tmp, char **line, int fd);
int					ft_check_array_input(char **tmp, char **line, char *buffer, \
int fd);
void				*ft_memmove(void *dest, const void *src, size_t n);
char				*ft_strchr_gnl(const char *s, int c);
char				*ft_strjoin_gnl(char *s1, char *s2, int j);
size_t				ft_strlen_gnl(char *s);

//ft_get_path_linux.c
void				ft_get_path(void);

//GLOBAL UTILS
void				ft_set_lasts(char **args, int pid, int lreturn, int mode);
void				ft_last_arg(char **args, pid_t pid);
void				ft_elem_copy(t_env_var **env_var, t_list **env_cpy, \
t_list **ptr_cpy);
t_list				*ft_copy_env(void);
void				ft_checking(t_env_var **env_var, t_list **ptr_cpy);

//MEMORY UTILS
void				ft_free_secure(void **s);

//pipex utils
void				ft_initialize_fds(int *fd_temp);
int					out_redirect(char *filename, int type);
int					in_redirect(char *filename, int type);
void				ft_infile_handler(int *fd_docks, int *fd_infile, \
t_command *cmd);
void				ft_re_attach_docs(int **fd_docks, int *fd_infile, \
int *fd_outfile);
void				ft_append_handler(int *fd_outfile, t_command *cmd);
void				ft_in_infile_handler(int **fd_docks, int *fd_infile);
void				ft_outfile_handler(int *fd_outfile, t_command *cmd);
int					ft_file_itterator(t_command *cmd, int *fd_outfile, \
int *fd_infile, int **fd_docks);
void				ft_file_checker(t_list **cmd_list, t_pipex **pipex);
void				ft_execute_child(t_list *cmd_list, char **envp, pid_t pid);
void				ft_free_pipex_and_reattach_pipes(t_pipex **pipex);
void				ft_pipe_attachment(t_pipex **pipex);
void				ft_pipex_exit(t_pipex **pipex);
void				ft_here_doc_handler(t_pipex **pipex);

typedef struct s_content
{
	char	**cmd_n_flags;
	char	*path;
	int		index;
}	t_content;

void				print_list_parse(t_list *el); //rm?
void				ft_close_fd(void);
void				ft_free_split(char **split);
void				ft_exit_on_error(t_list **cmd_list, char *error_msg);
void				ft_exit_on_invalid_cmd(char **path_list, t_list **cmd_list, \
t_content *content, t_list *elem);

/*helper.c*/
void				ft_make_cmd_list(char **argv, char **envp, int argc, \
t_list **cmd_list);
char				*ft_get_cmd_path(const char *cmd, char **path_list);
char				**ft_split_path(char **env);
char				*ft_strjoin_with_free(char *s1, char const *s2);
char				*ft_strjoin_with_dfree(char *s1, char *s2);
char				*ft_strjoin_with_scnd_free(char *s1, char *s2);

/*piping.c*/
void				ft_pipex(t_list *cmd_list, char **envp);
void				ft_initialize_fds(int *fd_temp);
void				ft_execute_child_process(t_list *cmd_list, char **envp, \
int *fd, int *fd_stream);
void				ft_execute_parent_process(int *fd, t_list **cmd_list, \
pid_t pid);
void				ft_execute_last_cmd(int *fd, t_list **cmd_list, pid_t pid, \
int *fd_temp);

/*pipex.c*/
void				ft_check_input_file(char *filename, char *outputfile);
void				ft_check_output_file(const char *filename);
char				*ft_check_cmd_path(char **path, char **split, int j, \
const char *cmd);
void				ft_exit_on_error2(char *error_msg);
int					pipex(t_list *cmd, char **envp);

/*ft_heredoc.c*/
void				heredoc_child(int *fd, int *fd_stream, char *stop_name, \
char *keyword);
void				heredoc_parent(int *fd, pid_t pid);
void				ft_signal_setup(void);

#endif
