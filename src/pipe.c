/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daflynn <daflynn@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 15:34:45 by daflynn           #+#    #+#             */
/*   Updated: 2025/09/13 15:34:52 by daflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_close_pipes(int **fd, int tot)
{
	int	i;

	i = 0;
	while (i < tot - 1)
	{
		close(fd[i][0]);
		close(fd[i][1]);
		i++;
	}
}

int	ft_check_pipe(char **tokens, int i)
{
	if (i == 0 || !tokens[i + 1] || ft_is_redirection(tokens[i + 1]))
		return (ft_syntax_error(tokens[i + 1]));
	return (1);
}
