/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: safandri <safandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 15:06:33 by safandri          #+#    #+#             */
/*   Updated: 2024/02/28 14:52:41 by safandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

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
