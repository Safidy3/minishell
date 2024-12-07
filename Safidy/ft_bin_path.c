/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bin_path.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: larakoto < larakoto@student.42antananar    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 11:37:08 by larakoto          #+#    #+#             */
/*   Updated: 2024/12/07 14:27:37 by larakoto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*join_bin_path(char *commands_list, char *bin_path)
{
	char	*temp;
	char	*res;

	temp = ft_strjoin(bin_path, "/");
	ft_free(bin_path);
	if (!temp)
		return (NULL);
	res = ft_strjoin(temp, commands_list);
	if (!res)
		return (NULL);
	return (free(temp), res);
}

char	*get_bin_path(char *command, t_all *all)
{
	char	**bin_paths;
	char	*bin_path;
	char	*result;
	char	*path;
	int		i;


	if (access(command, F_OK | X_OK) == 0)
		return (ft_strdup(command));
	result = NULL;
	path = ft_getenv("PATH", all);
	if (!path)
		return (NULL);
	bin_paths = ft_split(path, ':');
	free(path);
	if (!bin_paths)
		return (NULL);
	i = array_len(bin_paths);
	while (--i >= 0)
	{
		bin_path = join_bin_path(command, bin_paths[i]);
		if (bin_path && access(bin_path, F_OK | X_OK) == 0)
		{
			if (result)
				free(result);
			result = bin_path;
		}
		else
			ft_free(bin_path);
	}
	return (free(bin_paths), result);
}




