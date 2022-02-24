#include "../incl/minishell.h"

void	heredoc_parent(int *fd, pid_t pid)
{
	waitpid(pid, NULL, 0);
	close(fd[1]);
}

static int	ft_free_writer(char *stop_word, int fd, char **out)
{
	if (stop_word != NULL)
	{
		if (!ft_strcmp(*out, stop_word))
		{
			free(*out);
			return (-1);
		}
	}
	if (write(fd, *out, ft_strlen(*out)) == -1)
	{
		free(*out);
		return (-1);
	}
	if (write(fd, "\n", 1) == -1)
	{
		free(*out);
		return (-1);
	}
	return (0);
}

void	heredoc(char *stop_word, int fd_stream[2], int fd_out, char *keyword)
{
	char	*out;

	dup2(fd_stream[0], STDIN_FILENO);
	dup2(fd_stream[1], STDOUT_FILENO);
	while (1)
	{
		out = readline(keyword);
		if (out == NULL)
			break ;
		if (ft_free_writer(stop_word, fd_out, &out) == -1)
			break ;
		free(out);
	}
	close(fd_out);
	close(fd_stream[0]);
	close(fd_stream[1]);
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
}

void	handle_sigterm_heardoc(int signum)
{
	if (signum == SIGTERM)
	{
		close(STDIN_FILENO);
		close(STDOUT_FILENO);
	}
}

void	heredoc_child(int *fd, int *fd_stream, char *stop_name, char *keyword)
{
	signal(SIGTERM, handle_sigterm_heardoc);
	close(fd[0]);
	heredoc(stop_name, fd_stream, fd[1], keyword);
	exit(0);
}
