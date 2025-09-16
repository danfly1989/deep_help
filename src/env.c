/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daflynn <daflynn@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 11:46:28 by daflynn           #+#    #+#             */
/*   Updated: 2025/09/16 11:46:36 by daflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_env(t_dat *data)
{
	t_va	*cur;

	cur = data->ev;
	while (cur != NULL)
	{
		printf("%s=%s\n", cur->name, cur->value);
		cur = cur->next;
	}
}
