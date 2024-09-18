/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: safandri <safandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 14:03:23 by safandri          #+#    #+#             */
/*   Updated: 2024/02/28 15:09:58 by safandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	unsigned int	len;
	unsigned int	i;
	unsigned int	start;
	unsigned int	end;

	len = ft_strlen((char *) s1);
	start = 0;
	end = 0;
	i = 0;
	if (*s1 == '\0')
		return (ft_calloc(sizeof(char), 1));
	if (*set == '\0')
		return (ft_strdup(s1));
	while (s1[start] && ft_strchr(set, s1[start]))
		start++;
	if (start == len)
		return (ft_strdup(""));
	i = len;
	while (ft_strchr(set, s1[--i]))
		end++;
	return (ft_substr(s1 + start, 0, len - end - start));
}
