/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: safandri <safandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 14:23:39 by safandri          #+#    #+#             */
/*   Updated: 2024/10/07 15:05:01 by safandri         ###   ########.fr       */
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

char	**ft_split_esc_2(char const *s, char c);

#endif