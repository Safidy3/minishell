/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd_cd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: safandri <safandri@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 15:57:34 by larakoto          #+#    #+#             */
/*   Updated: 2024/12/21 15:04:41 by safandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_pwd(void)
{
	char	*s;

	s = getcwd(NULL, 0);
	printf("%s\n", s);
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
			if (!ft_strcmp(env->first, "HOME") && env->second)
				return (env->second);
			env = env->next;
		}
		return (ft_putstr_fd("bash: cd: HOME not set\n", 2), path);
	}
	else
		return (path);
}

void	replace_env_value(t_env_list *env, char *path, int *found)
{
	char	*tmp;

	*found = 1;
	tmp = env->second;
	env->second = path;
	free(tmp);
}

void	update_env(t_all *all, char *n_path, char *o_path)
{
	t_env_list	*env;
	int			found_old;
	int			found_new;

	found_old = 0;
	found_new = 0;
	env = all->env_list;
	while (env)
	{
		if (!ft_strcmp(env->first, "PWD"))
			replace_env_value(env, n_path, &found_new);
		else if (!ft_strcmp(env->first, "OLDPWD"))
			replace_env_value(env, o_path, &found_old);
		env = env->next;
	}
	if (!found_old)
		free(o_path);
	if (!found_new)
		free(n_path);
}

int	ft_cd(char **t_path, t_all *all)
{
	char	*o_path;
	char	*n_path;
	char	*path;

	if (t_path[1] && t_path[1][0] == '\0')
		return (0);
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
	update_env(all, n_path, o_path);
	return (0);
}
