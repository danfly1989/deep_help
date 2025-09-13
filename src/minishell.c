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
