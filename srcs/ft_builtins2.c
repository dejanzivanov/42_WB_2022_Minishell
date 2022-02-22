#include "../incl/minishell.h"

static int	ft_cd_free(char **args, char **current_path, char **abs_path, int pid)
{
		free(g_access.last_return);
		g_access.last_return = ft_itoa(1);
		if (current_path != NULL)
			free(current_path);
		if (abs_path != NULL)
			free(abs_path);
		ft_last_arg(args, pid);
		return (1);
}

static int	ft_err_cd(char **args, int pid)
{
	if (args[1] == NULL)
	{
		if (pid == 0)
			write(2, "cd without an argument not permitted in this minishell.\n", 56);
		free(g_access.last_return);
		g_access.last_return = ft_itoa(1);
		ft_last_arg(args, pid);
		return(1);
	}
	return (0);
}

static int	ft_dir_checker(char **args, DIR **dir, char **abs_path, int pid)
{
	if (*dir == NULL)
	{
		if (g_access.last_return != NULL)
			free(g_access.last_return);
		g_access.last_return = ft_itoa(1);
		if (pid == 0)
		{
			write(2, "minishell: cd: ", 15);
			write(2, args[1], ft_strlen(args[1]));
			perror(" ");
			
		}
		if (*abs_path != NULL)
			free(*abs_path);
		ft_last_arg(args, pid);
		return (1);
	}
	else
		closedir(*dir);
	return (0);
}
/**
	 @brief Bultin command: change directory.
	 @param args List of args. args[0] is "cd". args[1] is the directory.
	 @return Always returns 1, to continue executing.
 */
int minishell_cd(char **args, pid_t pid)
{
	char *abs_path;
	int sym_check;
	char *current_path;
	DIR		*dir;

	ft_set_lasts(args, pid, 0, 2);
	if (ft_err_cd(args, pid) == 1)
		return (1);
	abs_path = NULL;
	current_path = NULL;
	ft_rtoa_path(args[1], &abs_path);
	dir = opendir(abs_path);
	if (ft_dir_checker(args, &dir, &abs_path, pid) == 1)
		return (1);
	ft_set_current_path(&current_path);
	sym_check = ft_check_symlink(abs_path, args[1], pid);
	if (sym_check == -1)
		return (ft_cd_free(args, &current_path, &abs_path, pid));
	else if (sym_check == SYMLINK)
		ft_cd_simlink(abs_path, current_path, pid, 1);
	else if (sym_check == NOT_SYMLINK)
		ft_cd_simlink(abs_path, current_path, pid, 2);
	ft_free_secure((void *) &abs_path);
	ft_free_secure((void *) &current_path);
	// if (abs_path != NULL)
	// 	free(abs_path);
	// if (current_path != NULL)
	// 	free(current_path);
	ft_last_arg(args, pid);
	return(1);
}

/**
	 @brief Builtin command: echo.
	 @param args List of args.	Not examined.
	 @return Always returns 1, to continue execution.
	 @todo edge case fetch -nnnnn -nnnn test / edge case echo $_ -> $_ must be implemented alongside history; implemented -nnnnnnnn and also implemented -nnnnn -nnnn V.
	 @todo fix flag -n try printing
*/
int	minishell_echo(char **args, pid_t pid)
{
	int i;
	int flag;
	int temp;
	int len;

	if (pid == 0)
	{
		len = 0;
		while (args[len] != NULL)
		{
			len++;
		}
		flag = 0;
		if (args[1] == NULL)
			write(1, "\n", 1);
		else
		{
			i = 1;
			while (args[i]) //go through all arguments
			{
				temp = echo_flag(args[i]); //checks for -n / -nnnnnnn and returns 1 if so or 0 if the format is not -n / -nnnnn
				flag += temp; //flag keeps growing by temp
				if (flag == 0 || temp == 0)
					break;
				else
					i++;
			}
			echo_print(args, i, len, flag); //based on the flag, we either print with or without a \n
		}
	}
	ft_last_arg(args, pid);
	free(g_access.last_return);
	g_access.last_return = ft_itoa(0);
	return (1);
}
