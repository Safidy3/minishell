/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: safandri <safandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 15:01:46 by safandri          #+#    #+#             */
/*   Updated: 2024/05/23 01:57:30 by safandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>

int	ft_atoi(const char *str)
{
	int	signe;
	int	result;

	signe = 1;
	result = 0;
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '+' || *str == '-')
		if (*str++ == '-')
			signe *= -1;
	while (*str >= '0' && *str <= '9')
		result = result * 10 + (*str++ - 48);
	return (result * signe);
}

long int	ft_atoi_long(const char *str)
{
	long int	signe;
	long int	result;

	signe = 1;
	result = 0;
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '+' || *str == '-')
		if (*str++ == '-')
			signe *= -1;
	while (*str >= '0' && *str <= '9')
		result = result * 10 + (*str++ - 48);
	return (result * signe);
}
