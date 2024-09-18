/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: larakoto <larakoto@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 10:47:14 by larakoto          #+#    #+#             */
/*   Updated: 2024/09/05 10:54:11 by larakoto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include <readline/history.h>
# include <readline/readline.h>
# include <stdio.h>
# include <stdlib.h>
#include <stdbool.h>

#define A 12
#define B 55

enum e_typage
{
	STRING,
	INPUT_FILE,
	OUTPUT_FILE,
	PIPE_FILE,
};

typedef struct s_command_list
{
	char *value;
	int type;
	struct s_command_list *next;

}t_command_list;

char **ft_split(char  *s);
char	**ft_split_simple(char const *s, char c);
char	*ft_strjoin(char const *s1, char const *s2);

#endif