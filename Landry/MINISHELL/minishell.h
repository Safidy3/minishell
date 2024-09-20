/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: larakoto < larakoto@student.42antananar    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 10:47:14 by larakoto          #+#    #+#             */
/*   Updated: 2024/09/20 17:11:28 by larakoto         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#ifndef MINISHELL_H
# define MINISHELL_H
# include <readline/history.h>
# include <readline/readline.h>
# include <stdio.h>
# include <stdlib.h>
#include <stdbool.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>



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

char	**ft_split_separator(char *s);

char	**ft_split_simple(char const *s, char c);
char	*ft_strjoin(char const *s1, char const *s2);

void ft_exit(char *line);

#endif