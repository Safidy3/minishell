/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: safandri <safandri@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 10:02:40 by larakoto          #+#    #+#             */
/*   Updated: 2024/12/20 11:47:05 by safandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*ft_init_variable_name(char *command, int *flag, char *line)
{
	char	*variable_name;
	int		i;

	variable_name = NULL;
	i = 0;
	while (command[i] && isspace(command[i]))
		i++;
	if (command[i] == '=')
		return (variable_name);
	if (!line)
		variable_name = ft_strdup(command);
	else
	{
		if (command[line - command - 1] == '+')
			*flag = 1;
		if (*flag == 1)
			variable_name = ft_substr(command, 0, (line - command - 1));
		else
			variable_name = ft_substr(command, 0, (line - command));
	}
	return (variable_name);
}

int	ft_print_export(t_env_list *env)
{
	t_env_list	*tmp;
	t_env_list	*env_export;
	int			i;

	env_export = ft_dup_env(&env);
	ft_sort_list(env_export);
	tmp = env_export;
	i = 0;
	while (tmp)
	{
		if (ft_strcmp(tmp->first, "_") == 0)
			tmp = tmp->next;
		else
		{
			if (tmp->second == NULL)
				printf("declare -x %s\n", tmp->first);
			else
				printf("declare -x %s=\"%s\"\n", tmp->first, tmp->second);
			tmp = tmp->next;
		}
		i++;
	}
	ft_free_env_list(env_export);
	return (0);
}

int	ft_maj_export(t_env_list *env, char *command, int *flag, int *return_value)
{
	char		*line;
	char		*variable_name;
	t_env_list	*env_list;
	int			check_env;

	env_list = env;
	line = ft_strchr(command, '=');
	variable_name = ft_init_variable_name(command, flag, line);
	check_env = check_env_error(command, variable_name, return_value);
	if (check_env != 0)
		return (check_env);
	while (env_list)
	{
		if (ft_strcmp(env_list->first, variable_name) == 0)
		{
			if (line)
				ft_update_ensemble(flag, env_list, line, command);
			return (free(variable_name), 1);
		}
		env_list = env_list->next;
	}
	return (free(variable_name), 0);
}

int	ft_export(t_env_list **env, char **command)
{
	int			i;
	int			flag;
	int			return_value;
	t_env_list	*new;

	i = 1;
	return_value = 0;
	while (command[i] != NULL)
	{
		flag = 0;
		if (command[i])
		{
			if (ft_maj_export(*env, command[i], &flag, &return_value) == 0)
			{
				new = ft_lstnew(command[i], flag);
				ft_lstadd_back(env, new);
			}
		}
		i++;
	}
	return (return_value);
}
