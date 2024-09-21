/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_list.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: larakoto < larakoto@student.42antananar    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 17:43:52 by larakoto          #+#    #+#             */
/*   Updated: 2024/09/20 17:44:44 by larakoto         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "minishell.h"

t_command_list	*ft_lstnew(char *content)
{
	t_command_list	*new;

	new = (t_command_list *)malloc(sizeof(t_command_list));
	if (!new)
		return (NULL);
	new->value = content;
	new->next = NULL;
	return (new);
}
t_command_list	*ft_lstlast(t_command_list *lst)
{
	t_command_list	*tmp;

	while ((lst->next) != NULL)
		lst = lst->next;
	tmp = lst;
	return (tmp);
}

void	ft_lstadd_back(t_command_list **lst, t_command_list *new_element)
{
	t_command_list	*last;

	if (*lst == NULL)
	{
		*lst = new_element;
		return ;
	}
	last = ft_lstlast(*lst);
	last->next = new_element;
}