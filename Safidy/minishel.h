/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: safandri <safandri@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 14:23:39 by safandri          #+#    #+#             */
/*   Updated: 2024/08/30 15:20:18 by safandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>
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

#endif