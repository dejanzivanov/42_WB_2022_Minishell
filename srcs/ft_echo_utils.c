#include "../incl/minishell.h"

void	echo_print(char **str, int starter, int size, int flag)
{
	int	i;

	i = starter;
	while (i < size)
	{
		write(1, str[i], ft_strlen(str[i]));
		if (i != size - 1)
			write(1, " ", 1);
		i++;
	}
	if (flag == 0)
	{
		write(1, "\n", 1);
	}
}

int	echo_flag(char *str)
{
	int	j;
	int	len;

	len = ft_strlen(str);
	if ((str[0] == '-') && len >= 2)
	{
		if (str[1] == 'n' && len >= 2)
		{
			j = 1;
			while (j < len && str[j] == 'n')
			{
				j++;
			}
			if (j == len)
				return (1);
			else
				return (0);
		}
		else
			return (0);
	}
	else
		return (0);
}

void	ft_echo_printer(char **args, int len, int flag)
{
	int	i;
	int	temp;

	i = 1;
	while (args[i])
	{
		temp = echo_flag(args[i]);
		flag += temp;
		if (flag == 0 || temp == 0)
			break ;
		else
			i++;
	}
	echo_print(args, i, len, flag);
}
