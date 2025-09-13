/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   end.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daflynn <daflynn@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 17:27:08 by daflynn           #+#    #+#             */
/*   Updated: 2025/09/13 17:27:18 by daflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_remove_redirections(char ***tokens_ptr, int i, int j)
{
	char	**tokens;

	if (!tokens_ptr || !*tokens_ptr)
		return (0);
	tokens = *tokens_ptr;
	while (tokens[i])
	{
		if (ft_is_redirection(tokens[i]) && tokens[i + 1])
		{
			free(tokens[i]);
			free(tokens[i + 1]);
			j = i - 1;
			while (tokens[++j + 2])
				tokens[j] = tokens[j + 2];
			tokens[j] = NULL;
			tokens[j + 1] = NULL;
			continue ;
		}
		i++;
	}
	*tokens_ptr = tokens;
	return (1);
}

int	ft_remove_sing_redirections(char **t, int i, int j)
{
	if (!t)
		return (0);
	i = 0;
	while (t[i])
	{
		if (ft_is_redirection(t[i]) && t[i + 1])
		{
			free(t[i]);
			free(t[i + 1]);
			j = i;
			while (t[j + 2])
			{
				t[j] = t[j + 2];
				j++;
			}
			t[j] = NULL;
			t[j + 1] = NULL;
			continue ;
		}
		i++;
	}
	return (1);
}

int	ft_is_redirection(char *str)
{
	return (!ft_strcmp(str, "<") || !ft_strcmp(str, ">") || !ft_strcmp(str,
			">>") || !ft_strcmp(str, "<<"));
}

void	ft_free_redirection(t_rdr *r)
{
	if (r->in_file)
	{
		free(r->in_file);
		r->in_file = NULL;
	}
	if (r->out_file)
	{
		free(r->out_file);
		r->out_file = NULL;
	}
	if (r->append_file)
	{
		free(r->append_file);
		r->append_file = NULL;
	}
	if (r->heredoc_delim)
	{
		free(r->heredoc_delim);
		r->heredoc_delim = NULL;
	}
}

int	ft_syntax_error(char *token)
{
	char	*mes1;
	char	*mes2;

	mes1 = "minishell: syntax error near unexpected token `";
	mes2 = "minishell: syntax error near unexpected token `newline'\n";
	if (token)
	{
		write(2, mes1, ft_strlen(mes1));
		write(2, token, ft_strlen(token));
		write(2, "'\n", 2);
	}
	else
		write(2, mes2, ft_strlen(mes2));
	return (0);
}

int	ft_check_redir(char **tokens, int i)
{
	if (!tokens[i + 1] || ft_is_redirection(tokens[i + 1])
		|| !ft_strcmp(tokens[i + 1], "|"))
		return (ft_syntax_error(tokens[i + 1]));
	return (1);
}

int	ft_check_pipe(char **tokens, int i)
{
	if (i == 0 || !tokens[i + 1] || ft_is_redirection(tokens[i + 1]))
		return (ft_syntax_error(tokens[i + 1]));
	return (1);
}

int	ft_validate_syntax(char **tokens)
{
	int	i;

	if (!tokens || !tokens[0])
		return (0);
	if (ft_is_redirection(tokens[0]) && ft_strcmp(tokens[0], "<<"))
		return (0);
	i = 0;
	while (tokens[i])
	{
		if (ft_is_redirection(tokens[i]))
		{
			if (!ft_check_redir(tokens, i))
				return (0);
		}
		else if (!ft_strcmp(tokens[i], "|"))
		{
			if (!ft_check_pipe(tokens, i))
				return (0);
		}
		i++;
	}
	return (1);
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

char	*append_char(char *res, char *token, int *i)
{
	char	*temp;

	temp = ft_substr(token, *i, 1);
	res = ft_strjoin_free(res, temp);
	free(temp);
	(*i)++;
	return (res);
}
