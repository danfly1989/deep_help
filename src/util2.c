/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daflynn <daflynn@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 16:39:56 by daflynn           #+#    #+#             */
/*   Updated: 2025/09/13 16:40:03 by daflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

char	*ft_extract_var_name(char *str)
{
	char	*name;
	size_t	i;
	size_t	len;

	len = 0;
	while (str[len] && str[len] != '=')
		len++;
	name = malloc(len + 1);
	if (!name)
		return (NULL);
	i = 0;
	while (i < len)
	{
		name[i] = str[i];
		i++;
	}
	name[i] = '\0';
	return (name);
}

char	*ft_extract_var_value(char *str, char quote, size_t len)
{
	char	*val;
	size_t	i;

	if (!str || str[0] == '\0')
		return (NULL);
	if (str[0] == '"' || str[0] == '\'')
	{
		quote = str[0];
		str++;
	}
	while (str[len] && str[len] != quote)
		len++;
	val = malloc(len + 1);
	if (!val)
		return (NULL);
	i = -1;
	while (++i < len)
		val[i] = str[i];
	val[len] = '\0';
	return (val);
}

int	ft_strisspace(char *str)
{
	int	i;

	if (!str)
		return (0);
	i = 0;
	while (str[i] && str[i] != '\0')
	{
		if (!ft_isspace(str[i]))
			return (0);
		i++;
	}
	return (1);
}
