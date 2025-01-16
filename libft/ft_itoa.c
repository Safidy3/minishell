/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: safandri <safandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 14:49:55 by safandri          #+#    #+#             */
/*   Updated: 2024/02/28 17:43:40 by safandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_num_len(int n)
{
	int		len;

	len = 1;
	while (n / 10 != 0)
	{
		n /= 10;
		len++;
	}
	return (len);
}

static char	*ft_result_size(int n)
{
	char	*res;

	if (n < 0)
		res = (char *)malloc(sizeof(char) *(ft_num_len(n) + 2));
	else
		res = (char *)malloc(sizeof(char) * (ft_num_len(n) + 1));
	if (!res)
		return (NULL);
	return (res);
}

char	*ft_itoa(int n)
{
	char	*res;
	int		i;
	int		len;

	len = ft_num_len(n);
	i = 0;
	if (n == -2147483648)
		return (ft_strdup("-2147483648"));
	if (n == 2147483647)
		return (ft_strdup("2147483647"));
	res = ft_result_size(n);
	if (!res)
		return (NULL);
	if (n < 0)
	{
		n *= -1;
		res[0] = '-';
		i++;
	}
	while (--len >= 0)
		res[i++] = (char)((n / ft_power(10, len))
				- (n / ft_power(10, len + 1)) * 10 + 48);
	res[i] = '\0';
	return (res);
}
