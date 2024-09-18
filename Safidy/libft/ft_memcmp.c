/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: safandri <safandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 15:05:00 by safandri          #+#    #+#             */
/*   Updated: 2024/02/28 14:52:07 by safandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	unsigned char	*t1;
	unsigned char	*t2;
	size_t			i;

	i = -1;
	t1 = (unsigned char *) s1;
	t2 = (unsigned char *) s2;
	while (++i < n)
	{
		if (t1[i] - t2[i] < 0)
			return (-1);
		else if (t1[i] - t2[i] > 0)
			return (1);
	}
	return (0);
}
