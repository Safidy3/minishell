/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa_unsigned_base.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: safandri <safandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 10:13:56 by safandri          #+#    #+#             */
/*   Updated: 2024/04/18 10:14:42 by safandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_check_base(char *base)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	if (base[0] == '\0' || base[1] == '\0')
		return (0);
	while (base[i])
	{
		j = i + 1;
		if (base[i] == '+' || base[i] == '-')
			return (0);
		if (base[i] < 32 || base[i] > 126)
			return (0);
		while (base[j])
		{
			if (base[i] == base[j])
				return (0);
			j++;
		}
		i++;
	}
	return (i);
}

char	*ft_itoa_unsigned_base(unsigned int nbr, char *base)
{
	char	*res;
	int		size_base;

	size_base = 0;
	res = ft_calloc(1, 1);
	size_base = ft_check_base(base);
	if (!size_base)
		return (free(res), NULL);
	while (nbr)
	{
		res = ft_join_char_str(base[nbr % size_base], res);
		if (!res)
			return (NULL);
		nbr = nbr / size_base;
	}
	return (res);
}
