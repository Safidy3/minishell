/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd_cd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: safandri <safandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 15:57:34 by larakoto          #+#    #+#             */
/*   Updated: 2024/11/29 15:20:14 by safandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int ft_pwd()
{
	char *s;
	s = getcwd(NULL, 0);
	printf("%s\n",s);
	free(s);
	return (0);
}

char	*manage_home_path(char *path, t_all *all)
{
	t_env_list	*env;

	env = all->env_list;
	if (!path || !ft_strcmp(path, "~"))
	{
		while (env)
		{
			if (strcmp(env->first, "HOME") == 0)
				return (env->second);
			env = env->next;
		}
	}
	return (path);
}

int	ft_cd(char *path, t_all *all)
{
	t_env_list	*env;
	char		*o_path;
	char		*n_path;

	path = manage_home_path(path, all);
	o_path = getcwd(NULL, 0);
	if (!o_path)
		return (perror("getcwd"), 2);
	if (chdir(path) == -1)
		return (free(o_path), perror(path), 1);
	n_path = getcwd(NULL,0);
	if (!n_path)
		return (perror("getcwd"), 2);
	env = all->env_list;
	while (env)
	{
		if (strcmp(env->first , "PWD") == 0)
			env->second = n_path;
		else if (strcmp(env->first , "OLDPWD") == 0)
			env->second = o_path;
		env = env->next;
	}
	return (0);
}
