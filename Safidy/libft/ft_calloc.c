/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: safandri <safandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 14:51:48 by safandri          #+#    #+#             */
/*   Updated: 2024/02/28 17:40:41 by safandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	unsigned char	*ptr;
	size_t			i;

	i = -1;
	ptr = malloc(nmemb * size);
	if (ptr == NULL)
		return (NULL);
	while (++i < nmemb * size)
		ptr[i] = 0;
	return (ptr);
}
