/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isspace.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: safandri <safandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 16:16:56 by safandri          #+#    #+#             */
/*   Updated: 2024/12/06 15:39:45 by safandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int ft_isspace(char c)
{
    if (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r')
        return (1);
    return (0);
}

int ft_isallspace(char *s)
{
    int i;

    i = -1;
    if (s[0] == '\0')
        return (0);
    while (s[++i] && ft_isspace(s[i]))
        i++;
    if (s[i] && !ft_isspace(s[i]))
        return (0);
    return (1);
}