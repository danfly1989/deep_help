/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daflynn <daflynn@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 20:36:33 by daflynn           #+#    #+#             */
/*   Updated: 2025/09/12 20:37:16 by daflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

char	*ft_extract_token(char *str, t_dat *d, int *quote_type)
{
	int		start;
	int		end;
	char	*token;

	start = d->i;
	*quote_type = 0;
	end = ft_get_token_end(str, d->i);
	d->i = end;
	token = ft_strndup(str + start, end - start);
	if (!token)
		return (NULL);
	ft_detect_quote_type(token, quote_type);
	return (token);
}

char	*ft_extract_var_key(const char *str, size_t *i)
{
	size_t	start;
	char	*key;

	start = *i;
	if (str[*i] == '?')
	{
		(*i)++;
		return (ft_strdup("?"));
	}
	while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
		(*i)++;
	key = ft_substr(str, start, *i - start);
	return (key);
}

char	**ft_extract_tokens(t_dat *data, int start, int end)
{
	char	**tokens;
	int		i;

	tokens = malloc((end - start + 1) * sizeof(char *));
	if (!tokens)
		return (NULL);
	i = 0;
	while (start < end)
	{
		tokens[i] = ft_strdup(data->xln[start]);
		if (!tokens[i])
		{
			ft_free_string_array(tokens);
			return (NULL);
		}
		start++;
		i++;
	}
	tokens[i] = NULL;
	return (tokens);
}
