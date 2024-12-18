/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: larakoto < larakoto@student.42antananar    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 17:08:16 by safandri          #+#    #+#             */
/*   Updated: 2024/12/18 12:01:14 by larakoto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/******************* init list ******************/

void	list_init_error(t_list *commands_list, char **arr_commands)
{
	split_iterate((void **) arr_commands, free_split_1);
	free(arr_commands);
	free_list(commands_list);
	printf("Allocation error \033[0m: faild to allocate memory\n");
	exit(EXIT_FAILURE);
}

void	init_list(t_list **commands_list, char **arr_commands)
{
	t_list	*new_list;
	int		i;

	i = -1;
	while (arr_commands[++i])
	{
		new_list = ft_newlst((void *) ft_split_esc_2(arr_commands[i], ' '),
				ft_split_arg_type(arr_commands[i], ' '));
		if (!new_list)
			list_init_error(*commands_list, arr_commands);
		if (i == 0)
			*commands_list = new_list;
		else
			ft_add_back_lst(commands_list, new_list);
		free(arr_commands[i]);
	}
	free(arr_commands);
}

/******************	env var ********************/

char	*ft_getenv(char *env_var, t_all *all)
{
	t_env_list	*tmp;
	char		*res;

	tmp = all->env_list;
	res = NULL;
	while (tmp)
	{
		if (!ft_strcmp(tmp->first, env_var) && tmp->second)
		{
			res = ft_strdup(tmp->second);
			break ;
		}
		tmp = tmp->next;
	}
	return (res);
}

char	*get_env_name(char *s)
{
	char	*var_name;
	size_t	len;
	size_t	i;

	if (*s != '$')
		return (NULL);
	s++;
	i = 0;
	len = -1;
	while (s[++len] && ft_isdigit(s[len]))
		;
	while (s[++len] && (s[len] == '_' || ft_isalnum(s[len])))
		i++;
	var_name = (char *)malloc(sizeof(char) * (len + 1));
	if (!var_name)
		return (NULL);
	var_name[len] = '\0';
	i = -1;
	while (s[++i] && i < len)
		var_name[i] = s[i];
	return (var_name);
}

char	*get_env_value(char *var_name, t_all *all)
{
	char	*var_value;

	var_value = NULL;
	if (var_name)
		var_value = ft_getenv(var_name, all);
	return (var_value);
}

char	*copy_char(char *dest, char c)
{
	char	*res;
	int		len;

	if (!dest)
		return (NULL);
	len = ft_strlen(dest);
	res = malloc(sizeof(char) * (len + 2));
	if (!res)
		return (NULL);
	ft_strlcpy(res, dest, len + 1);
	res[len] = c;
	res[len + 1] = '\0';
	return (free(dest), res);
}

void	manage_env_var(char **dst, char **s, t_all *all)
{
	char	*env_name;
	char	*env_val;
	char	*temp;

	env_name = get_env_name(*s);
	env_val = get_env_value(env_name, all);
	if (env_val)
	{
		temp = *dst;
		*dst = ft_strjoin(*dst, env_val);
		free(temp);
		free(env_val);
	}
	else if (ft_isdigit(*(*s + 1)))
	{
		temp = *dst;
		*dst = ft_strjoin(*dst, &env_name[1]);
		free(temp);
	}
	while (++(*s) && (**s == '_' || ft_isalnum(**s)))
		;
	ft_free(env_name);
}

void	append_env_value(char **dst, char **s, t_all *all)
{
	char	*temp;
	char	*exit_stat;

	if (**s == '$' && ft_isalnum(*(*s + 1)))
		manage_env_var(dst, s, all);
	else if (**s == '$' && *(*s + 1) == '?')
	{
		exit_stat = ft_itoa(all->exit_status);
		temp = *dst;
		*dst = ft_strjoin(*dst, exit_stat);
		free(temp);
		free(exit_stat);
		*s += 2;
	}
	else if (**s == '$' && (*(*s + 1) == '\'' || *(*s + 1) == '"'))
		*s += 1;
	else
		*dst = copy_char(*dst, *(*s)++);
}

void	append_env_value_quote(char **dst, char **s, t_all *all)
{
	char	*temp;
	char	*exit_stat;

	if (**s == '$' && ft_isalnum(*(*s + 1)))
		manage_env_var(dst, s, all);
	else if (**s == '$' && *(*s + 1) == '?')
	{
		exit_stat = ft_itoa(all->exit_status);
		temp = *dst;
		*dst = ft_strjoin(*dst, exit_stat);
		free(temp);
		free(exit_stat);
		*s += 2;
	}
	else if (**s == '$' && (ft_isspace(*(*s + 1))
			|| *(*s + 1) == '\'' || *(*s + 1) == '"'
			|| !(*(*s + 1)) || !ft_isalnum(*(*s + 2))))
		*dst = copy_char(*dst, *(*s)++);
	else
		*dst = copy_char(*dst, *(*s)++);
}

void	append_quoted_text(char **dst, char **s, char quote, t_all *all)
{
	*dst = copy_char(*dst, *(*s)++);
	while (**s && **s != quote)
	{
		if (**s == '$' && quote == '"' )
			append_env_value_quote(dst, s, all);
		else if (**s == '\\' && *(*s + 1) == '$')
		{
			*dst = copy_char(*dst, *(*s + 1));
			*s += 2;
		}
		else
			*dst = copy_char(*dst, *(*s)++);
	}
	if (*s && **s == quote)
		*dst = copy_char(*dst, *(*s)++);
}

int	if_path(char *s)
{
	if (*s == '~' && (!*(s + 1) || ft_isspace(*(s + 1))
			|| *(s + 1) == '/') && (!*(s - 1) || ft_isspace(*(s - 1))))
		return (1);
	return (0);
}

char	*expand_path(char **s, char **dst, t_all *all)
{
	char	*temp;
	char	*home_path;

	home_path = ft_getenv("HOME", all);
	if (!home_path)
		return (*dst);
	temp = *dst;
	*dst = ft_strjoin(*dst, home_path);
	free(temp);
	(*s)++;
	return (*dst);
}

char	*replace_env_vars(char *s, t_all *all)
{
	char	*dst;

	if (!ft_strchr(s, '$'))
		return (s);
	dst = ft_strdup("");
	while (*s)
	{
		if (*s == '\'')
			append_quoted_text(&dst, &s, '\'', all);
		else if (*s == '\"')
			append_quoted_text(&dst, &s, '\"', all);
		else if (*s == '\\' && *(s + 1) == '$')
		{
			dst = copy_char(dst, *(s + 1));
			s += 2;
		}
		else if (*s == '<' && *(s + 1) == '<')
		{
			dst = copy_char(dst, *s++);
			dst = copy_char(dst, *s++);
			while (isspace(*s))
				dst = copy_char(dst, *s++);
			if (*s == '\'')
			{
				dst = copy_char(dst, '"');
				dst = copy_char(dst, *s++);
				while (*s && *s != '\'')
					dst = copy_char(dst, *s++);
				dst = copy_char(dst, *s++);
				dst = copy_char(dst, '"');
			}
			else if (*s == '"')
			{
				dst = copy_char(dst, '\'');
				dst = copy_char(dst, *s++);
				while (*s && *s != '"')
					dst = copy_char(dst, *s++);
				dst = copy_char(dst, *s++);
				dst = copy_char(dst, '\'');
			}
			else
				while (*s && !isspace(*s))
					dst = copy_char(dst, *s++);
		}
		else if (*s == '$' && *(s + 1))
			append_env_value(&dst, &s, all);
		else if (if_path(s))
			dst = expand_path(&s, &dst, all);
		else
			dst = copy_char(dst, *s++);
	}
	return (dst);
}

int	get_exit_stat(pid_t pids[MAX_COMMANDS], int command_count)
{
	int	status;
	int	i;

	i = -1;
	status = 0;
	while (++i < command_count)
		waitpid(pids[i], &status, 0);
	return (WEXITSTATUS(status));
}

/******************* Exec BINARY PATH ******************/

int	check_spetial_char(char **command)
{
	int	i;

	i = -1;
	while (command[++i])
		if (command[i][0] == '>' || command[i][0] == '<' )
			return (1);
	return (0);
}

/******************* Exec Pipe ******************/

void	dup_in(int fd[2], int closeall)
{
	dup2(fd[0], STDIN_FILENO);
	close(fd[0]);
	if (closeall != 0)
		close(fd[1]);
}

void	dup_out(int fd[2], int closeall)
{
	dup2(fd[1], STDOUT_FILENO);
	close(fd[1]);
	if (closeall != 0)
		close(fd[0]);
}

/******************* command validity ******************/

void	command_error(t_all *all)
{
	ft_putstr_fd("error : faild to allocate command\n", 1);
	free_all_struct(all);
	exit(EXIT_FAILURE);
}

