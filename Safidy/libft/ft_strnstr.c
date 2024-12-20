/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: safandri <safandri@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 14:51:29 by safandri          #+#    #+#             */
/*   Updated: 2024/12/20 14:52:50 by safandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *src, const char *sub, size_t len)
{
	size_t	i;
	size_t	j;

	j = 0;
	i = -1;
	while (sub[++i])
		;
	if (i == 0)
		return ((char *) src);
	while (src[j] && j < len)
	{
		i = 0;
		while (src[j + i] == sub[i]
			&& sub[i] != '\0'
			&& (i + j) < len)
			i++;
		if (sub[i] == '\0')
			return ((char *) src + j);
		j++;
	}
	return (NULL);
}

char	*ft_strstr(char *str, char *sub_str)
{
	char *h = str;
	char *n = sub_str;

    if (*sub_str == '\0')
        return (str);
    while (*str != '\0')
	{
        if (*str == *sub_str)
		{
            h = str;
            n = sub_str;
            while (*n != '\0' && *h == *n)
			{
                h++;
                n++;
            }
            if (*n == '\0')
                return (str);
        }
        str++;
    }
    return (NULL); 
}
