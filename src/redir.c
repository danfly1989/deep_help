/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daflynn <daflynn@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 20:02:33 by daflynn           #+#    #+#             */
/*   Updated: 2025/09/13 20:02:39 by daflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_apply_redirections(t_rdr *r, char **tokens)
{
	int	fd;

	if (r->heredoc_delim)
	{
		fd = handle_heredoc(r->heredoc_delim);
		if (fd == -1)
			return (0);
		if (dup2(fd, STDIN_FILENO) == -1)
		{
			perror("dup2");
			close(fd);
			return (0);
		}
		close(fd);
	}
	if (r->out_file && !ft_redir_out(r->out_file))
		return (0);
	if (r->append_file && !ft_redir_append(r->append_file))
		return (0);
	if (r->in_file && !ft_redir_in(r->in_file))
		return (0);
	ft_free_redirection(r);
	return (ft_remove_redirections(&tokens, 0, 0));
}

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
	g_last_exit_status = 130;
}
