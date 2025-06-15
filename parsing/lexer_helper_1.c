/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_helper_1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anel-men <anel-men@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 11:07:47 by anel-men          #+#    #+#             */
/*   Updated: 2025/06/15 17:29:37 by anel-men         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n'
		|| c == '\r' || c == '\v' || c == '\f');
}

int	special_character_cheker(char character)
{
	if (character == '|')
		return (1);
	return (0);
}

t_token	*special_character_handle(char *input, int *i)
{
	t_token	*node;
	int		buffer_index;

	buffer_index = 0;
	node = NULL;
	if (input[(*i)] == ' ')
		(*i)++;
	if (input[(*i)] == '|' )
	{
		node = creat_token("|", "pipe_token");
		(*i)++;
	}
	return (node);
}

void	handle_block_helper(char *input, int *i, t_token **token_list)
{
	t_token	*node;

	node = special_character_handle(input, i);
	if (node)
		ft_lstadd_back_token(token_list, node);
}
