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

void	ft_parent_sigint_handler(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	g_last_exit_status = 130;
}

int	ft_redir_in(char *file)
{
	int	fd;

	fd = open(file, O_RDONLY);
	if (fd < 0)
		return (perror(file), 0);
	if (dup2(fd, STDIN_FILENO) < 0)
		return (perror("dup2 in"), 0);
	close(fd);
	return (1);
}

int	ft_redir_out(char *file)
{
	int	fd;

	fd = open(file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd < 0)
		return (perror(file), 0);
	if (dup2(fd, STDOUT_FILENO) < 0)
		return (perror("dup2 out"), 0);
	close(fd);
	return (1);
}

int	ft_redir_append(char *file)
{
	int	fd;

	fd = open(file, O_CREAT | O_WRONLY | O_APPEND, 0644);
	if (fd < 0)
		return (perror(file), 0);
	if (dup2(fd, STDOUT_FILENO) < 0)
		return (perror("dup2 append"), 0);
	close(fd);
	return (1);
}
