/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd_cd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: larakoto < larakoto@student.42antananar    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 15:57:34 by larakoto          #+#    #+#             */
/*   Updated: 2024/12/05 11:37:36 by larakoto         ###   ########.fr       */
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
			if (!strcmp(env->first, "HOME") && env->second)
				return (env->second);
			env = env->next;
		}
		return (ft_putstr_fd("bash: cd: HOME not set\n", 2), path);
	}
	else
		return (path);
}

int	ft_cd(char **t_path, t_all *all)
{
	t_env_list	*env;
	char		*o_path;
	char		*n_path;
	char		*path;

	if (array_len(t_path) > 2)
		return (ft_putstr_fd(" too many arguments\n", 2), 1);
	path = manage_home_path(t_path[1], all);
	if (path == NULL)
		return (1);
	o_path = getcwd(NULL, 0);
	if (!o_path)
		return (perror("getcwd"), 2);
	if (ft_strcmp(o_path, t_path[1]) == 0)
		return (free(o_path), 0);
	if (chdir(path) == -1)
		return (free(o_path), perror(path), 1);
	n_path = getcwd(NULL, 0);
	if (!n_path)
		return (perror("getcwd"), free(o_path), 2);
	env = all->env_list;
	while (env)
	{
		char *tmp;
		if (!strcmp(env->first , "PWD"))
		{
			tmp = env->second;
			env->second = n_path;
			free(tmp);
		}
		else if (strcmp(env->first , "OLDPWD") == 0)
		{
			tmp = env->second;
			env->second = o_path;
			free(tmp);
		}
		env = env->next;
	}
	return (0);
}
