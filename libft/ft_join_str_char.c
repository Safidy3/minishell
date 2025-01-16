/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: safandri <safandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/29 11:07:23 by safandri          #+#    #+#             */
/*   Updated: 2024/03/29 11:08:13 by safandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_join_str_char(char *s, char c)
{
	char	*res;
	int		i;

	i = 0;
	while (s[i])
		i++;
	res = (char *)malloc(sizeof(char) * (i + 2));
	if (!res)
		return (free(s), NULL);
	i = -1;
	while (s[++i])
		res[i] = s[i];
	res[i++] = c;
	res[i] = 0;
	return (free(s), res);
}
