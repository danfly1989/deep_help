/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daflynn <daflynn@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 15:34:58 by daflynn           #+#    #+#             */
/*   Updated: 2025/09/16 15:35:09 by daflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_echo(char **arr, size_t k)
{
	int	i;
	int	newline;

	i = 1;
	newline = 1;
	while (arr[k + i] != NULL && ft_strncmp(arr[k + i], "-n", 2) == 0)
	{
		newline = 0;
		i++;
	}
	while (arr[k + i] != NULL)
	{
		printf("%s", arr[k + i]);
		i++;
		if (arr[k + i] != NULL)
			printf(" ");
	}
	if (newline)
		printf("\n");
}
