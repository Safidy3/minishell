/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: safandri <safandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 10:02:40 by larakoto          #+#    #+#             */
/*   Updated: 2024/11/15 17:10:50 by safandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*************************************************************/

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while ((s1[i] != '\0' || s2[i] != '\0'))
	{
		if (s1[i] != s2[i])
			return ((unsigned char) s1[i] - (unsigned char) s2[i]);
		i++;
	}
	return (0);
}

/*************************************************************/

void	ft_print_export_error(char *commande)
{
	ft_putstr_fd("export: ", 2);
	ft_putstr_fd("'", 2);
	ft_putstr_fd(commande, 2);
	ft_putstr_fd("'", 2);
	ft_putstr_fd(": not a valid identifier\n", 2);
}

int	ft_check_after_first_caracter(char *variable_name, char *commande)
{
	int	i;

	i = 0;
	while (variable_name[i])
	{
		if (!ft_isalpha(variable_name[i]) && !ft_isdigit(variable_name[i])
			&& variable_name[i] != '_')
		{
			ft_print_export_error(commande);
			return (1);
		}
		i++;
	}
	return (0);
}

int	ft_export_error(char *variable_name, char *commande)
{
	if (variable_name)
	{
		if (variable_name[0] == '\0' || (!ft_isalpha(variable_name[0])
				&& variable_name[0] != '_'))
		{
			ft_print_export_error(commande);
			return (1);
		}
		else if (ft_check_after_first_caracter(variable_name, commande) == 1)
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
	while ((tmp))
	{
		ft_lstadd_back(&dup_env, ft_lstnew(tmp->value, 0));
		tmp = tmp->next;
	}
	return (dup_env);
}

void	ft_print_export(t_env_list *env)
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
				printf("declare\t-x %s\n", tmp->first);
			else
				printf("declare\t-x %s=\"%s\"\n", tmp->first, tmp->second);
			tmp = tmp->next;
		}
		i++;
	}
	ft_free_env_list(env_export);
}

void	ft_update_flag_1(t_env_list *tmp, char *line)
{
	char	*char_tmp;

	char_tmp = ft_strdup(tmp->second);
	free(tmp->second);
	tmp->second = ft_strjoin(char_tmp, ++line);
	free(char_tmp);
	free(tmp->value);
	char_tmp = ft_strjoin(tmp->first, "=");
	tmp->value = ft_strjoin(char_tmp, tmp->second);
	free(char_tmp);
}

void	ft_update_flag_0(t_env_list *tmp, char *line, char *commande)
{
	free(tmp->value);
	free(tmp->second);
	tmp->second = ft_strdup(++line);
	tmp->value = ft_strdup(commande);
}

void	ft_update_ensemble(int *flag, t_env_list *tmp, char *line,
		char *commande)
{
	if (*flag == 1)
		ft_update_flag_1(tmp, line);
	else
		ft_update_flag_0(tmp, line, commande);
}

char	*ft_init_variable_name(char *commande, int *flag, char *line)
{
	char	*variable_name;

	if (!line)
		variable_name = ft_strdup(commande);
	else
	{
		if (commande[line - commande - 1] == '+')
			*flag = 1;
		if (*flag == 1)
			variable_name = ft_substr(commande, 0, (line - commande - 1));
		else
			variable_name = ft_substr(commande, 0, (line - commande));
	}
	return (variable_name);
}

void	update_final_export(char *line, t_env_list *tmp,
		char *variable_name, int *flag, char *commande)
{
	if (ft_strcmp(tmp->first, variable_name) == 0)
	{
		if (line)
		{
			ft_update_ensemble(flag, tmp, line, commande);
			free(variable_name);
		}
		free(variable_name);
	}
}

int	ft_maj_export(t_env_list *env, char *commande, int *flag)
{
	char		*line;
	char		*variable_name;
	t_env_list	*tmp;

	tmp = env;
	line = strchr(commande, '=');
	variable_name = ft_init_variable_name(commande, flag, line);
	if (ft_export_error(variable_name, commande) == 1)
		return (free(variable_name), 1);
	while (tmp)
	{
		update_final_export(line, tmp, variable_name, flag, commande);
		tmp = tmp->next;
	}
	return (free(variable_name), 0);
}

void	ft_export(t_env_list *env, char **commande)
{
	int			i;
	int			flag;

	flag = 0;
	i = 0;
	while (commande[i] != NULL)
	{
		if (commande[i] && ft_maj_export(env, commande[i], &flag) == 0)
			ft_lstadd_back(&env, ft_lstnew(commande[i], flag));
		i++;
	}
}
