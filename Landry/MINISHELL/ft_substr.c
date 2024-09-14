/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: larakoto <larakoto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/02 10:28:54 by larakoto          #+#    #+#             */
/*   Updated: 2024/07/23 16:46:27 by larakoto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

char	*ft_strdup(const char *s)
{
	size_t	i;
	char	*d;

	d = (char *)malloc(sizeof(char) * (ft_strlen(s) + 1));
	i = 0;
	if (d == NULL)
	{
		return (NULL);
	}
	while (s[i] != '\0')
	{
		d[i] = s [i];
		i++;
	}
	d[i] = '\0';
	return (d);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*str_sub;
	size_t	i;
	size_t	len_str_sub;
	size_t	s_len;

	s_len = ft_strlen(s);
	len_str_sub = s_len - start;
	if (!s)
		return (NULL);
	if (start >= (unsigned int) s_len)
		return (ft_strdup(""));
	if (len_str_sub > len)
		len_str_sub = len;
	str_sub = malloc ((len_str_sub + 1));
	if (!str_sub)
		return (NULL);
	i = 0;
	while (i < len_str_sub && s[start])
	{
		str_sub[i] = s[start];
		i++;
		start++;
	}
	str_sub[i] = '\0';
	return (str_sub);
}
