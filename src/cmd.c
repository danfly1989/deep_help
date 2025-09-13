/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daflynn <daflynn@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 15:11:34 by daflynn           #+#    #+#             */
/*   Updated: 2025/09/13 15:11:40 by daflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_get_cmd_path_nested(const char *cmd)
{
	if (access(cmd, X_OK) == 0)
		return (ft_strdup(cmd));
	return (NULL);
}

char	*ft_get_cmd_path(t_dat *d, const char *cmd, int i)
{
	char	*full;

	if (!cmd || *cmd == '\0')
		return (NULL);
	if (cmd[0] == '/' || (cmd[0] == '.' && cmd[1] == '/'))
		return (ft_get_cmd_path_nested(cmd));
	d->tmp1 = ft_get_val_from_list(d->ev, "PATH");
	if (!d->tmp1)
		return (NULL);
	d->avs = ft_split(d->tmp1, ':');
	while (d->avs && d->avs[i])
	{
		full = ft_join_path(d->avs[i], (char *)cmd);
		if (access(full, X_OK) == 0)
		{
			ft_free_string_array(d->avs);
			return (full);
		}
		free(full);
		i++;
	}
	ft_free_string_array(d->avs);
	return (NULL);
}
