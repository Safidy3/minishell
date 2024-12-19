/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env_getter.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: safandri <safandri@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 14:44:55 by safandri          #+#    #+#             */
/*   Updated: 2024/12/19 14:56:23 by safandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*ft_getenv(char *env_var, t_all *all)
{
	t_env_list	*tmp;
	char		*res;

	tmp = all->env_list;
	res = NULL;
	while (tmp)
	{
		if (!ft_strcmp(tmp->first, env_var) && tmp->second)
		{
			res = ft_strdup(tmp->second);
			break ;
		}
		tmp = tmp->next;
	}
	return (res);
}

char	*get_env_name(char *s)
{
	char	*var_name;
	size_t	len;
	size_t	i;

	if (*s != '$')
		return (NULL);
	s++;
	i = 0;
	len = -1;
	while (s[++len] && ft_isdigit(s[len]))
		;
	while (s[++len] && (s[len] == '_' || ft_isalnum(s[len])))
		i++;
	var_name = (char *)malloc(sizeof(char) * (len + 1));
	if (!var_name)
		return (NULL);
	var_name[len] = '\0';
	i = -1;
	while (s[++i] && i < len)
		var_name[i] = s[i];
	return (var_name);
}

char	*get_env_value(char *var_name, t_all *all)
{
	char	*var_value;

	var_value = NULL;
	if (var_name)
		var_value = ft_getenv(var_name, all);
	return (var_value);
}

void	manage_env_var(char **dst, char **s, t_all *all)
{
	char	*env_name;
	char	*env_val;
	char	*temp;

	env_name = get_env_name(*s);
	env_val = get_env_value(env_name, all);
	if (env_val)
	{
		temp = *dst;
		*dst = ft_strjoin(*dst, env_val);
		free(temp);
		free(env_val);
	}
	else if (ft_isdigit(*(*s + 1)))
	{
		temp = *dst;
		*dst = ft_strjoin(*dst, &env_name[1]);
		free(temp);
	}
	while (++(*s) && (**s == '_' || ft_isalnum(**s)))
		;
	ft_free(env_name);
}
