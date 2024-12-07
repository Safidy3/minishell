/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_signals.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: larakoto < larakoto@student.42antananar    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 10:08:17 by larakoto          #+#    #+#             */
/*   Updated: 2024/12/07 10:08:50 by larakoto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile int flag;

void handle_ctrl_c_heredoc(int sig, siginfo_t *ok, void *param)
{
	int pipefd[2];
	
	if (ok->si_pid != 0 && sig == SIGINT)
	{
		pipe(pipefd);
		ft_putstr_fd("\n", pipefd[1]);
		close(pipefd[1]);
		dup2(pipefd[0], STDIN_FILENO);
		rl_replace_line("", 0);
		close(pipefd[0]);
	}
	flag = SIGINT;
	param++;
}
void put_signal_handlig(int i)
{
	struct sigaction a;

	ft_bzero(&a, sizeof(sigaction));
	if (i == 0)
		a.sa_handler = SIG_IGN;
	else if (i == 2)
		a.sa_sigaction = handle_ctrl_c_heredoc;
	sigemptyset(&a.sa_mask);
	a.sa_flags = SA_SIGINFO;
	if (sigaction(SIGINT, &a, NULL) < 0)
	{
		return;
	}
	signal(SIGQUIT, SIG_IGN);
}
