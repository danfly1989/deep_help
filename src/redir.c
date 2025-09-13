/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daflynn <daflynn@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 15:33:15 by daflynn           #+#    #+#             */
/*   Updated: 2025/09/13 15:33:23 by daflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_check_redir(char **tokens, int i)
{
	if (!tokens[i + 1] || ft_is_redirection(tokens[i + 1])
		|| !ft_strcmp(tokens[i + 1], "|"))
		return (ft_syntax_error(tokens[i + 1]));
	return (1);
}
int	ft_parse_redirection(char **tokens, t_rdr *r)
{
	int	i;

	i = 0;
	ft_memset(r, 0, sizeof(*r));
	while (tokens[i])
	{
		if (!ft_strcmp(tokens[i], "<") && tokens[i + 1])
			r->in_file = ft_strdup(tokens[++i]);
		else if (!ft_strcmp(tokens[i], ">") && tokens[i + 1])
			r->out_file = ft_strdup(tokens[++i]);
		else if (!ft_strcmp(tokens[i], ">>") && tokens[i + 1])
			r->append_file = ft_strdup(tokens[++i]);
		else if (!ft_strcmp(tokens[i], "<<") && tokens[i + 1])
			r->heredoc_delim = ft_strdup(tokens[++i]);
		i++;
	}
	return (1);
}

int	ft_redir_in(char *file)
{
	int	fd;

	fd = open(file, O_RDONLY);
	if (fd < 0)
		return (perror(file), 0);
	if (dup2(fd, STDIN_FILENO) < 0)
		return (perror("dup2 in"), 0);
	close(fd);
	return (1);
}

int	ft_redir_out(char *file)
{
	int	fd;

	fd = open(file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd < 0)
		return (perror(file), 0);
	if (dup2(fd, STDOUT_FILENO) < 0)
		return (perror("dup2 out"), 0);
	close(fd);
	return (1);
}

int	ft_redir_append(char *file)
{
	int	fd;

	fd = open(file, O_CREAT | O_WRONLY | O_APPEND, 0644);
	if (fd < 0)
		return (perror(file), 0);
	if (dup2(fd, STDOUT_FILENO) < 0)
		return (perror("dup2 append"), 0);
	close(fd);
	return (1);
}

int	ft_apply_redirections(t_rdr *r, char **tokens)
{
	int	fd;

	// Handle output redirections (>) and append (>>)
	// (same as your newer function)
	if (r->heredoc_delim)
	{
		fd = handle_heredoc(r->heredoc_delim);
		if (fd == -1)
			return (0);
		if (dup2(fd, STDIN_FILENO) == -1)
		{
			perror("dup2");
			close(fd);
			return (0);
		}
		close(fd);
	}
	if (r->out_file && !ft_redir_out(r->out_file))
		return (0);
	if (r->append_file && !ft_redir_append(r->append_file))
		return (0);
	// Handle heredoc explicitly
	// Handle regular input file
	if (r->in_file && !ft_redir_in(r->in_file))
		return (0);
	ft_free_redirection(r);
	return (ft_remove_redirections(&tokens, 0, 0));
}

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
