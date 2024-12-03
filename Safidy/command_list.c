#include "minishell.h"


void	ft_lst_add_back_cmd(t_cmd_list **lst, t_cmd_list *new)
{
	t_cmd_list	*temp;

	if (!lst || !new)
		return ;
	if (!(*lst))
	{
		*lst = new;
		return ;
	}
	temp = *lst;
	while (temp->next)
		temp = temp->next;
	temp->next = new;
}

void	ft_lst_cmd_delone(t_cmd_list *lst, void (*del)(char **))
{
	if (!lst || !del)
		return ;
	(del)(lst->cmd_arr);
	free(lst);
}

void	ft_lst_cmd_clear(t_cmd_list **lst, void (*del)(char **))
{
	t_cmd_list	*tmp;

	if (!lst || !del)
		return ;
	while (*lst)
	{
		tmp = (*lst)->next;
		ft_lst_cmd_delone(*lst, del);
		(*lst) = tmp;
	}
}

void	ft_lst_cmd_iter(t_cmd_list *lst, void (*f)(char **))
{
	if (!lst || !f)
		return ;
	while (lst)
	{
		f(lst->cmd_arr);
		lst = lst->next;
	}
}

t_cmd_list	*ft_lst_new_cmd(char **cmd_arr)
{
	t_cmd_list	*new;

	new = (t_cmd_list *)malloc(sizeof(t_cmd_list));
	if (!new)
		return (NULL);
	new->cmd_arr = cmd_arr;
	new->in_quote = NULL;
	new->next = NULL;
	return (new);
}

int	ft_lst_cmd_size(t_cmd_list *lst)
{
	size_t	i;

	i = 0;
	while (lst)
	{
		lst = lst->next;
		i++;
	}
	return (i);
}
