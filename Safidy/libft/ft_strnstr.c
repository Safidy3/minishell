/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: safandri <safandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 14:51:29 by safandri          #+#    #+#             */
/*   Updated: 2024/02/28 14:53:53 by safandri         ###   ########.fr       */
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
