/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daflynn <daflynn@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 19:36:50 by daflynn           #+#    #+#             */
/*   Updated: 2025/09/13 19:36:56 by daflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_handle_builtin(t_dat *data, size_t k)
{
	if (data == NULL || data->xln == NULL)
		return (0);
	if (ft_strcmp(data->xln[k], "pwd") == 0)
		ft_pwd();
	else if (ft_strcmp(data->xln[k], "cd") == 0)
		ft_change_directory(data, k);
	else if (ft_strcmp(data->xln[k], "echo") == 0)
		ft_echo(data->xln, k);
	else if (ft_strcmp(data->xln[k], "exit") == 0)
		ft_exit(data, k);
	else if (ft_strcmp(data->xln[k], "env") == 0)
		ft_env(data);
	else if (ft_strcmp(data->xln[k], "unset") == 0)
		ft_unset_multi_var(data, k);
	else if (ft_strcmp(data->xln[k], "export") == 0)
		ft_export_multi_var(data, k);
	else
		return (0);
	return (1);
}

void	ft_list_to_env_array(t_dat *data)
{
	int		i;
	int		count;
	t_va	*cur;

	i = 0;
	data->tmp1 = NULL;
	count = ft_count_list(data->ev);
	data->evs = malloc((count + 1) * sizeof(char *));
	if (!data->evs)
		return ;
	cur = data->ev;
	while (cur && i < count)
	{
		data->tmp1 = ft_strjoin(cur->name, "=");
		data->evs[i] = ft_strjoin(data->tmp1, cur->value);
		free(data->tmp1);
		data->tmp1 = NULL;
		cur = cur->next;
		i++;
	}
	data->evs[i] = NULL;
}

void	ft_external_functions(t_dat *data, char *line)
{
	char	***cmd;

	(void)line;
	if (!data || !data->xln || !data->xln[0])
		return ;
	if (!ft_validate_syntax(data->xln))
		return ;
	ft_list_to_env_array(data);
	data->no_pipes = ft_count_pipes(data->xln);
	if (!data->no_pipes && !ft_count_redirections(data->xln))
	{
		if (ft_all_valid_lvar(data, data->xln))
			ft_update_local_variables(data);
		if (ft_handle_builtin(data, data->st))
			return ;
	}
	cmd = ft_parse_cmd(data, 0, 0, 0);
	if (!cmd)
		return ;
	ft_execute_pipeline(data, cmd);
	ft_clean_cmd(cmd);
	ft_free_string_array(data->evs);
	data->evs = NULL;
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

char	***ft_clean_cmd(char ***cmd)
{
	int	i;

	if (!cmd)
		return (NULL);
	i = 0;
	while (cmd[i])
	{
		ft_free_string_array(cmd[i]);
		i++;
	}
	free(cmd);
	return (NULL);
}

int	ft_parse_cmd_helper(t_dat *d, char ***cmd, int *idx, int *st_i)
{
	int	i;

	i = st_i[1];
	if (i < st_i[0])
		return (0);
	if (!ft_validate_segment(d->xln, st_i[0], i))
		return (0);
	cmd[*idx] = ft_extract_tokens(d, st_i[0], i);
	if (!cmd[*idx])
		return (0);
	(*idx)++;
	st_i[0] = i + 1;
	return (1);
}

char	***ft_parse_cmd(t_dat *d, int st, int i, int idx)
{
	char	***cmd;
	int		st_i[2];

	d->k = ft_count_pipes(d->xln) + 1;
	cmd = ft_calloc(d->k + 1, sizeof(char **));
	if (!cmd)
		return (NULL);
	st_i[0] = st;
	while (1)
	{
		st_i[1] = i;
		if (!d->xln[i] || !ft_strcmp(d->xln[i], "|"))
		{
			if (!ft_parse_cmd_helper(d, cmd, &idx, st_i))
				return (ft_clean_cmd(cmd));
			if (!d->xln[i])
				break ;
		}
		i++;
	}
	cmd[idx] = NULL;
	d->tot = idx;
	return (cmd);
}

void	ft_free_fd(int **fd)
{
	int	i;

	if (!fd)
		return ;
	i = 0;
	while (fd[i])
	{
		free(fd[i]);
		i++;
	}
	free(fd);
}

int	**init_fd_array(int tot)
{
	int	**fd;
	int	i;

	fd = malloc(sizeof(int *) * tot);
	if (!fd)
		return (NULL);
	i = 0;
	while (i < tot - 1)
	{
		fd[i] = malloc(sizeof(int) * 2);
		if (!fd[i])
		{
			while (--i >= 0)
				free(fd[i]);
			free(fd);
			return (NULL);
		}
		i++;
	}
	fd[tot - 1] = NULL;
	return (fd);
}

int	ft_create_pipes(int **fd, int tot)
{
	int	i;

	i = 0;
	while (i < tot - 1)
	{
		if (pipe(fd[i]) == -1)
		{
			perror("pipe");
			while (i-- > 0)
				free(fd[i]);
			free(fd);
			return (0);
		}
		i++;
	}
	return (1);
}

void	ft_setup_io(int **fd, size_t i, size_t total)
{
	if (i > 0)
		dup2(fd[i - 1][0], STDIN_FILENO);
	if (i < total - 1)
		dup2(fd[i][1], STDOUT_FILENO);
}

int	ft_is_builtin(char *cmd)
{
	if (!ft_strcmp(cmd, "pwd"))
		return (1);
	if (!ft_strcmp(cmd, "cd"))
		return (1);
	if (!ft_strcmp(cmd, "echo"))
		return (1);
	if (!ft_strcmp(cmd, "exit"))
		return (1);
	if (!ft_strcmp(cmd, "export"))
		return (1);
	if (!ft_strcmp(cmd, "unset"))
		return (1);
	if (!ft_strcmp(cmd, "env"))
		return (1);
	return (0);
}

void	ft_execute_builtin_in_child(t_dat *d, char **cmd)
{
	if (!ft_strcmp(cmd[0], "pwd"))
		ft_pwd();
	else if (!ft_strcmp(cmd[0], "echo"))
		ft_echo(cmd, 0);
	else if (!ft_strcmp(cmd[0], "env"))
		ft_env(d);
}
