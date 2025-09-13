/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daflynn <daflynn@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 20:43:26 by daflynn           #+#    #+#             */
/*   Updated: 2025/09/12 20:43:34 by daflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_skip_token(char *str, int i)
{
	while (str[i] && str[i] != ' ')
	{
		if (str[i] == '\'' || str[i] == '"')
			i = ft_skip_quote(str, i);
		else
			i++;
	}
	return (i);
}

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

int	ft_get_token_end(char *str, int i)
{
	if (str[i] == '\'' || str[i] == '"')
		return (ft_skip_quote(str, i));
	return (ft_skip_token(str, i));
}

char	**ft_tokenize_line(t_dat *d, char *str, int **quote_types_out)
{
	char **tokens;

	ft_reset_iterators(d);
	d->k = ft_count_tokens(str);
	tokens = malloc(sizeof(char *) * (d->k + 1));
	d->qtypes = malloc(sizeof(int) * (d->k + 1));
	if (!tokens || !d->qtypes)
		return (ft_free_token_quote(tokens, d->qtypes));
	while (str[d->i])
	{
		while (str[d->i] == ' ')
			d->i++;
		if (!str[d->i])
			break ;
		tokens[d->j] = ft_extract_token(str, d, &d->qtypes[d->j]);
		d->j++;
	}
	tokens[d->j] = NULL;
	d->qtypes[d->j] = -1;
	*quote_types_out = d->qtypes;
	return (tokens);
}