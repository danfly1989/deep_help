/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dagwu <dagwu@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 18:44:41 by dagwu             #+#    #+#             */
/*   Updated: 2025/06/14 18:44:46 by dagwu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static volatile sig_atomic_t	g_last_exit_status = 0;

int	main(int argc, char *argv[], char *env[])
{
	char	*line;
	t_dat	data;

	data = ft_duplicate_input_args(argc, argv, env);
	ft_set_main_signals();
	while (1)
	{
		line = readline("dandav>");
		if (line == NULL)
		{
			if (ft_is_interactive())
				write(1, "exit\n", 5);
			break ;
		}
		if (line && *line && !ft_strisspace(line) && ft_strcmp(line, "<<"))
			add_history(line);
		// Only process non-empty, non-whitespace lines
		if (line && *line && !ft_strisspace(line))
			ft_check_var_assign_and_expand_line(&data, line);
		if (line)
			free(line);
		line = NULL;
	}
	ft_cleanup_data(&data);
	return (g_last_exit_status);
}

int	ft_is_pipe_builtin(char *cmd)
{
	if (!ft_strcmp(cmd, "pwd"))
		return (1);
	if (!ft_strcmp(cmd, "echo"))
		return (1);
	if (!ft_strcmp(cmd, "env"))
		return (1);
	return (0);
}

// THIS AND FOLLOWING CONTAIN GLOBAL VARIABLE
void	ft_nested_sigint_handler(int sig)
{
	(void)sig;
	rl_replace_line("", 0);
	rl_redisplay();
	g_last_exit_status = 130;
}

void	ft_parent_sigint_handler(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	// rl_redisplay();
	g_last_exit_status = 130;
}

void	ft_change_directory(t_dat *data, size_t k)
{
	char	*path;
	char	*oldpwd;

	if (data->xln[k + 1] != NULL && data->xln[k + 2] != NULL)
	{
		write(2, "minishell: cd: too many arguments\n", 34);
		g_last_exit_status = 1;
		return ;
	}
	oldpwd = getcwd(NULL, 0);
	if (data->xln[k + 1] == NULL || ft_strcmp(data->xln[k + 1], "~") == 0)
	{
		path = ft_get_val_from_list(data->ev, "HOME");
		if (path == NULL)
		{
			write(2, "cd: HOME not set\n", 17);
			g_last_exit_status = 1;
			return ;
		}
	}
	else
		path = data->xln[k + 1];
	if (chdir(path) == 0)
		ft_update_directories(data, oldpwd);
	else
	{
		ft_cd_error(path);
		g_last_exit_status = 1;
	}
}

void	ft_export_error(char *arg, char *message)
{
	write(2, "export: '", 9);
	write(2, arg, ft_strlen(arg));
	write(2, "': ", 3);
	write(2, message, ft_strlen(message));
	write(2, "\n", 1);
	// only this line is new
	g_last_exit_status = 1;
}

void	ft_exec_command(t_dat *d, char **cmd)
{
	char	*cmd_path;

	if (!cmd || !cmd[0])
		exit(127);
	if (ft_is_pipe_builtin(cmd[0]))
	{
		ft_execute_builtin_in_child(d, cmd);
		exit(g_last_exit_status);
	}
	cmd_path = ft_get_cmd_path(d, cmd[0], 0);
	if (!cmd_path)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd[0], 2);
		ft_putendl_fd(": command not found", 2);
		exit(127);
	}
	execve(cmd_path, cmd, d->evs);
	free(cmd_path);
	perror("execve");
	exit(1);
}

// END OF GLOBAL VARIABLE
// MORE TEXT TO MAKE THIS MARKER
// MORE OBVIOUSLY
// APPARENTLY
// CLEARLY
// DISTINCTLY
// UNMISTAKABLY
// CONSPICUOUSLY
// MANIFESTLY
// PATENTLY
// PALPABLY
// HERE

void	ft_appropriate_child_signal(char *str)
{
	if (ft_is_builtin(str) && ft_is_pipe_builtin(str))
		ft_set_main_signals();
	else
		ft_set_child_signals();
}

int	ft_is_interactive(void)
{
	return (isatty(STDIN_FILENO) && isatty(STDOUT_FILENO));
}

void	ft_child_process(t_dat *d, char ***cmd, int **fd, size_t i)
{
	t_rdr	r;
	int		len;

	if (!cmd || !cmd[i] || !cmd[i][0])
		exit(1);
	len = 0;
	while (cmd[i][len])
		len++;
	ft_memset(&r, 0, sizeof(r));
	if (!ft_validate_segment(cmd[i], 0, len))
		exit(1);
	ft_setup_io(fd, i, d->tot);
	ft_close_pipes(fd, d->tot);
	ft_appropriate_child_signal(cmd[i][0]);
	ft_parse_redirection(cmd[i], &r);
	if (!ft_apply_redirections(&r, cmd[i]))
	{
		ft_free_redirection(&r);
		exit(1);
	}
	ft_exec_command(d, cmd[i]);
}

int	ft_syntax_error_msg(char *token)
{
	char	*prefix;
	char	*newline;

	prefix = "minishell: syntax error near unexpected token `";
	newline = "'\n";
	if (token)
	{
		write(2, prefix, ft_strlen(prefix));
		write(2, token, ft_strlen(token));
		write(2, newline, 2);
	}
	else
	{
		write(2, prefix, ft_strlen(prefix));
		write(2, "newline", 7);
		write(2, newline, 2);
	}
	return (0);
}

int	ft_validate_segment(char **tokens, int start, int end)
{
	int	i;

	if (!tokens || start >= end)
		return (0);
	i = start;
	while (i < end)
	{
		if (ft_is_redirection(tokens[i]))
		{
			if (i + 1 >= end || ft_is_redirection(tokens[i + 1])
				|| !ft_strcmp(tokens[i + 1], "|"))
			{
				return (ft_syntax_error_msg(tokens[i + 1]));
			}
		}
		i++;
	}
	return (1);
}

void	ft_fork_children(t_dat *d, char ***cmd, int **fd)
{
	pid_t	pid;
	size_t	i;

	i = 0;
	// Remove this line: ft_set_child_signals();
	while (i < d->tot)
	{
		if (!cmd[i] || !cmd[i][0])
		{
			i++;
			continue ;
		}
		pid = fork();
		if (pid == 0)
		{
			ft_set_child_signals(); // Move it here - only in child!
			ft_child_process(d, cmd, fd, i);
		}
		else if (pid < 0)
			perror("fork");
		i++;
	}
	// Keep parent signals as they were - don't call ft_set_main_signals() again
}

void	ft_close_pipes(int **fd, int tot)
{
	int	i;

	i = 0;
	while (i < tot - 1)
	{
		close(fd[i][0]);
		close(fd[i][1]);
		i++;
	}
}

void	ft_wait_children(t_dat *d, int tot)
{
	int	status;
	int	i;
	int	signal_num;

	(void)d;
	i = 0;
	while (i < tot)
	{
		waitpid(-1, &status, 0); // wait for any child
		if (WIFSIGNALED(status))
		{
			signal_num = WTERMSIG(status);
			if (signal_num == SIGQUIT)
			{
				printf("We want this message to appear on blocking commands but never on heredoc\n");
				g_last_exit_status = 131;
			}
			else if (signal_num == SIGINT)
			{
				// Newline for interrupted commands
				write(1, "\n", 1);
				g_last_exit_status = 130;
			}
		}
		else if (WIFEXITED(status))
		{
			g_last_exit_status = WEXITSTATUS(status);
		}
		i++;
	}
}

void	ft_nested_parent(t_dat *d, pid_t pid, int saved_stdin)
{
	int	status;
	int	signal_num;

	(void)d;
	status = 0;
	while (waitpid(pid, &status, 0) == -1 && errno == EINTR)
		;
	if (WIFEXITED(status))
		g_last_exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status)) // Add this block
	{
		signal_num = WTERMSIG(status);
		if (signal_num == SIGQUIT)
		{
			write(1, "^\\", 2); // Print the ^\ that was pressed
			// ft_printf("minishell: quit (core dumped)\n");
			g_last_exit_status = 131;
		}
		else if (signal_num == SIGINT)
		{
			write(1, "\n", 1);
			g_last_exit_status = 130;
		}
	}
	if (dup2(saved_stdin, STDIN_FILENO) < 0)
		perror("dup2");
	close(saved_stdin);
}

void	ft_nested_child(t_dat *d, char **cmd, char *cmd_path, int s_stdin)
{
	close(s_stdin);
	cmd_path = ft_get_cmd_path(d, cmd[0], 0);
	if (!cmd_path)
	{
		ft_cmd_not_found(cmd[0]);
		exit(1);
	}
	execve(cmd_path, cmd, d->evs);
	exit(1);
}

void	ft_nested_minishell(t_dat *d, char **cmd, char *cmd_path)
{
	pid_t	pid;
	int		saved_stdin;

	saved_stdin = dup(STDIN_FILENO);
	if (saved_stdin < 0)
	{
		perror("dup");
		return ;
	}
	ft_set_main_nested_signals();
	pid = fork();
	if (pid == 0)
		ft_nested_child(d, cmd, cmd_path, saved_stdin);
	else if (pid > 0)
		ft_nested_parent(d, pid, saved_stdin);
	else
		perror("nesting minishell fork");
	ft_set_main_signals();
}

void	ft_execute_pipeline(t_dat *d, char ***cmd)
{
	int	**fd;

	if (ft_strcmp(cmd[0][0], "./minishell") == 0)
	{
		ft_nested_minishell(d, cmd[0], NULL);
		return ;
	}
	fd = init_fd_array(d->tot);
	if (!fd || !ft_create_pipes(fd, d->tot))
	{
		if (fd)
			ft_free_fd(fd);
		return ;
	}
	ft_fork_children(d, cmd, fd);
	ft_close_pipes(fd, d->tot);
	ft_wait_children(d, d->tot);
	ft_set_main_signals();
	ft_free_fd(fd);
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

char	*append_exit_status(char *res, int status, int *i)
{
	char	*temp;

	temp = ft_itoa(status);
	res = ft_strjoin_free(res, temp);
	free(temp);
	*i += 2;
	return (res);
}

char	*ft_expand_exit_status(t_dat *d, char *token)
{
	char	*res;
	int		i;

	(void)d;
	i = 0;
	res = malloc(1);
	if (!res)
		return (NULL);
	res[0] = '\0';
	while (token[i])
	{
		if (token[i] == '$' && token[i + 1] == '?')
			res = append_exit_status(res, g_last_exit_status, &i);
		else
			res = append_char(res, token, &i);
	}
	return (res);
}
