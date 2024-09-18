/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: safandri <safandri@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 11:11:01 by safandri          #+#    #+#             */
/*   Updated: 2024/08/06 10:06:05 by safandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

char	*ft_join_and_free(char *buff_next, char *buff_read)
{
	char	*res;

	if (!buff_next)
		return (ft_substr(buff_read, 0, ft_strlen(buff_read) + 1));
	res = ft_strjoin(buff_next, buff_read);
	return (free(buff_next), res);
}

char	*ft_remove_previous(char *buff_next)
{
	char	*res;
	int		i;

	i = 0;
	if (!buff_next[i])
		return (free(buff_next), NULL);
	while (buff_next[i] && buff_next[i] != '\n')
		i++;
	i++;
	res = ft_substr(buff_next, i, ft_strlen(buff_next) - i);
	if (!res)
		return (free(buff_next), NULL);
	return (free(buff_next), res);
}

char	*ft_get_line(char *buff_next)
{
	char	*line;
	int		i;

	i = 0;
	if (buff_next[i] == '\0')
		return (NULL);
	while (buff_next[i] && buff_next[i] != '\n')
		i++;
	line = ft_substr(buff_next, 0, ++i);
	return (line);
}

char	*ft_read_buff(int fd, char *buff_next)
{
	char	*buff_read;
	int		byte_read;

	buff_read = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buff_read)
		return (free(buff_next), NULL);
	byte_read = -1;
	while (++byte_read < BUFFER_SIZE)
		buff_read[byte_read] = 0;
	byte_read = 1;
	while (byte_read > 0 && !ft_strchr(buff_read, '\n'))
	{
		byte_read = read(fd, buff_read, BUFFER_SIZE);
		if (byte_read == -1)
			return (free(buff_read), free(buff_next), NULL);
		buff_read[byte_read] = 0;
		buff_next = ft_join_and_free(buff_next, buff_read);
	}
	return (free(buff_read), buff_next);
}

char	*get_next_line(int fd)
{
	static char	*buff_next[1024];
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	buff_next[fd] = ft_read_buff(fd, buff_next[fd]);
	if (!buff_next[fd])
		return (NULL);
	line = ft_get_line(buff_next[fd]);
	buff_next[fd] = ft_remove_previous(buff_next[fd]);
	return (line);
}
