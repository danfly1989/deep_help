/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daflynn <daflynn@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 20:09:43 by daflynn           #+#    #+#             */
/*   Updated: 2025/09/12 20:09:53 by daflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


void	ft_free_lines(t_dat *data)
{
	ft_free_string_array(data->ln);
	data->ln = NULL;
	ft_free_string_array(data->xln);
	data->xln = NULL;
}

void	ft_free_string_array(char **str_array)
{
	int	i;

	if (str_array == NULL)
		return ;
	i = 0;
	while (str_array[i] != NULL)
	{
		free(str_array[i]);
		str_array[i] = NULL;
		i++;
	}
	free(str_array);
}

void	ft_cleanup_data(t_dat *data)
{
	if (data->ev)
		ft_free_list(data->ev);
	if (data->av)
		ft_free_list(data->av);
	if (data->lo)
		ft_free_list(data->lo);
	if (data->ln)
		ft_free_string_array(data->ln);
	if (data->xln)
		ft_free_string_array(data->xln);
	if (data->tmp1)
		free(data->tmp1);
	if (data->tmp2)
		free(data->tmp2);
	if (data->qtypes)
		free(data->qtypes);
	if (data->evs)
		ft_free_string_array(data->evs);
	if (data->avs)
		ft_free_string_array(data->avs);
	ft_nullify_pointers(data);
}

void	ft_nullify_pointers(t_dat *data)
{
	data->ev = NULL;
	data->av = NULL;
	data->lo = NULL;
	data->ln = NULL;
	data->xln = NULL;
	data->tmp1 = NULL;
	data->tmp2 = NULL;
	data->qtypes = NULL;
	data->evs = NULL;
	data->avs = NULL;
}

void	ft_cleanup_exit(t_dat *data, int flag)
{
	ft_cleanup_data(data);
	rl_clear_history();
	exit(flag);
}
