/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils_5.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anel-men <anel-men@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 11:48:37 by anel-men          #+#    #+#             */
/*   Updated: 2025/06/15 11:45:35 by anel-men         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	helper3(t_exp_helper *expand, int exit_status, int pipe_out)
{
	static int	was_there_a_pipe_befor = 0;

	if (pipe_out == 1)
		was_there_a_pipe_befor++;
	if (expand->original[expand->i] == '?')
	{
		if (was_there_a_pipe_befor == 1)
		{
			was_there_a_pipe_befor = 0;
			expand->var_value = ft_itoa(0);
		}
		else
			expand->var_value = ft_itoa(get_or_set(GET, 0));
		expand->i++;
		return (1);
	}
	return (0);
}

int	expand_handle_helper0(t_exp_helper *expand)
{
	if (expand->original[expand->i] == '\'')
	{
		if (expand->quote_state == 0)
			expand->quote_state = 1;
		else if (expand->quote_state == 1)
			expand->quote_state = 0;
		expand->expanded[expand->j++] = expand->original[expand->i++];
		return (1);
	}
	else if (expand->original[expand->i] == '"')
	{
		if (expand->quote_state == 0)
			expand->quote_state = 2;
		else if (expand->quote_state == 2)
			expand->quote_state = 0;
		expand->expanded[expand->j++] = expand->original[expand->i++];
		return (1);
	}
	return (0);
}
