/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daflynn <daflynn@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 12:14:06 by daflynn           #+#    #+#             */
/*   Updated: 2025/09/16 12:14:14 by daflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_execute_pipeline(t_dat *d, char ***cmd)
{
	int **fd;

	if (ft_strcmp(cmd[0][0], "./minishell") == 0)
	{
		ft_nested_minishell(d, cmd[0], NULL);
		return ;
	}
	fd = init_fd_array(d->tot);
	if (!fd || !ft_create_pipes(fd, d->tot))
	{
		if (fd)
			ft_free_fd(fd);
		return ;
	}
	ft_fork_children(d, cmd, fd);
	ft_close_pipes(fd, d->tot);
	ft_wait_children(d, d->tot);
	ft_set_main_signals();
	ft_free_fd(fd);
}
