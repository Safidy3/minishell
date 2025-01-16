/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: safandri <safandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 15:06:49 by safandri          #+#    #+#             */
/*   Updated: 2024/02/28 14:53:25 by safandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	length;
	size_t	i;

	length = 0;
	while (dst[length] != '\0' && length < dstsize)
		length++;
	i = length;
	while (src[length - i] != '\0' && length + 1 < dstsize)
	{
		dst[length] = src[length - i];
		length++;
	}
	if (i < dstsize)
		dst[length] = '\0';
	length = 0;
	while (src[length])
		length++;
	return (i + length);
}
