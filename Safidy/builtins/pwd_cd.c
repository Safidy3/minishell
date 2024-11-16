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

void ft_cd(char *path, t_all *all)
{
	t_env_list	*env;
	char		*o_path;
	char		*n_path;

	o_path = getcwd(NULL,0);
	chdir(path);
	n_path = getcwd(NULL,0);
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
