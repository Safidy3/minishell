/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: safandri <safandri@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 12:28:22 by larakoto          #+#    #+#             */
/*   Updated: 2024/12/21 15:03:02 by safandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	ft_sort_list(t_env_list *env)
{
	t_env_list	*tmp;

	tmp = env;
	while (tmp->next)
	{
		if (!ft_strcmp(tmp->value, tmp->next->value))
		{
			ft_swap_string(&tmp->first, &tmp->next->first);
			ft_swap_string(&tmp->second, &tmp->next->second);
			ft_swap_string(&tmp->value, &tmp->next->value);
			tmp = env;
		}
		else
			tmp = tmp->next;
	}
}

void	join_first_second(int flag, t_env_list *new)
{
	char	*tmp;

	if (flag == 1)
	{
		free(new->value);
		tmp = ft_strjoin(new->first, "=");
		if (new->second)
			new->value = ft_strjoin(tmp, new->second);
		else
			new->value = ft_strdup(new->first);
		free(tmp);
	}
}

void	int_lst_env(t_env_list **list, char **envp)
{
	int		i;
	int		j;
	char	*shlvl_char;

	i = 0;
	j = 0;
	while (envp[i])
	{
		if (!ft_strncmp(envp[i], "SHLVL", 5))
		{
			while (!ft_isdigit(envp[i][j]))
				j++;
			shlvl_char = ft_itoa(ft_atoi(&envp[i][j]) + 1);
			ft_lstadd_back(list, ft_lstnew_init(
					ft_strjoin("SHLVL=", shlvl_char), 0));
			free(shlvl_char);
		}
		else
			ft_lstadd_back(list, ft_lstnew_init(envp[i], 0));
		i++;
	}
}

char	**list_to_array(t_env_list *env)
{
	int			i;
	char		**dir;
	char		*cha_join;
	t_env_list	*tmp;

	i = 0;
	tmp = env;
	while (tmp)
	{
		tmp = tmp->next;
		i++;
	}
	dir = (char **)ft_calloc(sizeof(char *), (i + 1));
	i = -1;
	while (env)
	{
		cha_join = ft_strjoin((env->first), "=");
		dir[++i] = ft_strjoin(cha_join, env->second);
		free(cha_join);
		env = env->next;
	}
	return (dir);
}

int	ft_print_env(t_env_list *env)
{
	t_env_list	*tmp;

	tmp = env;
	while (tmp)
	{
		if (tmp->second)
			printf("%s=%s\n", tmp->first, tmp->second);
		tmp = tmp->next;
	}
	return (0);
}
