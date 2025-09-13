/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daflynn <daflynn@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 20:03:59 by daflynn           #+#    #+#             */
/*   Updated: 2025/09/12 20:04:08 by daflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_expand_loop(char *token, t_dat *data, char **res, size_t *i)
{
	char	*key;
	char	*val;
	char	*tmp;

	key = ft_extract_var_key(token, i);
	if (!key)
		return ;
	val = NULL;
	if (data->lo != NULL)
		val = ft_get_var_value(data->lo, key);
	if (val == NULL)
		val = ft_get_var_value(data->ev, key);
	if (val != NULL)
	{
		tmp = *res;
		*res = ft_strjoin(*res, val);
		free(tmp);
	}
	free(key);
}

char	*ft_expand_token(char *token, t_dat *data, int qt, size_t i)
{
	char	*res;
	char	*tmp;

	if (qt == 1)
		return (ft_strdup(token));
	res = ft_calloc(1, sizeof(char));
	while (token[i])
	{
		if (token[i] == '$' && token[i + 1] && (ft_isalpha(token[i + 1])
				|| token[i + 1] == '_' || token[i + 1] == '?'))
		{
			i++;
			ft_expand_loop(token, data, &res, &i);
		}
		else
		{
			tmp = res;
			res = ft_strjoin_char(res, token[i++]);
			free(tmp);
		}
	}
	return (res);
}

char	**ft_expand_tokens(t_dat *d, char **tokens, int *qtypes, int i)
{
	char	**expanded;

	while (tokens[i])
		i++;
	expanded = malloc(sizeof(char *) * (i + 1));
	if (!expanded)
		return (NULL);
	i = 0;
	while (tokens[i])
	{
		d->tmp2 = ft_expand_exit_status(d, tokens[i]);
		expanded[i] = ft_expand_token(d->tmp2, d, qtypes[i], 0);
		free(d->tmp2);
		d->tmp2 = NULL;
		if (!expanded[i])
			return (ft_free_error_expanded(expanded, i));
		i++;
	}
	expanded[i] = NULL;
	return (expanded);
}

void	ft_check_var_assign_and_expand_line_ext(t_dat *data, char *line)
{
	ft_strip_quotes_from_xln(data);
	ft_external_functions(data, line);
	if (data->qtypes)
	{
		free(data->qtypes);
		data->qtypes = NULL;
	}
	ft_free_string_array(data->ln);
	data->ln = NULL;
	ft_free_string_array(data->xln);
	data->xln = NULL;
}

void	ft_check_var_assign_and_expand_line(t_dat *data, char *line)
{
	if (!data || !line)
		return ;
	data->qtypes = NULL;
	data->ln = ft_tokenize_line(data, line, &data->qtypes);
	if (!data->ln)
	{
		if (data->qtypes)
			free(data->qtypes);
		return ;
	}
	data->xln = ft_expand_tokens(data, data->ln, data->qtypes, 0);
	if (!data->xln)
	{
		if (data->qtypes)
			free(data->qtypes);
		ft_free_string_array(data->ln);
		data->ln = NULL;
		return ;
	}
	ft_check_var_assign_and_expand_line_ext(data, line);
}