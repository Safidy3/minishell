/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: safandri <safandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 16:33:16 by larakoto          #+#    #+#             */
/*   Updated: 2024/11/16 16:53:17 by safandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_free_tmp(t_env_list *tmp)
{
	free(tmp->first);
	free(tmp->second);
	free(tmp->value);
	free(tmp);
}

void	ft_unset(t_env_list **env, char **commande)
{
	t_env_list	*tmp;
	t_env_list	*prev;
	int			i;

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
