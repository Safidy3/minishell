/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_signals.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: larakoto < larakoto@student.42antananar    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/21 10:08:57 by larakoto          #+#    #+#             */
/*   Updated: 2024/09/21 16:12:13 by larakoto         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "minishell.h"

void handle_signals(int sig)
{
    if (sig == SIGINT)
    {
        printf("\n");
        rl_on_new_line();
        rl_replace_line("",0);
        rl_redisplay();
    }
    else if (sig == SIGQUIT)
    {
        
    }
}

void put_signal_handlig()
{
    signal(SIGINT,handle_signals);
    signal(SIGQUIT,SIG_IGN);
}




