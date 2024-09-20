/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: safandri <safandri@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 14:11:27 by safandri          #+#    #+#             */
/*   Updated: 2024/08/30 15:47:48 by safandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    echo(char *line)
{
	printf("%s\n", line);
}

void	pwd(void)
{
	char command[1024];
	char *ptr;

	ptr = getcwd(command, 1024);
	if (ptr == NULL)
	{
        perror("getcwd failed");
        //return 1;
    }
	printf("%s\n", ptr);
}

int main(void)
{
	// char *command;


	ft_putstr_fd("> ", 0);
	pwd();
	// while (1)
	// {
	// 	command = get_next_line(0);

	// }
	return (0);
}
