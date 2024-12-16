/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: larakoto < larakoto@student.42antananar    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 10:02:40 by larakoto          #+#    #+#             */
/*   Updated: 2024/12/16 10:55:49 by larakoto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*************************************************************/

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = -1;
	if (ft_strlen(s1) != ft_strlen(s2))
		return(1);
	while (s1[++i] != '\0' && s2[i] != '\0')
		if (s1[i] != s2[i])
			return ((unsigned char) s1[i] - (unsigned char) s2[i]);
	return (0);
}

/*************************************************************/

void	ft_print_export_error(char *command)
{
	ft_putstr_fd("export: ", 2);
	ft_putstr_fd("'", 2);
	ft_putstr_fd(command, 2);
	ft_putstr_fd("'", 2);
	ft_putstr_fd(": not a valid identifier\n", 2);
}

int	ft_check_after_first_caracter(char *variable_name, char *command)
{
	int	i;

	i = 0;
	while (variable_name[i])
	{
		if (!ft_isalpha(variable_name[i]) && !ft_isdigit(variable_name[i]) && variable_name[i] != '_')
		{
			ft_print_export_error(command);
			return (1);
		}
		i++;
	}
	return (0);
}

int	ft_export_error(char *variable_name, char *command)
{
	if (variable_name)
	{
		if (variable_name[0] == '\0' || (!ft_isalpha(variable_name[0]) && variable_name[0] != '_'))
		{
			ft_print_export_error(command);
			return (1);
		}
		else if (ft_check_after_first_caracter(variable_name, command) == 1)
			return (1);
	}
	return (0);
}

t_env_list	*ft_dup_env(t_env_list **env)
{
	t_env_list	*tmp;
	t_env_list	*dup_env;

	dup_env = NULL;
	tmp = *env;
	while (tmp)
	{
		ft_lstadd_back(&dup_env, ft_lstnew(tmp->value, 0));
		tmp = tmp->next;
	}
	return (dup_env);
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
		if (strcmp(tmp->first, "_") == 0)
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

void	ft_update_flag_1(t_env_list *env_list, char *line)
{
	char	*char_tmp;

	char_tmp = ft_strdup(env_list->second);
	free(env_list->second);
	env_list->second = ft_strjoin(char_tmp, ++line);
	free(char_tmp);
	free(env_list->value);
	char_tmp = ft_strjoin(env_list->first, "=");
	env_list->value = ft_strjoin(char_tmp, env_list->second);
	free(char_tmp);
}

void	ft_update_flag_0(t_env_list *env_list, char *line, char *command)
{
	free(env_list->value);
	if (env_list->second)
	{
		free(env_list->second);
		if (line)
			env_list->second = ft_strdup(++line);
		else
			env_list->second = ft_strdup("");
	}
	else if (!env_list->second && line)
			env_list->second = ft_strdup(++line);
	env_list->value = ft_strdup(command);
}

void	ft_update_ensemble(int *flag, t_env_list *env_list, char *line,
		char *command)
{
	if (*flag == 1)
		ft_update_flag_1(env_list, line);
	else
		ft_update_flag_0(env_list, line, command);
}

char	*ft_init_variable_name(char *command, int *flag, char *line)
{
	char	*variable_name;

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

int	ft_maj_export(t_env_list *env, char *command, int *flag)
{
	char		*line;
	char		*variable_name;
	t_env_list	*env_list;

	env_list = env;
	line = ft_strchr(command, '=');
	printf("line: %s\n", line);
	variable_name = ft_init_variable_name(command, flag, line);
	if (ft_export_error(variable_name, command) == 1)
		return (free(variable_name), 2);
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
	int		i;
	int		flag;
	t_env_list	*new;

	i = 1;
	while (command[i] != NULL)
	{
		flag = 0;
		if (command[i])
		{
			if (ft_maj_export(*env, command[i], &flag) == 0)
			{
				new = ft_lstnew(command[i], flag);
				ft_lstadd_back(env, new);
			}
		}
		i++;
	}
	return (0);
}
