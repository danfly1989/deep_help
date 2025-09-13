/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daflynn <daflynn@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 20:11:06 by daflynn           #+#    #+#             */
/*   Updated: 2025/09/12 20:11:14 by daflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

t_va	*ft_create_node(char *str)
{
	t_va	*node;

	node = malloc(sizeof(t_va));
	if (!node)
		return (NULL);
	node->name = NULL;
	node->value = ft_strdup(str);
	node->next = NULL;
	return (node);
}

t_va	*ft_create_var_node(char *str)
{
	t_va	*node;
	char	*equal_pos;

	if (!str)
		return (NULL);
	equal_pos = ft_strchr(str, '=');
	if (!equal_pos)
		return (NULL);
	node = malloc(sizeof(t_va));
	if (!node)
		return (NULL);
	node->name = ft_extract_var_name(str);
	node->value = ft_extract_var_value(equal_pos + 1, 0, 0);
	node->next = NULL;
	if (!node->name || !node->value)
	{
		free(node->name);
		free(node->value);
		free(node);
		return (NULL);
	}
	return (node);
}
