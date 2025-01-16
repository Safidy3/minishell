/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: safandri <safandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 15:04:41 by safandri          #+#    #+#             */
/*   Updated: 2024/02/28 14:52:06 by safandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *src, int c, size_t n)
{
	size_t				i;
	const unsigned char	*s;

	s = (const unsigned char *) src;
	i = 0;
	while (i < n)
	{
		if (*s == (unsigned char) c)
			return ((void *) s);
		i++;
		s++;
	}
	return (NULL);
}
