/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: safandri <safandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 14:23:39 by safandri          #+#    #+#             */
/*   Updated: 2024/10/17 13:10:31 by safandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/types.h>
# include <sys/wait.h>
# include "libft/libft.h"

# define MAX_COMMANDS 100

enum e_typage
{
	STRING,
	R_OUT,
	R_IN,
	R_PIPE,
	RR_IN,
	RR_OUT,
	PIPE_FILE,
	INPUT_FILE,
	OUTPUT_FILE,
};

typedef struct s_all
{
	char	**env;
	int		exit_status;
	t_list	*command_list;
}			t_all;

char	**ft_split_esc_2(char *s, char c);

#endif