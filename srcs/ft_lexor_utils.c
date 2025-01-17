/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lexor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzivanov <dzivanov@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/21 23:08:47 by dzivanov          #+#    #+#             */
/*   Updated: 2022/02/21 23:13:51 by dzivanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

/*
**	@brief Current string being appended by new string
**	@param char *current_str, char *add_on. Addon is a string being appened
**	to a current string.
**	@return Returns character pointer generated by combinining two strings.
**	@todo /
*/

char	*join2current_str(char *current_str, char *add_on)
{
	char	*temp;

	if (current_str)
		temp = ft_strjoin(current_str, add_on);
	else
		temp = ft_strdup(add_on);
	free(current_str);
	current_str = NULL;
	free(add_on);
	add_on = NULL;
	return (temp);
}

/*
**	@brief Prints element of the linked list and its datatype.
**	@param Linked List t_word.
**	@return /
**	@todo /
*/

void	print_element(void *input)
{
	t_word	*word;

	word = (t_word *)input;
	if (word->type == FT_STRING)
		printf("string: %s\n", word->address);
	else if (word->type == FT_SPECIAL_CHAR_STRING)
		printf("special char string: %s\n", word->address);
}

void	print_list(t_list *el)
{
	ft_lstiter(el, print_element);
}

/*
**	@brief Prints element of the linked list and its datatype.
**	@param t_list **list, char *str. Where str is the curent STRING or
**	CHARACTER(it needs to be compared with type)
**	@return /
**	@todo /
*/

void	add_string(t_list **list, char	*str)
{
	t_word	*word;
	t_list	*new_el;

	if (str == NULL)
		return ;
	word = (t_word *)malloc(sizeof(t_word));
	word->type = FT_STRING;
	word->address = ft_strdup(str);
	new_el = ft_lstnew((void *)word);
	ft_lstadd_back(list, new_el);
}
