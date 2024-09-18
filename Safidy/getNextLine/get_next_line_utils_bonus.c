/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: safandri <safandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 11:15:08 by safandri          #+#    #+#             */
/*   Updated: 2024/07/27 13:58:55 by safandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	len1;
	size_t	len2;
	char	*res;

	len1 = 0;
	len2 = 0;
	while (s1[len1])
		len1++;
	while (s2[len2])
		len2++;
	res = (char *)malloc(sizeof(char) * (len1 + len2 + 1));
	if (res == NULL)
		return (NULL);
	while (*s1)
		*res++ = *s1++;
	while (*s2)
		*res++ = *s2++;
	*res = '\0';
	return (res - (len1 + len2));
}

char	*ft_strdup(const char *src)
{
	size_t			src_size;
	unsigned char	*copy;

	src_size = 0;
	while (src[src_size])
		src_size++;
	copy = (unsigned char *)malloc(src_size + 1);
	if (copy == NULL)
		return (NULL);
	src_size = -1;
	while (src[++src_size])
		copy[src_size] = src[src_size];
	copy[src_size] = '\0';
	return ((char *) copy);
}

size_t	ft_strlen(const char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

char	*ft_substr(char const *str, unsigned int start, size_t len)
{
	char	*res;
	size_t	i;

	i = 0;
	if (*str == '\0' || start >= ft_strlen(str))
		return (ft_strdup("\0"));
	if (len > ft_strlen(str))
		return (ft_strdup(str + start));
	res = (char *)malloc(len + 1);
	if (!res)
		return (NULL);
	while (str[start] && i < len)
		res[i++] = str[start++];
	res[i] = '\0';
	return (res);
}

char	*ft_strchr(const char *s, int c)
{
	while (*s != '\0' && *s != (char) c)
		s++;
	if (*s == (char) c)
		return ((char *) s);
	return (NULL);
}
