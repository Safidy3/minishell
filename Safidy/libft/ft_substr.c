/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: safandri <safandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 14:54:03 by safandri          #+#    #+#             */
/*   Updated: 2024/03/10 15:47:18 by safandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *str, unsigned int start, size_t len)
{
	char	*res;
	size_t	i;

	i = 0;
	if (*str == '\0' || start >= ft_strlen(str))
		return (ft_calloc(sizeof(char), 1));
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
