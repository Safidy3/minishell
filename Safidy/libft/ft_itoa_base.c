/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: safandri <safandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/01 15:30:08 by safandri          #+#    #+#             */
/*   Updated: 2024/04/01 16:45:20 by safandri         ###   ########.fr       */
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

static char	*ft_convert_nbr(int nbr, char *res, char *base, int len_base)
{
	char	*tmp;
	int		i;

	i = 0;
	while (nbr)
	{
		res = ft_join_str_char(res, base[nbr % len_base]);
		nbr = nbr / len_base;
		i++;
	}
	tmp = res;
	res = ft_strrev(res);
	return (free(tmp), res);
}

char	*ft_itoa_base(long int nbr, char *base)
{
	int		len_base;
	char	*res;

	len_base = 0;
	res = malloc(2);
	res[0] = 0;
	res[1] = 0;
	len_base = ft_check_base(base);
	if (!len_base)
		return (free(res), NULL);
	if (nbr == 0)
	{
		res[0] = '0';
		return (res);
	}
	if (nbr < 0)
	{
		nbr = -nbr;
		res[0] = '-';
	}
	res = ft_convert_nbr(nbr, res, base, len_base);
	return (res);
}
