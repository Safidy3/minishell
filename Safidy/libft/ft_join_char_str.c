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

char	*ft_join_char_str(char c, char *s)
{
	char	*res;
	int		i;

	i = 0;
	while (s[i])
		i++;
	res = (char *)malloc(sizeof(char) * (i + 2));
	if (!res)
		return (free(s), NULL);
	i = 0;
	res[i] = c;
	while (s[++i])
		res[i] = s[i - 1];
	res[i] = s[i - 1];
	res[++i] = 0;
	return (free(s), res);
}
