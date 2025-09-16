/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daflynn <daflynn@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 16:33:47 by daflynn           #+#    #+#             */
/*   Updated: 2025/09/13 16:33:57 by daflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_dat	ft_duplicate_input_args(int argc, char **argv, char **env)
{
	t_dat	data;

	(void)argc;
	data.av = NULL;
	data.ev = NULL;
	data.lo = NULL;
	data.ln = NULL;
	data.xln = NULL;
	data.tmp1 = NULL;
	data.tmp2 = NULL;
	data.i = 0;
	data.j = 0;
	data.k = 0;
	data.tot = 0;
	data.st = 0;
	data.no_pipes = 0;
	data.avs = NULL;
	data.evs = NULL;
	data.qtypes = NULL;
	data.av = create_lst_frm_arr(argv + 1, NULL, 0, ft_create_node);
	data.ev = create_lst_frm_arr(env, NULL, 0, ft_create_var_node);
	ft_increment_shlvl(&data.ev);
	return (data);
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

t_va	*create_lst_frm_arr(char **arr, t_va *h, int i, t_va *(*f)(char *))
{
	t_va	*current;
	t_va	*new_node;

	current = NULL;
	while (arr[i])
	{
		new_node = f(arr[i]);
		if (!new_node)
		{
			if (h)
				ft_free_list(h);
			return (NULL);
		}
		if (h == NULL)
			h = new_node;
		else
			current->next = new_node;
		current = new_node;
		i++;
	}
	return (h);
}
