/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_func_1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anel-men <anel-men@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 14:55:39 by anel-men          #+#    #+#             */
/*   Updated: 2025/06/15 14:58:09 by anel-men         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	free_redirs(t_redir *redir_list)
{
	t_redir	*current;
	t_redir	*next;

	current = redir_list;
	while (current)
	{
		next = current->next;
		if (current->fd)
			free(current->fd);
		if (current->file)
			free(current->file);
		if (current->orig_token)
			free(current->orig_token);
		free(current);
		current = next;
	}
}

void	free_split_str(char **split_str)
{
	int	j;

	j = 0;
	if (!split_str)
		return ;
	while (split_str[j])
		free(split_str[j++]);
	free(split_str);
}

void	free_cmd_list_hp(char **str, char **str_befor)
{
	if (str)
		free_split_str(str);
	if (str_befor)
		free_split_str(str_befor);
}

void	free_cmd_list(t_cmd *cmd_list)
{
	t_cmd	*current;
	t_cmd	*next;

	current = cmd_list;
	while (current)
	{
		next = current->next;
		if (current->cmd)
		{
			free(current->cmd);
			current->cmd = NULL;
		}
		free_cmd_list_hp(current->args, current->args_befor_quotes_remover);
		if (current->redirs)
		{
			free_redirs(current->redirs);
			current->redirs = NULL;
		}
		free(current);
		current = next;
	}
}
