/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   count.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daflynn <daflynn@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 11:28:19 by daflynn           #+#    #+#             */
/*   Updated: 2025/09/16 11:28:25 by daflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_count_tokens(char *str)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (str[i])
	{
		while (str[i] == ' ')
			i++;
		if (!str[i])
			break ;
		count++;
		if (str[i] == '\'' || str[i] == '"')
			i = ft_skip_quote(str, i);
		else
			i = ft_skip_token(str, i);
	}
	return (count);
}

int	ft_count_list(t_va *head)
{
	t_va	*cur;
	int		count;

	cur = head;
	count = 0;
	while (cur)
	{
		if (cur->name)
			count++;
		cur = cur->next;
	}
	return (count);
}
