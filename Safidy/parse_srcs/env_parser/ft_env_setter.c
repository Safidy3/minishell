/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env_setter.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: safandri <safandri@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 14:51:20 by safandri          #+#    #+#             */
/*   Updated: 2024/12/20 14:51:03 by safandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	initiate_dst(char **dst, char *s)
{
	if (!ft_strchr(s, '$') || !ft_strstr(s, "<<"))
		return (0);
	*dst = ft_strdup("");
	return (1);
}

char	*copy_char(char *dest, char c)
{
	char	*res;
	int		len;

	if (!dest)
		return (NULL);
	len = ft_strlen(dest);
	res = malloc(sizeof(char) * (len + 2));
	if (!res)
		return (NULL);
	ft_strlcpy(res, dest, len + 1);
	res[len] = c;
	res[len + 1] = '\0';
	return (free(dest), res);
}

void	append_env_value_quote(char **dst, char **s, t_all *all)
{
	char	*temp;
	char	*exit_stat;

	if (**s == '$' && ft_isalnum(*(*s + 1)))
		manage_env_var(dst, s, all);
	else if (**s == '$' && *(*s + 1) == '?')
	{
		exit_stat = ft_itoa(all->exit_status);
		temp = *dst;
		*dst = ft_strjoin(*dst, exit_stat);
		free(temp);
		free(exit_stat);
		*s += 2;
	}
	else if (**s == '$' && (ft_isspace(*(*s + 1))
			|| *(*s + 1) == '\'' || *(*s + 1) == '"'
			|| !(*(*s + 1)) || !ft_isalnum(*(*s + 2))))
		*dst = copy_char(*dst, *(*s)++);
	else
		*dst = copy_char(*dst, *(*s)++);
}

void	append_env_value(char **dst, char **s, t_all *all)
{
	char	*temp;
	char	*exit_stat;

	if (**s == '$' && ft_isalnum(*(*s + 1)))
		manage_env_var(dst, s, all);
	else if (**s == '$' && *(*s + 1) == '?')
	{
		exit_stat = ft_itoa(all->exit_status);
		temp = *dst;
		*dst = ft_strjoin(*dst, exit_stat);
		free(temp);
		free(exit_stat);
		*s += 2;
	}
	else if (**s == '$' && (*(*s + 1) == '\'' || *(*s + 1) == '"'))
		*s += 1;
	else
		*dst = copy_char(*dst, *(*s)++);
}

char	*expand_path(char **s, char **dst, t_all *all)
{
	char	*temp;
	char	*home_path;

	home_path = ft_getenv("HOME", all);
	if (!home_path)
		return (*dst);
	temp = *dst;
	*dst = ft_strjoin(*dst, home_path);
	free(temp);
	(*s)++;
	return (*dst);
}
