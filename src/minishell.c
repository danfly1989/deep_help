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
		if (line && *line && !ft_strisspace(line))
			ft_check_var_assign_and_expand_line(&data, line);
		if (line)
			free(line);
		line = NULL;
	}
	ft_cleanup_data(&data);
	return (g_last_exit_status);
}

void	ft_export_error(char *arg, char *message)
{
	write(2, "export: '", 9);
	write(2, arg, ft_strlen(arg));
	write(2, "': ", 3);
	write(2, message, ft_strlen(message));
	write(2, "\n", 1);
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
