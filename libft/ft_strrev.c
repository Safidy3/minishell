/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: safandri <safandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/01 15:20:52 by safandri          #+#    #+#             */
/*   Updated: 2024/04/01 16:10:01 by safandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrev(const char *s)
{
	int		i;
	int		j;
	char	*res;

	j = 0;
	i = ft_strlen(s);
	res = (char *)malloc(sizeof(char) * i);
	while (--i >= 0)
	{
		res[j] = s[i];
		j++;
	}
	res[j] = 0;
	return (res);
}
