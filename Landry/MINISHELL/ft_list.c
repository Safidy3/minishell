/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_list.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: larakoto < larakoto@student.42antananar    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 17:43:52 by larakoto          #+#    #+#             */
/*   Updated: 2024/09/23 14:35:23 by larakoto         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "minishell.h"

void	ft_init_enum(char *content, t_command_list *new)
{
	if (strcmp(content, ">") == 0)
		new->type = R_OUT;
	else if (strcmp(content, "<") == 0)
		new->type = R_IN;
	else if (strcmp(content, "|") == 0)
		new->type = R_PIPE;
	else if (strcmp(content, "<<") == 0)
		new->type = RR_IN;
	else if (strcmp(content, ">>") == 0)
		new->type = RR_OUT;
}
t_command_list	*ft_lstnew(char *content)
{
	t_command_list	*new;

	new = (t_command_list *)malloc(sizeof(t_command_list));
	if (!new)
		return (NULL);
	new->value = content;
	if (strcmp(content, ">") && strcmp(content, "<") && strcmp(content, "|")
		&& strcmp(content, ">>") && strcmp(content, "<<"))
	{
		new->type = STRING;
	}
	else
		ft_init_enum(content,new);
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
	t_command_list *last;

	if (*lst == NULL)
	{
		*lst = new_element;
		return ;
	}
	last = ft_lstlast(*lst);
	last->next = new_element;
}