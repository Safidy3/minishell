/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nbr_base_len.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: safandri <safandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 10:14:05 by safandri          #+#    #+#             */
/*   Updated: 2024/04/18 10:16:26 by safandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_nbr_base_len(long long int nbr, char *base)
{
	size_t	size_base;
	size_t	i;

	i = 0;
	size_base = 0;
	while (base[size_base])
		size_base++;
	while (nbr)
	{
		nbr = nbr / size_base;
		i++;
	}
	return (i);
}
