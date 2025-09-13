/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dagwu <dagwu@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 18:44:41 by dagwu             #+#    #+#             */
/*   Updated: 2025/06/14 18:44:46 by dagwu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_setup_io(int **fd, size_t i, size_t total)
{
	if (i > 0)
		dup2(fd[i - 1][0], STDIN_FILENO);
	if (i < total - 1)
		dup2(fd[i][1], STDOUT_FILENO);
}

int	ft_validate_segment(char **tokens, int start, int end)
{
	int	i;

	if (!tokens || start >= end)
		return (0);
	i = start;
	while (i < end)
	{
		if (ft_is_redirection(tokens[i]))
		{
			if (i + 1 >= end || ft_is_redirection(tokens[i + 1])
				|| !ft_strcmp(tokens[i + 1], "|"))
			{
				return (ft_syntax_error_msg(tokens[i + 1]));
			}
		}
		i++;
	}
	return (1);
}

void	ft_nested_parent(pid_t pid, int saved_stdin)
{
	int	status;
	int	signal_num;

	//(void)d;
	status = 0;
	while (waitpid(pid, &status, 0) == -1 && errno == EINTR)
		;
	if (WIFEXITED(status))
		g_last_exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		signal_num = WTERMSIG(status);
		if (signal_num == SIGQUIT)
		{
			write(1, "^\\", 2);
			g_last_exit_status = 131;
		}
		else if (signal_num == SIGINT)
		{
			write(1, "\n", 1);
			g_last_exit_status = 130;
		}
	}
	if (dup2(saved_stdin, STDIN_FILENO) < 0)
		perror("dup2");
	close(saved_stdin);
}
