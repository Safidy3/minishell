/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: safandri <safandri@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 11:42:21 by safandri          #+#    #+#             */
/*   Updated: 2024/12/20 11:48:46 by safandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

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

int	ft_check_after_first_caracter(char *variable_name, char *command)
{
	int	i;

	i = 0;
	while (variable_name[i])
	{
		if (!ft_isalpha(variable_name[i])
			&& !ft_isdigit(variable_name[i]) && variable_name[i] != '_')
		{
			ft_print_export_error(command);
			return (1);
		}
		i++;
	}
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
