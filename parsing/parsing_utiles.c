/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utiles.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anel-men <anel-men@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 10:29:47 by anel-men          #+#    #+#             */
/*   Updated: 2025/06/08 20:29:54 by anel-men         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	get_or_set(int type, int status)
{
	static int	exit_status = 0;

	if (type == SET)
		exit_status = status;
	return (exit_status);
}

int	is_valid_key(char *key)
{
	int	i;

	if (!key || (!((key[0] >= 'a' && key[0] <= 'z')
				|| (key[0] >= 'A' && key[0] <= 'Z')
				|| key[0] == '_')))
		return (1);
	i = 1;
	while (key[i] && key[i] != '=')
	{
		if (!((key[i] >= 'a' && key[i] <= 'z')
				|| (key[i] >= 'A' && key[i] <= 'Z')
				|| (key[i] >= '0' && key[i] <= '9')
				|| key[i] == '_'))
			return (1);
		i++;
	}
	return (0);
}

void	print_ambiguous_redir_errors(t_cmd *cmd)
{
	t_cmd		*tmp;
	t_redir		*redir;
	char		*prefix;
	char		*suffix;

	prefix = "minishell: ";
	suffix = ": ambiguous redirect\n";
	tmp = cmd;
	while (tmp)
	{
		redir = tmp->redirs;
		while (redir)
		{
			if (redir->Ambiguous)
			{
				write(2, prefix, ft_strlen(prefix));
				if (redir->orig_token)
					write(2, redir->orig_token,
						ft_strlen(redir->orig_token));
				write(2, suffix, ft_strlen(suffix));
			}
			redir = redir->next;
		}
		tmp = tmp->next;
	}
}
