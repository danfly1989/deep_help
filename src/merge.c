/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   merge.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daflynn <daflynn@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 20:46:43 by daflynn           #+#    #+#             */
/*   Updated: 2025/09/12 20:46:48 by daflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strjoin_char(const char *s, char c)
{
	char	*new;
	size_t	len;

	if (!s)
		return (NULL);
	len = ft_strlen(s);
	new = malloc(len + 2);
	if (!new)
		return (NULL);
	ft_strlcpy(new, s, len + 1);
	new[len] = c;
	new[len + 1] = '\0';
	return (new);
}

void	ft_append_env_var(t_dat *data, char *key, char *value)
{
	t_va	*new;
	t_va	*cur;

	new = malloc(sizeof(t_va));
	if (!new)
		return ;
	new->name = ft_strdup(key);
	new->value = ft_strdup(value);
	new->next = NULL;
	cur = data->ev;
	if (!cur)
	{
		data->ev = new;
		return ;
	}
	while (cur->next)
		cur = cur->next;
	cur->next = new;
}

t_va	*ft_merge_sorted_lists(t_va *a, t_va *b)
{
	t_va	*result;

	if (a == NULL)
		return (b);
	else if (b == NULL)
		return (a);
	if (ft_strcmp(a->name, b->name) <= 0)
	{
		result = a;
		result->next = ft_merge_sorted_lists(a->next, b);
	}
	else
	{
		result = b;
		result->next = ft_merge_sorted_lists(a, b->next);
	}
	return (result);
}

t_va	*ft_duplicate_node(const t_va *node)
{
	t_va	*new;

	new = malloc(sizeof(t_va));
	if (new == NULL)
		return (NULL);
	new->name = ft_strdup(node->name);
	new->value = ft_strdup(node->value);
	new->next = NULL;
	if (new->name == NULL || new->value == NULL)
	{
		free(new->name);
		free(new->value);
		free(new);
		return (NULL);
	}
	return (new);
}

int	ft_append_dup_node(const t_va *cur, t_va **head, t_va **tail)
{
	t_va	*new_node;

	new_node = ft_duplicate_node(cur);
	if (new_node == NULL)
	{
		ft_free_list(*head);
		return (0);
	}
	if (*tail == NULL)
	{
		*head = new_node;
		*tail = new_node;
	}
	else
	{
		(*tail)->next = new_node;
		*tail = new_node;
	}
	return (1);
}

char	*ft_join_path(char *str1, char *cmd)
{
	char	*temp;
	char	*full_path;

	temp = ft_strjoin(str1, "/");
	full_path = ft_strjoin(temp, cmd);
	free(temp);
	temp = NULL;
	return (full_path);
}

char	*ft_strjoin_free(char *s1, char *s2)
{
	char	*joined;

	if (!s1 || !s2)
		return (NULL);
	joined = ft_strjoin(s1, s2);
	free(s1);
	return (joined);
}

char	*append_exit_status(char *res, int status, int *i)
{
	char	*temp;

	temp = ft_itoa(status);
	res = ft_strjoin_free(res, temp);
	free(temp);
	*i += 2;
	return (res);
}

char	*append_char(char *res, char *token, int *i)
{
	char	*temp;

	temp = ft_substr(token, *i, 1);
	res = ft_strjoin_free(res, temp);
	free(temp);
	(*i)++;
	return (res);
}
