/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_init_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdragomi <vdragomi@students.42wolfsburg.de +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/24 13:28:43 by vdragomi          #+#    #+#             */
/*   Updated: 2022/02/24 18:20:32 by vdragomi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

void	ft_symlink_initiate_free(struct stat **buf, \
	char **path_substr, char **path_substr_free, int mode)
{
	if (mode == 1)
	{
		ft_free_secure((void *)buf);
		ft_free_secure((void *)path_substr);
		ft_free_secure((void *)path_substr_free);
	}
	else if (mode == 2)
	{
		ft_free_secure((void *)buf);
		ft_free_secure((void *)path_substr);
	}
}
