/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_heredoc_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: safandri <safandri@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 14:35:23 by safandri          #+#    #+#             */
/*   Updated: 2024/12/20 15:16:01 by safandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*join_result(char *buffer, char *input, int replace_env, t_all *all)
{
	char	*tmp;
	char	*tmp2;

	tmp2 = NULL;
	if (replace_env == 1)
		input = replace_env_vars(input, all);
	tmp = ft_strjoin(input, "\n");
	free(input);
	tmp2 = buffer;
	buffer = ft_strjoin(buffer, tmp);
	return (free(tmp2), free(tmp), buffer);
}
