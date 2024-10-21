/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: larakoto <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 16:33:16 by larakoto          #+#    #+#             */
/*   Updated: 2024/10/19 16:33:19 by larakoto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_free_tmp(t_command_list *tmp)
{
	free(tmp->first);
	free(tmp->second);
	free(tmp->value);
	free(tmp);
}

void	ft_unset(t_command_list **env, char **commande)
{
	t_command_list	*tmp;
	t_command_list	*prev;
	int				i;

	i = 0;
	while (commande[i])
	{
		prev = NULL;
		tmp = (*env);
		while (tmp)
		{
			if (ft_strcmp(tmp->first, commande[i]) == 0)
			{
				if (prev == NULL)
					*env = tmp->next;
				else
					prev->next = tmp->next;
				ft_free_tmp(tmp);
				break ;
			}
			prev = tmp;
			tmp = tmp->next;
		}
		i++;
	}
}
