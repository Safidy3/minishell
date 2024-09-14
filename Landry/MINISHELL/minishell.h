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

typedef struct s_command_table	t_command_table;

struct							s_command_table
{
	char						*main_cmd;
	char						*options;
};
typedef struct s_command_list
{
	char *value;
	struct s_command_list *next;

}t_command_list;


char	**ft_split(char const *s, char c);


#endif