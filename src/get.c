/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daflynn <daflynn@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 20:57:05 by daflynn           #+#    #+#             */
/*   Updated: 2025/09/12 20:57:11 by daflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_get_val_from_list(t_va *head, const char *key)
{
	t_va	*cur;
	size_t	len;

	if (!head || !key)
		return (NULL);
	len = ft_strlen(key);
	cur = head;
	while (cur)
	{
		if (ft_strncmp(cur->name, key, len + 1) == 0)
			return (cur->value);
		cur = cur->next;
	}
	return (NULL);
}
