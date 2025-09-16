/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daflynn <daflynn@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 15:32:14 by daflynn           #+#    #+#             */
/*   Updated: 2025/09/16 15:32:20 by daflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_print_export(t_va *head)
{
	t_va *cur;

	cur = head;
	while (cur != NULL)
	{
		printf("declare -x %s=\"%s\"\n", cur->name, cur->value);
		cur = cur->next;
	}
}