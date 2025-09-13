/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daflynn <daflynn@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 21:02:19 by daflynn           #+#    #+#             */
/*   Updated: 2025/09/12 21:02:25 by daflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_exit_numeric_error(char *arg)
{
	write(2, "minishell: exit: ", 18);
	write(2, arg, ft_strlen(arg));
	write(2, ": numeric argument required\n", 29);
}

void	ft_exit(t_dat *data, size_t k)
{
	int status;

	if (data->xln[k + 1] && data->xln[k + 2])
	{
		write(2, "minishell: exit: too many arguments\n", 36);
		return ;
	}
	rl_clear_history();
	if (data->xln[k + 1] == NULL)
		ft_cleanup_exit(data, 0);
	if (ft_is_number(data->xln[k + 1]) == 0)
	{
		ft_exit_numeric_error(data->xln[k + 1]);
		ft_cleanup_exit(data, 255);
	}
	status = ft_atoi(data->xln[k + 1]);
	ft_cleanup_exit(data, status % 256);
}