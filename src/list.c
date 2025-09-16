/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daflynn <daflynn@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 11:53:28 by daflynn           #+#    #+#             */
/*   Updated: 2025/09/16 11:53:35 by daflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_split_list(t_va *source, t_va **front, t_va **back)
{
	t_va	*fast;
	t_va	*slow;

	slow = source;
	fast = source->next;
	while (fast != NULL)
	{
		fast = fast->next;
		if (fast != NULL)
		{
			slow = slow->next;
			fast = fast->next;
		}
	}
	*front = source;
	*back = slow->next;
	slow->next = NULL;
}

void	ft_sort_list_by_name(t_va **head_ref)
{
	t_va	*head;
	t_va	*a;
	t_va	*b;

	if (*head_ref == NULL || (*head_ref)->next == NULL)
		return ;
	head = *head_ref;
	ft_split_list(head, &a, &b);
	ft_sort_list_by_name(&a);
	ft_sort_list_by_name(&b);
	*head_ref = ft_merge_sorted_lists(a, b);
}
