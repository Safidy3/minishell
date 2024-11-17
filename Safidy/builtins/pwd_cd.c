/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd_cd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: safandri <safandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 15:57:34 by larakoto          #+#    #+#             */
/*   Updated: 2024/11/16 17:49:13 by safandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void ft_pwd()
{
	char *s;
	s = getcwd(NULL, 0);
	printf("%s\n",s);
	free(s);
}

char	*manage_home_path(char *path, t_all *all)
{
	t_env_list	*env;

	env = all->env_list;
	if (!ft_strcmp(path, "~") || !path)
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

void	ft_cd(char *path, t_all *all)
{
	t_env_list	*env;
	char		*o_path;
	char		*n_path;

	path = manage_home_path(path, all);
	o_path = getcwd(NULL,0);
	if (!o_path)
		return (perror("getcwd"));
	if (chdir(path) != 0)
		return (free(o_path), perror(path));
	n_path = getcwd(NULL,0);
	if (!n_path)
		return (perror("getcwd"));
	env = all->env_list;
	while (env)
	{
		if (strcmp(env->first , "PWD") == 0)
			env->second = n_path;
		else if (strcmp(env->first , "OLDPWD") == 0)
			env->second = o_path;
		env = env->next;
	}
}
