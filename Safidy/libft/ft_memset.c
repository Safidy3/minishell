/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: safandri <safandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 15:06:16 by safandri          #+#    #+#             */
/*   Updated: 2024/02/28 14:52:11 by safandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *ptr, int x, size_t n)
{
	unsigned char	*p;
	unsigned char	value;
	size_t			i;

	p = ptr;
	i = -1;
	value = (unsigned char) x;
	while (++i < n)
		p[i] = value;
	return (ptr);
}
