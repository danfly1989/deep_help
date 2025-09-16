/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daflynn <daflynn@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 19:32:13 by daflynn           #+#    #+#             */
/*   Updated: 2025/09/13 19:32:20 by daflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_update_existing_var(t_va *node, const char *name, const char *val)
{
	while (node)
	{
		if (ft_strncmp(node->name, name, ft_strlen(name)) == 0)
		{
			free(node->value);
			node->value = ft_strdup(val);
			if (!node->value)
				perror("minishell: malloc error");
			return (1);
		}
		node = node->next;
	}
	return (0);
}

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

void	ft_unset_multi_var(t_dat *d, size_t k)
{
	int	i;

	i = 1;
	while (d->xln[k + i] != NULL)
	{
		d->ev = ft_remove_variable_node(d->xln[k + i], d->ev, NULL);
		d->lo = ft_remove_variable_node(d->xln[k + i], d->lo, NULL);
		i++;
	}
}

t_va	*ft_remove_variable_node(const char *key, t_va *head, t_va *prev)
{
	t_va	*cur;
	t_va	*next;

	cur = head;
	while (cur != NULL)
	{
		if (ft_strncmp(cur->name, key, ft_strlen(key)) == 0)
		{
			next = cur->next;
			free(cur->name);
			free(cur->value);
			free(cur);
			if (prev == NULL)
				head = next;
			else
				prev->next = next;
			break ;
		}
		prev = cur;
		cur = cur->next;
	}
	return (head);
}

int	ft_var_name_only(char *str)
{
	size_t	i;

	if (!str)
		return (0);
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (0);
	i = 1;
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

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

void	ft_print_sorted_env(t_va *head)
{
	t_va *sorted;

	sorted = ft_duplicate_list(head);
	ft_sort_list_by_name(&sorted);
	ft_print_export(sorted);
	ft_free_list(sorted);
}