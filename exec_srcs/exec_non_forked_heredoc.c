/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_non_forked_heredoc.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: safandri <safandri@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 11:54:00 by safandri          #+#    #+#             */
/*   Updated: 2024/12/21 11:54:12 by safandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_builtin_hered_singint(char *buffer)
{
	ft_free(buffer);
	g_flag = 0;
}

int	handle_builtin_ctrl_d(char *delimiter, char *input)
{
	if (!input && g_flag != SIGINT)
	{
		ft_putstr_fd("bash: warning: here-document (", 2);
		ft_putstr_fd(delimiter, 2);
		ft_putstr_fd(")\n", 2);
		return (1);
	}
	return (0);
}

char	*read_join_builtin_heredoc(char *buffer, char *delimiter, t_all *all)
{
	char	*input;
	int		replace_env;

	input = NULL;
	replace_env = 1;
	delimiter = new_delimiter(delimiter, &replace_env);
	while (1)
	{
		put_signal_handlig(2);
		input = readline("heredoc> ");
		if (handle_builtin_ctrl_d(delimiter, input))
			break ;
		if (g_flag == SIGINT)
			return (handle_builtin_hered_singint(buffer), NULL);
		if (ft_strcmp(input, delimiter) == 0)
		{
			free(input);
			break ;
		}
		buffer = join_result(buffer, input, replace_env, all);
	}
	if (replace_env == 0)
		free(delimiter);
	return (buffer);
}

int	get_builtin_heredoc(char *delimiter, t_all *all)
{
	char	*buffer;

	buffer = ft_strdup("");
	buffer = read_join_builtin_heredoc(buffer, delimiter, all);
	if (buffer)
		return (free(buffer), 0);
	return (1);
}
