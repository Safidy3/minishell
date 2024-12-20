/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: safandri <safandri@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 10:35:42 by larakoto          #+#    #+#             */
/*   Updated: 2024/12/20 14:02:15 by safandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_free(void *s)
{
	if (s)
	{
		free(s);
		s = NULL;
	}
}

void	print_s(void *s)
{
	printf("%s\n", (char *) s);
}

void	free_all_struct(t_all *all)
{
	free_list(all->command_list);
	free_split(all->env_arr);
	ft_free_env_list(all->env_list);
	free(all);
}
