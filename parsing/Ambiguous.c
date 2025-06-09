/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ambiguous.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anel-men <anel-men@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 10:15:39 by anel-men          #+#    #+#             */
/*   Updated: 2025/06/09 15:17:01 by anel-men         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	check_for_space(char *str)
{
	int	i;

	i = 0;
	while (str && str[i] && str[i] != ' ')
		i++;
	if (i > 0)
		return (1);
	return (0);
}

char	*trim_space(char *str)
{
	int		i;
	int		start;
	int		end;
	char	*trim_str;

	i = 0;
	while (str && str[i] && str[i] == ' ')
		i++;
	start = i;
	while (str && str[i] && str[i] != ' ')
		i++;
	end = i;
	trim_str = ft_substr(str, start, end - start);
	return (trim_str);
}

void	ambiguous_checker_helper(t_redir *tmp)
{
	char	*tp;

	tp = tmp->file;
	tmp->file = ft_strtrim(tmp->file, " ");
	free(tp);
}

void	ambiguous_checker(t_redir *redir)
{
	t_redir	*tmp;

	tmp = redir;
	while (tmp)
	{
		if ((tmp->orig_token[0] == '\'' && tmp->orig_token[1] == '\'') || (tmp->orig_token[0] == '\"' && tmp->orig_token[1] == '\"'))
			tmp->Ambiguous = 1;
		if ((tmp->orig_token[0] == '\'' || tmp->orig_token[0] == '\"')
			&& (tmp->orig_token[ft_strlen(tmp->orig_token) - 1] == '\''
				|| tmp->orig_token[ft_strlen(tmp->orig_token) - 1] == '\"'))
		{
			tmp = tmp->next;
			continue ;
		}
		if (!tmp->file || tmp->file[0] == '\0')
			tmp->Ambiguous = 1;
		else if (tmp->file && check_for_space(tmp->file) == 1
			&& tmp->orig_token[0] == '$')
			tmp->Ambiguous = 1;
		else
			ambiguous_checker_helper(tmp);
		tmp = tmp->next;
	}
}

void	ambiguous_finder(t_cmd *cmd)
{
	t_cmd	*tmp;

	tmp = cmd;
	while (tmp)
	{
		ambiguous_checker(tmp->redirs);
		tmp = tmp->next;
	}
}
