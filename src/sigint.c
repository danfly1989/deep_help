/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sigint.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daflynn <daflynn@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 20:40:16 by daflynn           #+#    #+#             */
/*   Updated: 2025/09/12 20:40:30 by daflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_nested_sigint_handler(int sig)
{
	(void)sig;
	rl_replace_line("", 0);
	rl_redisplay();
	g_last_exit_status = 130;
}

void	ft_parent_sigint_handler(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	// rl_redisplay();
	g_last_exit_status = 130;
}

void	ft_child_sigint_handler(int sig)
{
	if (sig == SIGINT)
	{
		write(1, "\n", 1);
		exit(130);
	}
	else if (sig == SIGQUIT) // Add the missing condition!
	{
		// Don't use readline functions in child signal handlers
		exit(131);
	}
}
